#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../lib/lua/sol.hpp"
#include "Entity.h"
#include "Component.h"
#include "EntityManager.h"
#include "Components/TransformComponent.h"


class AssetManager;

class Game {
	private:
		bool isRunning;
		SDL_Window *window;
		//SDL_Renderer *renderer;
	public:
		Game();
		~Game();
		int ticksLastFrame;
		static TransformComponent* playerPosition;
		static SDL_Renderer* renderer;
		static AssetManager* assetManager;
		static SDL_Event event;
		static SDL_Rect camera;
		void LoadLevel(int levelNumber);
		bool IsRunning() const;
		void Initialize(int width, int height);
		void ProcessInput();
		void Update();
		void Render();
		void Destroy();
		void HandleCameraMovement();
		void CheckCollisions();
};

#endif