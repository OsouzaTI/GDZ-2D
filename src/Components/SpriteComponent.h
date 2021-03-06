#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>
#include "../TextureManager.h"
#include "../AssetManager.h"
#include "../Animation.h"
/**
 * @brief this class is sprite renderer in the entity object
 * 
 */
class SpriteComponent: public Component {

    private:
        TransformComponent* transform;
        SDL_Texture* texture;
        SDL_Rect sourceRectangle;
        SDL_Rect destinationRectangle;

        bool isAnimated;
        int numFrames;
        int animationSpeed;
        bool isFixed;
        std::map<std::string, Animation> animations;
        std::string currentAnimationName;
        unsigned int animationIndex = 0;

    public:
        SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
        /**
         * @brief Construct a new Sprite Component object
         * 
         * @param assetTextureId the asset texture id is the same id in asset manager
         */
        SpriteComponent(std::string assetTextureId){
            this->isAnimated = false;
            this->isFixed = false;
            SetTexture(assetTextureId);
        }

                
        SpriteComponent(std::string assetTextureId, bool isFixed) {
            this->isAnimated = false;
            this->isFixed = isFixed;
            SetTexture(assetTextureId);
        }

        /**
         * @brief Construct a new Sprite Component object
         * 
         * @param id the asset texture id is the same id in asset manager
         * @param numFrames number of frames of sprite
         * @param animationSpeed the animation speed
         * @param hasDirections if sprite has directions in the animation
         * @param isFixed if the sprite is UI
         */
        SpriteComponent(std::string id, int numFrames, int animationSpeed, bool hasDirections, bool isFixed){
            this->isAnimated = true;
            this->numFrames = numFrames;
            this->animationSpeed = animationSpeed;
            this->isFixed = isFixed;

            if(hasDirections){
                Animation downAnimation     = Animation(0, numFrames, animationSpeed);
                Animation rightAnimation    = Animation(1, numFrames, animationSpeed);
                Animation leftAnimation     = Animation(2, numFrames, animationSpeed);
                Animation upAnimation       = Animation(3, numFrames, animationSpeed);

                animations.emplace("DownAnimation", downAnimation);
                animations.emplace("UpAnimation", upAnimation);
                animations.emplace("RightAnimation", rightAnimation);
                animations.emplace("LeftAnimation", leftAnimation);

                this->animationIndex = 0;
                this->currentAnimationName = "RightAnimation";

            }else{

                Animation singleAnimation = Animation(0, numFrames, animationSpeed);
                animations.emplace("SingleAnimation", singleAnimation);
                this->animationIndex = 0;
                this->currentAnimationName = "SingleAnimation";

            }
            Play(this->currentAnimationName);
            SetTexture(id);
        }
        /**
         * @brief this method play the animation of sprite
         * 
         * @param animationName the specific animation name
         */
        void Play(std::string animationName){
            numFrames = animations[animationName].numFrames;
            animationIndex = animations[animationName].index;
            animationSpeed = animations[animationName].animationSpeed;
            currentAnimationName = animationName;  
        }
        /**
         * @brief this method recive a std::string asset id and call the GetTexture
         * 
         * @param assetTextureId the asset texture id is the same id in asset manager
         */
        void SetTexture(std::string assetTextureId){
            texture = Game::assetManager->GetTexture(assetTextureId);
        }
    
        void Initialize() override {
            transform = owner->GetComponent<TransformComponent>();
            sourceRectangle.x = 0;
            sourceRectangle.y = 0;
            sourceRectangle.w = transform->width;
            sourceRectangle.h = transform->height;
        }

        void Update(float deltaTime) override {
            if(isAnimated){
                sourceRectangle.x = sourceRectangle.w * static_cast<int>( (SDL_GetTicks() / animationSpeed) % numFrames );
                //messageInfo(animationSpeed);
            }
            sourceRectangle.y = animationIndex * transform->height;

            destinationRectangle.x = static_cast<int>(transform->position.x) - (isFixed ? 0 : Game::camera.x);
            destinationRectangle.y = static_cast<int>(transform->position.y) - (isFixed ? 0 : Game::camera.y);
            destinationRectangle.w = transform->width * transform->scale;
            destinationRectangle.h = transform->height * transform->scale;
        }

        void Render() override {
            TextureManager::Draw(texture, sourceRectangle, destinationRectangle, spriteFlip);
        }

};   


#endif
