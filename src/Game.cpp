#include <iostream>
#include "./Constants.h"
#include "./Game.h"
#include "../lib/glm/glm.hpp"
using namespace std;
#include "EntityManager.h"
#include "./Components/TransformComponent.h"
#include "./Components/SpriteComponent.h"
#include "./AssetManager.h"


EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;

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

    LoadLevel(0);
    manager.ListAllEntities();
    isRunning = true;
    return;
}

void Game::LoadLevel(int levelNumber){
    //Start including new assets to the assetmanager list
    std::string textureFilePath1 = "./assets/images/rock-big-1.png";
    std::string textureFilePath2 = "./assets/images/tree-small-1.png";
    assetManager->AddTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
    assetManager->AddTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());

    //add entities and add components to entities
    Entity& tankEntity(manager.AddEntity("tank"));
    tankEntity.AddComponent<TransformComponent>(0,0,20,20,32,32,1);
    tankEntity.AddComponent<SpriteComponent>("tank-image");

    Entity& chopperEnitity(manager.AddEntity("chopper"));
    chopperEnitity.AddComponent<TransformComponent>(240,106, 0,0,32,32,1);
    chopperEnitity.AddComponent<SpriteComponent>("chopper-image");

}

void Game::ProcessInput(){
    SDL_Event event;
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

void Game::Destroy(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}