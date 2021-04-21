#include <iostream>
#include "./Constants.h"
#include "./Game.h"
#include "../lib/glm/glm.hpp"
using namespace std;
#include "EntityManager.h"
#include "./Components/TransformComponent.h"
#include "./Components/SpriteComponent.h"
#include "./AssetManager.h"
#include "./Components/KeyboardControlComponent.h"
#include "./Map.h"
#include "./Components/ColliderComponent.h"
#include "./Components/TextLabelComponent.h"
#include "./Components/ProjectileEmitterComponent.h"


EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Entity* mainPlayer = NULL;
Map* gameMap;


Game::Game(){
    this->isRunning =false;
}

Game::~Game(){

}

bool Game::IsRunning() const {
    return this->isRunning;
}


void Game::Initialize(int width, int height) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        cerr << "Error initializing SDL" << endl;
        return;
    }
    if (TTF_Init() !=0) {
        cerr << "Error initializing SDL TTF" <<endl;
    }
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_BORDERLESS
    );
    if(!window){
        cerr << "Error creating SDL window." << endl;
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        cerr << "Error creating SDL renderer." << endl;
        return;
    }

    LoadLevel(1);
    isRunning = true;
    return;
}


void Game::LoadLevel(int levelNumber){

    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);
    //these libraries are now available to me

    std::string levelName = "Level" + std::to_string(levelNumber);
    lua.script_file("./assets/scripts/" + levelName + ".lua");


    /***************************************************/
    /* LOAD ASSETS FROM LUA CONFIG FILE */
    /***************************************************/

    sol::table levelData = lua[levelName];
    sol::table levelAssets = levelData["assets"];
    //will make a table of all the data in the file

    unsigned int assetIndex = 0;
    while (true) {
        sol::optional<sol::table> existsAssetIndexNode = levelAssets[assetIndex];
        //while loop will run undex index of assets doesnt exist
        if (existsAssetIndexNode == sol::nullopt) {
            break;
        } else {
            sol::table asset = levelAssets[assetIndex];
            std::string assetType = asset["type"];
            if ( assetType.compare("texture") == 0) {
                std::string assetId = asset["id"];
                std:string assetFile = asset["file"];
                assetManager->AddTexture(assetId, assetFile.c_str());
            }
        }
        assetIndex++;
    }

     /***************************************************/
    /* LOADS MAP FROM LUA CONFIG FILE */
    /***************************************************/

    sol::table levelMap = levelData["map"];
    std::string mapTextureId = levelMap["textureAssetId"];
    std::string mapFile = levelMap["file"];

    gameMap = new Map(
        mapTextureId,
        static_cast<int>(levelMap["scale"]),
        static_cast<int>(levelMap["tileSize"])
    );

    gameMap->LoadMap(
        mapFile,
        static_cast<int>(levelMap["mapSizeX"]),
        static_cast<int>(levelMap["mapSizeY"])
    );
    
    
}

void Game::ProcessInput(){
    SDL_PollEvent(&event);
    switch (event.type){
        case SDL_QUIT:{
            isRunning = false;
            break;
        }
        case SDL_KEYDOWN: {
            if (event.key.keysym.sym == SDLK_ESCAPE){
                isRunning =false;
            }
        }
        default: {
            break;
        }
    }
}

void Game::Update(){
    // while(!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME));

    //Sleep the execution until we reach the target frame time in milliseconds
    int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);

    //Only call delay if we are too fast to process this frame
    if(timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME){
        SDL_Delay(timeToWait);
    }
    
    // delta time is the difference in ticks from the last frame, converted to seconds
    float deltaTime =(SDL_GetTicks() - ticksLastFrame)/1000.0f;

    //Clamp deltaTime to a maximum value
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;


    //Sets the new ticks for the current frame to be used in the next pass
    ticksLastFrame = SDL_GetTicks();

    manager.Update(deltaTime);

    HandleCameraMovement();
    CheckCollisions();

    
}

void Game::Render(){
    SDL_SetRenderDrawColor(renderer, 21, 21,21 , 255);
    SDL_RenderClear(renderer);

    if(manager.HasNoEntities()){
        return;
    }
    manager.Render();

    SDL_RenderPresent(renderer);
}

void Game::HandleCameraMovement() {
    if (mainPlayer) {
        TransformComponent* mainPlayerTransform = mainPlayer->GetComponent<TransformComponent>();
        camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2); //as soon as player reaches half of the window, only then the camera follows
        camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

        //clamping camera values so doesnt go offscreen
        camera.x = camera.x < 0 ? 0 : camera.x;
        camera.y = camera.y < 0 ? 0 : camera.y;
        camera.x = camera.x > camera.w ? camera.w : camera.x;
        camera.y = camera.y > camera.h ? camera.h : camera.y;
    }
}

void Game::CheckCollisions() {
    CollisionType collisionType = manager.CheckCollisions(); 
    if (collisionType == PLAYER_ENEMY_COLLISION) {
        ProcessGameOver();
    }
    if (collisionType == PLAYER_PROJECTILE_COLLISION) {
        ProcessGameOver();
    }
    if (collisionType == PLAYER_LEVEL_COMPLETE_COLLISION) {
        ProcessNextLevel(1);
    }
}

void Game::ProcessNextLevel(int levelNumber) {
    std::cout << "Next Level" << std::endl;
    isRunning = false;
}

void Game::ProcessGameOver() {
    std::cout << "Game Over" << std::endl;
    isRunning = false;
}


void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}