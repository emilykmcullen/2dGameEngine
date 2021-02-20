#include <iostream>
#include "./Constants.h"
#include "./Game.h"
using namespace std;

Game::Game(){
    this->isRunning =false;
}

Game::~Game(){

}

bool Game::IsRunning() const {
    return this->isRunning;
}

float projectilePosX = 0.0f;
float projectilePosY = 0.0f;
float projectileVelX = 0.5f;
float projectileVelY = 0.5f;

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

    isRunning = true;
    return;
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
    //Wait until 16ms have elapsed since the last frame
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME));
    
    // delta time is the difference in ticks from the last frame, converted to seconds
    float deltaTime =(SDL_GetTicks() - ticksLastFrame)/1000.0f;

    //Sets the new ticks for the current frame to be used in the next pass
    ticksLastFrame = SDL_GetTicks();

    projectilePosX += projectileVelX * deltaTime;
    projectilePosY += projectileVelY * deltaTime;
}

void Game::Render(){
    SDL_SetRenderDrawColor(renderer, 21, 21,21 , 255);
    SDL_RenderClear(renderer);

    SDL_Rect projectile {
        (int) projectilePosX,
        (int) projectilePosY,
        10,
        10
    };

    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    SDL_RenderFillRect(renderer, &projectile);

    SDL_RenderPresent(renderer);
}

void Game::Destroy(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}