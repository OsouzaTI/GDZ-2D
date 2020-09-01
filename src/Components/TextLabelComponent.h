#ifndef TEXTLABELCOMPONENT_H
#define TEXTLABELCOMPONENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../FontManager.h"
#include "../EntityManager.h"
#include "../AssetManager.h"
#include "../Game.h"


class TextLabelComponent: public Component{

    private:
        SDL_Rect position;
        std::string text;
        std::string fontFamily;
        std::string* updateText;
        SDL_Color color;
        SDL_Texture* texture;
        bool update = false;
    public:

        TextLabelComponent(int x, int y, std::string text, std::string fontFamily, const SDL_Color& color){
            this->position.x = x;
            this->position.y = y;
            this->text = text;
            this->fontFamily = fontFamily;
            this->color = color;
            SetLabelText(text, fontFamily);
        }

        TextLabelComponent(int x, int y, std::string *text, std::string fontFamily, const SDL_Color& color){
            this->position.x = x;
            this->position.y = y;
            this->updateText = text;
            this->fontFamily = fontFamily;
            this->color = color;
            this->update = true;
            SetLabelUpdateText(text, fontFamily);
        }

        void SetLabelText(std::string text, std::string fontFamily){
            SDL_Surface* surface = TTF_RenderText_Blended(Game::assetManager->GetFont(fontFamily), text.c_str(), color);
            texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
            SDL_FreeSurface(surface);
            SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);
        }

        void SetLabelUpdateText(std::string *text, std::string fontFamily){
            SDL_Surface* surface = TTF_RenderText_Blended(Game::assetManager->GetFont(fontFamily), text->c_str(), color);
            texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
            SDL_FreeSurface(surface);
            SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);
        }

        void Render() override {
            FontManager::Draw(texture, position);
        }

        void Update(float deltaTime) override {
            if(this->update){                
                this->SetLabelUpdateText(this->updateText, this->fontFamily);
            }
        }

};

#endif
