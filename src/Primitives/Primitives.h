#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "../GDZmath.h"
#include <iostream>
#include <SDL2/SDL.h>


typedef struct {

    int x;
    int y;

    IPoint(int x = 0, int y = 0){
        this->x = x;
        this->y = y;
    };

} IPoint;


typedef struct
{

    IPoint center;
    int radius; 
    
} Circle;


#endif
