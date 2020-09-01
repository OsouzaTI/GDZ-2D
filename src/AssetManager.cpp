#include "AssetManager.h"

AssetManager::AssetManager(EntityManager* manager) : manager(manager) {

}

void AssetManager::ClearData(){
    textures.clear();
    fonts.clear();
}

void AssetManager::Addtexture(std::string textureId, const char* filePath){

    textures.emplace(textureId, TextureManager::LoadTexture(filePath));

}


void AssetManager::AddFont(std::string fontId, const char* filepath, int fontSize){
    fonts.emplace(fontId, FontManager::LoadFont(filepath, fontSize));
}

SDL_Texture* AssetManager::GetTexture(std::string textureId){
    return textures[textureId];
}

TTF_Font* AssetManager::GetFont(std::string fontId){
    return fonts[fontId];
}