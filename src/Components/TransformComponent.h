#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "../Constants.h"
#include "../EntityManager.h"
#include "../../lib/glm/glm.hpp"
#include <SDL2/SDL.h>
#include "../Game.h"

class TransformComponent: public Component {

    public:

        glm::vec2 position;
        glm::vec2 velocityFixed;
        glm::vec2 velocity;
        int width;
        int height;
        int scale;
        /**
         * @brief Construct a new Transform Component object
         * 
         * @param posX the position x
         * @param posY the y position
         * @param velX the velocity x
         * @param velY the velocity y
         * @param w the width of transform
         * @param h the height of transform
         * @param s the scale of transform
         * @param isPlayer if it's tranform player
         */
        TransformComponent(int posX, int posY, int velX, int velY, int w, int h, int s, bool isPlayer = false){
            position = glm::vec2(posX, posY);

            if(!isPlayer){
                velocityFixed = glm::vec2(1, 1);  
                velocity = glm::vec2(velX, velY);
            }else{
                velocityFixed = glm::vec2(velX, velY);                             
                velocity = glm::vec2(0, 0);  
            }
            
            
            width = w;
            height = h;
            scale = s;
        }
   
        void Initialize() override {

        }

        void Update(float deltaTime) override {
            position.x += (velocity.x * velocityFixed.x) * deltaTime;
            position.y += (velocity.y * velocityFixed.y) * deltaTime;
        }

        void Render() override {
            
            // SDL_Rect tranformRectangle = {
            //     (int) position.x,
            //     (int) position.y,
            //     width,
            //     height
            // };
            // SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
            // SDL_RenderFillRect(Game::renderer, &tranformRectangle);
        }

};


#endif