#pragma once

#include "../../lib/glm/glm.hpp"
#include "../EntityManager.h"
#include "../Components/TransformComponent.h"

class ProjectileEmitterComponent: public Component {
    private:
        TransformComponent *transform; //transform of owner entity
        glm::vec2 origin;
        int speed;
        int range;
        float angleRadians;
        bool shouldLoop;
    public:
        ProjectileEmitterComponent(int speed, int angleDegrees, int range, bool shouldLoop){
            this->speed = speed;
            this->range = range;
            this->shouldLoop = shouldLoop;
            this->angleRadians = glm::radians(static_cast<float>(angleDegrees));
        }

        void Initialize() override {
            transform = owner->GetComponent<TransformComponent>();
            origin = glm::vec2(transform->position.x, transform->position.y);
            //getting the velocity from x and y component of the vector (I know the speed and direction)
            transform->velocity = glm::vec2(glm::cos(angleRadians) * speed, glm::sin(angleRadians) * speed);
        }

        void Update(float deltaTime) override {
            if (glm::distance(transform->position, origin) > range) {
                if (shouldLoop) {
                    //set back to origin position
                    transform->position.x = origin.x;
                    transform->position.y = origin.y;
                }
                else {
                    owner->Destroy();
                }
            }
        }
};