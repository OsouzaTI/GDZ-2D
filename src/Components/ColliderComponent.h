#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <SDL2/SDL.h>
#include "../Game.h"
#include "../EntityManager.h"
#include "../AssetManager.h"
#include "../Component.h"
#include "TransformComponent.h"

class ColliderComponent: public Component {
    public:
        std::string colliderTag;
        SDL_Rect collider;
        SDL_Rect sourceRectangle;
        SDL_Rect destinationRectangle;
        SDL_Texture* texture;
        TransformComponent* transform;

        ColliderComponent(std::string colliderTag, int x, int y, int width, int height){
            this->colliderTag = colliderTag;
            this->collider = {x, y, width, height};
        }

        void setTexture(){
            texture = Game::assetManager->GetTexture("collider");
        }

        void Initialize() override {
            if(owner->HasComponent<TransformComponent>()){
                transform = owner->GetComponent<TransformComponent>();
                sourceRectangle = {0, 0, transform->width, transform->height};
                destinationRectangle = {collider.x, collider.y, collider.w, collider.h};
            }
            setTexture();
        }

        void Update(float deltaTime) override {
            collider.x = static_cast<int>(transform->position.x);
            collider.y = static_cast<int>(transform->position.y);
            collider.w = transform->width  * transform->scale;
            collider.h = transform->height * transform->scale;

            destinationRectangle.x = collider.x - Game::camera.x;
            destinationRectangle.y = collider.y - Game::camera.y;
        }

        void Render()override{
            TextureManager::Draw(texture, sourceRectangle, destinationRectangle, SDL_FLIP_NONE);
        }
};


#endif