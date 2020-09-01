#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include <SDL2/SDL.h>
#include "../EntityManager.h"
#include "../AssetManager.h"
#include "../../lib/glm/glm.hpp"

class TileComponent: public Component {

    public:
        SDL_Texture *texture;
        SDL_Rect sourceRectangle;
        SDL_Rect destinationRectangle;
        glm::vec2 position;
        /**
         * @brief Construct a new Tile Component object
         * 
         * @param sourceRectX the tile rectangle source x
         * @param sourceRectY the tile rectangle source y
         * @param x position x
         * @param y position y
         * @param tileSize size of tile
         * @param scale the scale 
         * @param assetTextureId the texture id of asset manager
         */
        TileComponent(
            int sourceRectX,
            int sourceRectY,
            int x,
            int y,
            int tileSize,
            int scale,
            std::string assetTextureId
        ){
            texture = Game::assetManager->GetTexture(assetTextureId);
            sourceRectangle.x = sourceRectX;
            sourceRectangle.y = sourceRectY;
            sourceRectangle.w = tileSize;
            sourceRectangle.h = tileSize;

            destinationRectangle.x = x;
            destinationRectangle.y = y;
            destinationRectangle.w = tileSize * scale;
            destinationRectangle.h = tileSize * scale;

            position.x = x;
            position.y = y;

        }

        ~TileComponent(){
            SDL_DestroyTexture(texture);            
        }

    void Update(float deltaTime) override {
        destinationRectangle.x = position.x - Game::camera.x;
        destinationRectangle.y = position.y - Game::camera.y;
    }

    void Render() override {
        TextureManager::Draw(
            texture,
            sourceRectangle,
            destinationRectangle,
            SDL_FLIP_NONE
        );
    }


};

#endif