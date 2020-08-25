#include <iostream>
#include "Constants.h"
#include "Game.h"
#include "AssetManager.h"
#include "Components/TransformComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/KeyboardControlComponent.h"
#include "./Components/ColliderComponent.h"
#include "../lib/glm/glm.hpp"
#include "GDZmath.h"
#include "Map.h"

class EntityManager;

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Map* map;

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

    LoadLevel(0);

    this->isRunning = true;
    return;
}

Entity& chopper(manager.AddEntity("chopper", PLAYER_LAYER));

void Game::LoadLevel(int levelNumber){
    // New Assets
    assetManager->Addtexture("tank-image",
        std::string("assets/images/tank-big-right.png").c_str());
    assetManager->Addtexture("chopper-image",
        std::string("assets/images/chopper-spritesheet.png").c_str());
    assetManager->Addtexture("ui-radar",
        std::string("assets/images/radar.png").c_str());
    assetManager->Addtexture("jungle-tiletexture",
        std::string("assets/tilemaps/jungle.png").c_str());
    assetManager->Addtexture("collider",
        std::string("assets/images/collision-texture.png").c_str());

    
    map = new Map("jungle-tiletexture", 2, 32);
    map->LoadMap(std::string("assets/tilemaps/jungle.map"), 25, 20);

    //TODO: add entities 
    Entity& tank(manager.AddEntity("tank", ENEMY_LAYER));
    tank.AddComponent<TransformComponent>(0, 0, 20, 20, 32, 32, 1);
    tank.AddComponent<SpriteComponent>("tank-image");
    tank.AddComponent<ColliderComponent>("ENEMY", 0, 0, 32, 32);

    chopper.AddComponent<TransformComponent>(240, 106, 50, 50, 32, 32, 1, true);
    chopper.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
    chopper.AddComponent<KeyboardControlComponent>("up", "down", "right", "left", "space");
    chopper.AddComponent<ColliderComponent>("PLAYER", 240, 106, 32, 32);

    Entity& radar(manager.AddEntity("radar", UI_LAYER));
    radar.AddComponent<TransformComponent>(WINDOW_WIDTH - 64, 0, 0, 0, 64, 64, 1);
    radar.AddComponent<SpriteComponent>("ui-radar", 8, 30, false, true);

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
    TransformComponent* mainPlayerTransform = chopper.GetComponent<TransformComponent>();

    camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH  / 2);
    camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

    camera.x = GDZmath::clamp(camera.x, camera.w, 0);
    camera.y = GDZmath::clamp(camera.y, camera.h, 0);

    KeepWindow(mainPlayerTransform);
}

void Game::CheckCollisions(){
    CollisionType collisionTagType = manager.CheckCollisions();
    if(collisionTagType == PLAYER_ENEMY_COLLISION){
        //TODO collider enemy
        isRunning = false;
    }
}

void Game::Destroy(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}