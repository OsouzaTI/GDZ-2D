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

    //Load entities
    sol::table levelEntities = levelData["entities"];
    unsigned int entityIndex = 0;
    while(true){
        sol::optional<sol::table> existsEntityIndexNode = levelEntities[entityIndex];
        if(existsEntityIndexNode == sol::nullopt){
            break;
        }else{
            sol::table entity = levelEntities[entityIndex];
            std::string entityName = entity["name"];
            LayerType entityLayerType = static_cast<LayerType>(static_cast<int>(entity["layer"]));

            // Add new entity
            auto& newEntity(manager.AddEntity(entityName, entityLayerType));

            // Add transform component
            sol::optional<sol::table> existsTransformComponent = entity[LUA_COMPONENTS][LUA_TRANSFORM];
            if(existsTransformComponent != sol::nullopt){
                sol::optional<bool> existsBoolIsPlayer = entity[LUA_COMPONENTS][LUA_TRANSFORM]["isPlayer"];
                if(existsBoolIsPlayer != sol::nullopt){
                    newEntity.AddComponent<TransformComponent>(
                        static_cast<int>(entity[LUA_COMPONENTS][LUA_TRANSFORM]["position"]["x"]),
                        static_cast<int>(entity[LUA_COMPONENTS][LUA_TRANSFORM]["position"]["y"]),
                        static_cast<int>(entity[LUA_COMPONENTS][LUA_TRANSFORM]["velocity"]["x"]),
                        static_cast<int>(entity[LUA_COMPONENTS][LUA_TRANSFORM]["velocity"]["y"]),
                        static_cast<int>(entity[LUA_COMPONENTS][LUA_TRANSFORM]["width"]),
                        static_cast<int>(entity[LUA_COMPONENTS][LUA_TRANSFORM]["height"]),
                        static_cast<int>(entity[LUA_COMPONENTS][LUA_TRANSFORM]["scale"]),
                        true
                    );
                }else{
                    newEntity.AddComponent<TransformComponent>(
                        static_cast<int>(entity[LUA_COMPONENTS][LUA_TRANSFORM]["position"]["x"]),
                        static_cast<int>(entity[LUA_COMPONENTS][LUA_TRANSFORM]["position"]["y"]),
                        static_cast<int>(entity[LUA_COMPONENTS][LUA_TRANSFORM]["velocity"]["x"]),
                        static_cast<int>(entity[LUA_COMPONENTS][LUA_TRANSFORM]["velocity"]["y"]),
                        static_cast<int>(entity[LUA_COMPONENTS][LUA_TRANSFORM]["width"]),
                        static_cast<int>(entity[LUA_COMPONENTS][LUA_TRANSFORM]["height"]),
                        static_cast<int>(entity[LUA_COMPONENTS][LUA_TRANSFORM]["scale"])                        
                    );
                }
            }

            // Add sprite component
            sol::optional<sol::table> existsSpriteComponent = entity[LUA_COMPONENTS][LUA_SPRITE];
            if(existsSpriteComponent != sol::nullopt){
                std::string textureId = entity[LUA_COMPONENTS][LUA_SPRITE]["textureAssetId"];
                bool isAnimated = entity[LUA_COMPONENTS][LUA_SPRITE]["animated"];
                if(isAnimated){
                    newEntity.AddComponent<SpriteComponent>(
                        textureId,
                        static_cast<int>(entity[LUA_COMPONENTS][LUA_SPRITE]["frameCount"]),
                        static_cast<int>(entity[LUA_COMPONENTS][LUA_SPRITE]["animationSpeed"]),
                        static_cast<bool>(entity[LUA_COMPONENTS][LUA_SPRITE]["hasDirections"]),
                        static_cast<bool>(entity[LUA_COMPONENTS][LUA_SPRITE]["fixed"])
                    );
                }else{
                    newEntity.AddComponent<SpriteComponent>(textureId, false);
                }
            }

            // Add input control
            sol::optional<sol::table> existsInputComponent = entity[LUA_COMPONENTS][LUA_INPUT];
            if(existsInputComponent != sol::nullopt){
                sol::optional<sol::table> existsKeyboardInputComponent = entity[LUA_COMPONENTS][LUA_INPUT]["keyboard"];
                if(existsKeyboardInputComponent != sol::nullopt){
                    std::string upKey = entity[LUA_COMPONENTS][LUA_INPUT]["keyboard"]["up"];
                    std::string downKey = entity[LUA_COMPONENTS][LUA_INPUT]["keyboard"]["down"];
                    std::string rightKey = entity[LUA_COMPONENTS][LUA_INPUT]["keyboard"]["right"];
                    std::string leftKey = entity[LUA_COMPONENTS][LUA_INPUT]["keyboard"]["left"];
                    std::string shootKey = entity[LUA_COMPONENTS][LUA_INPUT]["keyboard"]["shoot"];
                    newEntity.AddComponent<KeyboardControlComponent>(upKey, downKey, rightKey, leftKey, shootKey);
                }
            }

            //Add collider component
            sol::optional<sol::table> existsColliderComponent = entity[LUA_COMPONENTS]["collider"];
            if(existsColliderComponent != sol::nullopt){
                std::string colliderTag = entity[LUA_COMPONENTS]["collider"]["tag"];
                newEntity.AddComponent<ColliderComponent>(
                    colliderTag,
                    static_cast<int>(entity[LUA_COMPONENTS][LUA_TRANSFORM]["position"]["x"]),
                    static_cast<int>(entity[LUA_COMPONENTS][LUA_TRANSFORM]["position"]["y"]),
                    static_cast<int>(entity[LUA_COMPONENTS][LUA_TRANSFORM]["width"]),
                    static_cast<int>(entity[LUA_COMPONENTS][LUA_TRANSFORM]["height"])
                );
            }

            // Add projectile emitter component
            sol::optional<sol::table> existsProjectileEmitterComponent = entity[LUA_COMPONENTS][LUA_PROJECTILE_EMITTER];
            if(existsProjectileEmitterComponent != sol::nullopt){
                int parentEntityXPos = entity[LUA_COMPONENTS][LUA_TRANSFORM]["position"]["x"];
                int parentEntityYPos = entity[LUA_COMPONENTS][LUA_TRANSFORM]["position"]["y"];
                int parentEntityWidth = entity[LUA_COMPONENTS][LUA_TRANSFORM]["width"];
                int parentEntityHeight = entity[LUA_COMPONENTS][LUA_TRANSFORM]["height"];
                int projectileWidth  = entity[LUA_COMPONENTS][LUA_PROJECTILE_EMITTER]["width"];
                int projectileHeight = entity[LUA_COMPONENTS][LUA_PROJECTILE_EMITTER]["height"];
                int projectileSpeed = entity[LUA_COMPONENTS][LUA_PROJECTILE_EMITTER]["speed"];
                int projectileRange = entity[LUA_COMPONENTS][LUA_PROJECTILE_EMITTER]["range"];
                int projectileAngle = entity[LUA_COMPONENTS][LUA_PROJECTILE_EMITTER]["angle"];
                bool projectileShouldLoop = entity[LUA_COMPONENTS][LUA_PROJECTILE_EMITTER]["shouldLoop"];
                std::string textureAssetId = entity[LUA_COMPONENTS][LUA_PROJECTILE_EMITTER]["textureAssetId"];

                Entity& projectile(manager.AddEntity("projectile", PROJECTILE_LAYER));
                projectile.AddComponent<TransformComponent>(
                    parentEntityXPos + (parentEntityWidth / 2),
                    parentEntityYPos + (parentEntityHeight / 2),
                    0,
                    0,
                    projectileWidth,
                    projectileHeight,
                    1
                );
                
                projectile.AddComponent<SpriteComponent>(textureAssetId);
                
                projectile.AddComponent<ProjectileEmitterComponent>(
                    projectileSpeed,
                    projectileAngle,
                    projectileRange,
                    projectileShouldLoop
                );

                projectile.AddComponent<ColliderComponent>(
                    "PROJECTILE",
                    parentEntityXPos,
                    parentEntityYPos,
                    projectileWidth,
                    projectileHeight
                );

            }

        }
        entityIndex++;
    }
    mainPlayer = manager.GetEntityByName("player");
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
        playerTF->velocity.x = 0;
        playerTF->position.x--;
    }

    if(playerTF->position.x < 0){
        playerTF->velocity.x = 0;
         playerTF->position.x++;
    }
    
    if(playerTF->position.y > _WINDOW_HEIGHT - playerTF->height){
        playerTF->velocity.y = 0;
         playerTF->position.y--;
    }

    if(playerTF->position.y < 0){
        playerTF->velocity.y = 0;
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

        KeepWindow(mainPlayerTransform);
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