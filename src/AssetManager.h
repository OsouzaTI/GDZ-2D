#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>
#include "TextureManager.h"
#include "EntityManager.h"

class AssetManager {

    private:
        EntityManager* manager;
        std::map<std::string, SDL_Texture*> textures;
    public:
        AssetManager(EntityManager* manager);
        ~AssetManager();
        void ClearData();
        void Addtexture(std::string textureId, const char* filepath);
        SDL_Texture* GetTexture(std::string textureId);
};


#endif