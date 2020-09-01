/*


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
    assetManager->Addtexture("projectile-image",
        std::string("assets/images/bullet-enemy.png").c_str());
    
    assetManager->AddFont("charriot-font",
        std::string("assets/fonts/charriot.ttf").c_str(), 16);

    
    map = new Map("jungle-tiletexture", 2, 32);
    map->LoadMap(std::string("assets/tilemaps/jungle.map"), 25, 20);

    //TODO: add entities 
    Entity& tank(manager.AddEntity("tank", ENEMY_LAYER));
    tank.AddComponent<TransformComponent>(150, 495, 20, 20, 32, 32, 1);
    tank.AddComponent<SpriteComponent>("tank-image");
    tank.AddComponent<ColliderComponent>("ENEMY", 0, 0, 32, 32);

    chopper.AddComponent<TransformComponent>(240, 106, 50, 50, 32, 32, 1, true);
    chopper.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
    chopper.AddComponent<KeyboardControlComponent>("up", "down", "right", "left", "space");
    chopper.AddComponent<ColliderComponent>("PLAYER", 240, 106, 32, 32);
    playerPosition = chopper.GetComponent<TransformComponent>();

    Entity& radar(manager.AddEntity("radar", UI_LAYER));
    radar.AddComponent<TransformComponent>(WINDOW_WIDTH - 64, 0, 0, 0, 64, 64, 1);
    radar.AddComponent<SpriteComponent>("ui-radar", 8, 30, false, true);

    Entity& projectile(manager.AddEntity("projectile", PROJECTILE_LAYER));
    projectile.AddComponent<TransformComponent>(150+16, 495+16, 0, 0, 4, 4, 1);
    projectile.AddComponent<SpriteComponent>("projectile-image");
    projectile.AddComponent<ColliderComponent>("PROJECTILE", 150+16, 495+16, 4, 4);
    projectile.AddComponent<ProjectileEmitterComponent>(50, 270, 200, true);
    
    Entity& labelLevelName(manager.AddEntity("LabelLavel", UI_LAYER));
    labelLevelName.AddComponent<TextLabelComponent>(10, 10, "Level 1", "charriot-font", WHITE_COLOR);

    Entity& labelFPS(manager.AddEntity("LabelFPS", UI_LAYER));
    labelFPS.AddComponent<TextLabelComponent>(10, 25, &_FPS, "charriot-font", GREEN_COLOR);

}

*/