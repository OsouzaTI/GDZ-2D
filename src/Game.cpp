#include <iostream>
#include <string>
#include "Constants.h"
#include "Game.h"
#include "AssetManager.h"
#include "./Components/SpriteComponent.h"
#include "./Components/KeyboardControlComponent.h"
#include "./Components/ColliderComponent.h"
#include "./Components/TextLabelComponent.h"
#include "./Components/ProjectileEmitterComponent.h"
#include "./Primitives/PCircle.h"
#include "../lib/glm/glm.hpp"
#include "GDZmath.h"
#include "Map.h"

//Verificar o motivo disso ser necessario
class EntityManager;

// Gerenciador de entidades da engine
EntityManager manager;

AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
TransformComponent* Game::playerPosition;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Entity* mainPlayer = NULL;
Map* map;

PCircle pcircle = PCircle();

Game::Game() {
    this->isRunning = false;
}

Game::~Game(){

}

bool Game::IsRunning() const {
    return this->isRunning;
}

void Game::Initialize(int width, int height)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        messageErro("Not initializate SDL");
        return;
    }

    if(TTF_Init() != 0){
        messageInfo("Not initializate SDL_ttf");
        return;
    }

    window = SDL_CreateWindow(
        "Game Engine 2D",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_BORDERLESS
    );

    if(!window){
        messageErro("Not possible create a window");
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        messageErro("Not possible to get a renderer");
        return;
    }

    LoadLevel(1);

    this->isRunning = true;
    return;
}

void Game::LoadLevel(int levelNumber){
    
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);
    std::string levelName = "Level" + std::to_string(levelNumber);
    lua.script_file("assets/scripts/" + levelName + ".lua");

    // Load assets from lua
    sol::table levelData = lua[levelName];
    sol::table levelAssets = levelData["assets"];
    unsigned int assetIndex = 0;
    while(true){
        messageInfo("preso");
        sol::optional<sol::table> existsAssetIndexNode = levelAssets[assetIndex];
        if(existsAssetIndexNode == sol::nullopt){
            break;
        }else{
            sol::table asset = levelAssets[assetIndex];
            std::string assetType = asset["type"];
            if(assetType.compare("texture") == 0){
                std::string assetId = asset["id"];
                std::string assetFile = asset["file"];
                assetManager->Addtexture(assetId, assetFile.c_str());
            }
        }
        assetIndex++;
    }

    //Load map from lua

    sol::table levelMap = levelData["map"];
    std::string mapTextureId = levelMap["textureAssetId"];
    std::string mapFile = levelMap["file"];

    map = new Map(
        mapTextureId,
        static_cast<int>(levelMap["scale"]),
        static_cast<int>(levelMap["tileSize"])
    );

    map->LoadMap(
        mapFile,
        static_cast<int>(levelMap["mapSizeX"]),
        static_cast<int>(levelMap["mapSizeY"])
    );
}

void Game::ProcessInput(){

    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        this->isRunning = false;
        break;
    case SDL_KEYDOWN:
        if(event.key.keysym.sym == SDLK_ESCAPE){
            this->isRunning = false;
        }
        break;
    default:
        break;
    }

}

void Game::Update(){
    // Wait until 16ms has ellapsed since the last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME));


    // Delta time is the diference in
    // ticks from last frame converted to seconds
    float deltaTime = (SDL_GetTicks() - ticksLastFrame)/1000.0f;
    //clmap a delta Time a maximum value
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;
    
    //messageInfo(SDL_GetTicks() - ticksLastFrame);
    /// Sets the new ticks for the current frame to be
    // used in the nest pass
    ticksLastFrame = SDL_GetTicks();
    manager.Update(deltaTime);


    HandleCameraMovement();
    CheckCollisions();

}

void Game::Render(){
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(this->renderer);    

    //TODO 
    //Here we call the manager render to render all
    //Entities
    if(manager.HasNoEntities()){
        return;
    }

    manager.Render();

    pcircle.RenderCirle(renderer);

    SDL_RenderPresent(renderer);

}


void KeepWindow(TransformComponent* playerTF){
    int  MapScale =  map->GetScaleMap();
    int _WINDOW_WIDTH  = WINDOW_WIDTH  * MapScale;
    int _WINDOW_HEIGHT = WINDOW_HEIGHT * MapScale;

    if(playerTF->position.x > _WINDOW_WIDTH - playerTF->width){                
        playerTF->position.x--;
    }

    if(playerTF->position.x < 0){
        playerTF->position.x++;
    }
    
    if(playerTF->position.y > _WINDOW_HEIGHT - playerTF->height){
        playerTF->position.y--;
    }

    if(playerTF->position.y < 0){
        playerTF->position.y++;
    }

}

void Game::HandleCameraMovement(){
    if(mainPlayer){
        TransformComponent* mainPlayerTransform = mainPlayer->GetComponent<TransformComponent>();

        camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH  / 2);
        camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

        camera.x = GDZmath::clamp(camera.x, camera.w, 0);
        camera.y = GDZmath::clamp(camera.y, camera.h, 0);

        //KeepWindow(mainPlayerTransform);
    }
}

void Game::CheckCollisions(){
    CollisionType collisionTagType = manager.CheckCollisions();
    if(collisionTagType == PLAYER_ENEMY_COLLISION){
        //TODO collider enemy
        isRunning = false;
    }
    
    if(collisionTagType == PLAYER_PROJECTILE_COLLISION){
        //TODO collider a projectile enemy
        isRunning = false;
    }
}

void Game::Destroy(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}