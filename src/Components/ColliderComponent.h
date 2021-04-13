#pragma once

#include "../Game.h"
#include "../EntityManager.h"
#include "./TransformComponent.h"
#include <SDL2/SDL.h>

class ColliderComponent: public Component {
    public:
        std::string colliderTag;
        SDL_Rect collider; //of the entity 
        SDL_Rect sourceRectangle;
        SDL_Rect destinationRectangle;
        TransformComponent* transform;
        

        ColliderComponent(std::string colliderTag, int x, int y, int width, int height) {
            this->colliderTag = colliderTag;
            this->collider = {x , y , width, height};
        }

        void Initialize() override {
            //get and set the transform, only IF the entity has a transform component
            if (owner->HasComponent<TransformComponent>()){
                transform = owner->GetComponent<TransformComponent>();
                sourceRectangle = {0, 0, transform->width, transform->height};
                destinationRectangle = {collider.x, collider.y, collider.w, collider.h};
            }
        }

        void Update(float deltaTime) override {
            collider.x = static_cast<int>(transform->position.x);
            collider.y = static_cast<int>(transform->position.y);
            collider.w = transform->width * transform->scale;
            collider.h = transform->height * transform->scale;
            destinationRectangle.x = collider.x - Game::camera.x;
            destinationRectangle.y = collider.y - Game::camera.y;
        }


        //just used for viewing the bounding boxes, can be removed later
        void Render() override {
            SDL_SetRenderDrawColor(Game::renderer, 255,255,255,255);
            SDL_RenderDrawRect(Game::renderer, &destinationRectangle);
            SDL_RenderDrawRect(Game::renderer, &sourceRectangle);

        }
};