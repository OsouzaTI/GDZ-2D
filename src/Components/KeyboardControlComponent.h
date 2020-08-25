#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H

#include "../Game.h"
#include "../EntityManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Constants.h"

class KeyboardControlComponent: public Component {

    public:
        std::string upKey;
        std::string downKey;
        std::string rightKey;
        std::string leftKey;
        std::string shootKey;
        TransformComponent *transform;
        SpriteComponent *sprite;

        KeyboardControlComponent(){

        }

        KeyboardControlComponent(
            std::string _upKey,
            std::string _downKey,
            std::string _rightKey,
            std::string _leftKey,
            std::string _shootKey
        ){
            this->upKey     = GetSDLKeyStringCode(_upKey);
            this->downKey   = GetSDLKeyStringCode(_downKey);
            this->rightKey  = GetSDLKeyStringCode(_rightKey);
            this->leftKey   = GetSDLKeyStringCode(_leftKey);
            this->shootKey  = GetSDLKeyStringCode(_shootKey);
        }

        std::string GetSDLKeyStringCode(std::string key){
            if (key.compare("up") == 0) return "1073741906";
            if (key.compare("down") == 0) return "1073741905";
            if (key.compare("left") == 0) return "1073741904";
            if (key.compare("right") == 0) return "1073741903";
            if (key.compare("space") == 0) return "32"; 
            std::string logKey = std::to_string(static_cast<int>(key[0]));
            messageInfo("Key Get::[" << static_cast<int>(key[0]) << "]");
            messageInfo("Key ToString::[" << logKey << "]");
            messageInfo("Key Normal::[" << key[0] << "]");
            return logKey;
        }

        void Initialize() override {
            transform = owner->GetComponent<TransformComponent>();
            sprite = owner->GetComponent<SpriteComponent>();
        }

        void Update(float deltaTime) override {

            if(Game::event.type == SDL_KEYDOWN){
                std::string keyCode = std::to_string(Game::event.key.keysym.sym);
                
                if(keyCode.compare(upKey) == 0){
                    
                    transform->velocity.y = -1;
                    transform->velocity.x = 0;
                    sprite->Play("UpAnimation");
                    
                }

                if(keyCode.compare(downKey) == 0){
                    
                    transform->velocity.y = 1;
                    transform->velocity.x = 0;
                    sprite->Play("DownAnimation");

                }

                if(keyCode.compare(rightKey) == 0){
                    
                    transform->velocity.y = 0;
                    transform->velocity.x = 1;
                    sprite->Play("RightAnimation");

                }

                if(keyCode.compare(leftKey) == 0){
                    
                    transform->velocity.y = 0;
                    transform->velocity.x = -1;
                    sprite->Play("LeftAnimation");

                }

                if(keyCode.compare(shootKey) == 0){
                    //TODO
                    //Shoot algo poh
                }

            }

            if(Game::event.type == SDL_KEYUP){
                
                std::string keyCode = std::to_string(Game::event.key.keysym.sym);
                
                if(keyCode.compare(upKey) == 0){                    
                    transform->velocity.y = 0;
                }

                if(keyCode.compare(downKey) == 0){                    
                    transform->velocity.y = 0;
                }

                if(keyCode.compare(rightKey) == 0){                    
                    transform->velocity.x = 0;                    
                }

                if(keyCode.compare(leftKey) == 0){                    
                    transform->velocity.x = 0;
                }

                if(keyCode.compare(shootKey) == 0){
                    //TODO
                }

            }

        }

};

#endif