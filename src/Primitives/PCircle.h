#ifndef PCIRCLE_H
#define PCIRCLE_H

#include "Primitives.h"
#include "../Game.h"


class PCircle {

    public:

        Circle circle;

        PCircle(int x = 10, int y = 10, int radius = 5){
            this->circle.center.x = x;
            this->circle.center.y = y;
            this->circle.radius   = radius;
        }

        ~PCircle(){};

        void RenderCirle(SDL_Renderer* renderer, bool fillRect = false){
            std::cout << "Primitive Renderer::PCircle" << std::endl;
            int cx = this->circle.center.x;
            int cy = this->circle.center.y;
            int radius = this->circle.radius;

            if(fillRect){

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                for (int i = 0; i < radius * 2; i++)
                {
                    for (int j = 0; j < radius * 2; j++)
                    {
                        int dx = radius - i;
                        int dy = radius - j;
                        if ((dx*dx + dy*dy) <= (radius * radius))
                        {
                            SDL_RenderDrawPoint(renderer, cx + dx, cy + dy);
                        }
                        
                    }              

                }            
            }
            else{
                
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                for (int i = 0; i < 360; i++)
                {
                    
                    IPoint point;
                    point.x = cx + radius * SDL_cosf((float)i);
                    point.y = cy + radius * SDL_sinf((float)i);

                     SDL_RenderDrawPoint(renderer, point.x, point.y);

                }                

            }
        }
 

};



#endif