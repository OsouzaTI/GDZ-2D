#include "EntityManager.h"
#include "Constants.h"
#include "Collision.h"
#include "./Components/ColliderComponent.h"

void EntityManager::ClearData(){
    for(auto& entity: entities){
        entity->Destroy();
    }
}

bool EntityManager::HasNoEntities(){
    return entities.size() == 0;
}

void EntityManager::Update(float deltaTime){
    for(auto& entity: entities){
        entity->Update(deltaTime);
        //this->ShowEntity(entity);
    }
    ///this->ListAllEntities();
}

void EntityManager::Render(){

    for(int layerNumber = 0; layerNumber < NUM_LAYERS; layerNumber++){
        for(auto& entity: GetEntitiesByLayer(static_cast<LayerType>(layerNumber)))
        {
            entity->Render();
        }
    }

    // for(auto& entity: entities){
    //     entity->Render();
    // }
}

Entity& EntityManager::AddEntity(std::string entityName, LayerType layer){
    Entity *entity = new Entity(*this, entityName, layer);
    entities.emplace_back(entity);
    return *entity;
}

std::vector<Entity*> EntityManager::GetEntities() const {
    return entities;
}

std::vector<Entity*> EntityManager::GetEntitiesByLayer(LayerType layer) const {
    std::vector<Entity*> selectedEntities;
    for(auto& entity: entities){
        if(entity->layer == layer){
            selectedEntities.emplace_back(entity);
        }
    }
    return selectedEntities;
}

unsigned int EntityManager::GetEntityCount(){
    return entities.size();
}

void EntityManager::ListAllEntities(){
    unsigned int entityCount = 0;
    for(auto& entity: entities){
        messageInfo("Entity ["  << entityCount << "]: " << entity->name);
        entityCount++;
    }

}

CollisionType EntityManager::CheckCollisions() const {
    for(auto& thisEntity: entities){
        if(thisEntity->HasComponent<ColliderComponent>()){
            ColliderComponent* thisCollider = thisEntity->GetComponent<ColliderComponent>();
            for(auto& thatEntity: entities){
                if(thisEntity->name.compare(thatEntity->name)!=0 && thatEntity->HasComponent<ColliderComponent>()){
                    ColliderComponent* thatCollider = thatEntity->GetComponent<ColliderComponent>();
                    if(Collision::CheckRectangleCollision(thisCollider->collider, thatCollider->collider)){
                        if(
                            thisCollider->colliderTag.compare("PLAYER") == 0 &&
                            thatCollider->colliderTag.compare("ENEMY") == 0
                        )    {
                            return PLAYER_ENEMY_COLLISION;
                        }

                        if(
                            thisCollider->colliderTag.compare("PLAYER") == 0 &&
                            thatCollider->colliderTag.compare("PROJECTILE") == 0
                        ){
                            return PLAYER_PROJECTILE_COLLISION;
                        }

                        if(
                            thisCollider->colliderTag.compare("ENEMY") == 0 &&
                            thatCollider->colliderTag.compare("FRIENDLY_PROJECTILE") == 0
                        ){
                            return ENEMY_PROJECTILE_COLLISION;
                        }

                        if(
                            thisCollider->colliderTag.compare("PLAYER") == 0 &&
                            thatCollider->colliderTag.compare("LEVEL_COMPLETE") == 0
                        ){
                            return PLAYER_LEVEL_COMPLETE_COLLISION;
                        }

                    }  
                    return NO_COLLISION;  
                }
            }
        }
    }
}

void EntityManager::ShowEntity(Entity* entity){
    messageInfo("Entity Name -> " << entity->name);
}
