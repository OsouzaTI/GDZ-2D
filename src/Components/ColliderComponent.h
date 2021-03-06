#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <SDL2/SDL.h>
#include "../Game.h"
#include "../EntityManager.h"
#include "../AssetManager.h"
#include "../Component.h"
#include "TransformComponent.h"
/**
 * @brief Collision class component 
 */
class ColliderComponent: public Component {
    public:
        /**
         * @brief the tag of collider
         */
        std::string colliderTag;
        /**
         * @brief the collider rectangle
         */
        SDL_Rect collider;
        /**
         * @brief the souce rectangle collider
         */
        SDL_Rect sourceRectangle;
        /**
         * @brief the destination rectangle collider
         * 
         */
        SDL_Rect destinationRectangle;
        /**
         * @brief Texture of collider
         */
        SDL_Texture* texture;
        /**
         * @brief the transform of owner, the owner it's a entity
         */
        TransformComponent* transform;
        /**
         * @brief Construct a new Collider Component object
         * 
         * @param colliderTag Collider tag used for detect type collision
         * @param x the x initial for SDL_Rect of collider
         * @param y the y initial for SDL_Rect of collider
         * @param width the width for SDL_Rect of collider
         * @param height the height for SDL_Rect of collider
         */
        ColliderComponent(std::string colliderTag, int x, int y, int width, int height){
            this->colliderTag = colliderTag;
            this->collider = {x, y, width, height};
        }
        /**
         * @brief Set the Texture of Collider
         */
        void setTexture(){
            texture = Game::assetManager->GetTexture("collider");
        }
        /**
         * @brief the initialization method of collider, this method
         * load transform, sourceRectangle, destinationRectangle and
         * set the texture
         */
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