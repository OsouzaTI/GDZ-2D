#ifndef COMPONENT_H
#define COMPONENT_H

//#include "Constants.h"

class Entity;

class Component {

    public:
        Entity *owner;
        virtual ~Component(){};
        /**
        * @brief this method initialize the sprite component
        * 
        */
        virtual void Initialize(){}
        /**
         * @brief method update of this component
         * 
         * @param deltaTime 
         * the rate of update game loop
         */
        virtual void Update(float deltaTime){};
        /**
         * @brief 
         * method render of this object
         */
        virtual void Render(){};

};

#endif