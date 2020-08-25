#include "Entity.h"
#include "Constants.h"
Entity::Entity(EntityManager& manager) : manager(manager) {
    this->isActive = true;
}

Entity::Entity(EntityManager& manager, std::string name, LayerType layer) : manager(manager), name(name), layer(layer) {
    this->isActive = true;
}

void Entity::Update(float deltaTime){
    for(auto& component: components){
        component->Update(deltaTime);
    }
    //this->ListAllComponents();
}

void Entity::Render(){
    for(auto& component: components){
        component->Render();
    }
}

void Entity::Destroy(){
    this->isActive = false;
}

bool Entity::IsActive() const {
    return this->isActive;
}

void Entity::ListAllComponents(){
    for(auto& mapElement: componentTypeMap){
        messageInfo("Component -> " << mapElement.first->name());
    }
}

//void ShowComponent(Component* component)