#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Collision.h"
#include "Vector2D.h"



Map* myMap;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
SDL_Rect Game::camera{ 0,0,800,640};



bool Game::isRunning = false;

using namespace std;




auto& player(manager.addEntity());


Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if(fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		cout << "Subsystem Initialised!..." << endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
		{			
			cout << "Window created" << endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);

		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			cout << "renderer created!.." << endl;
		}
		isRunning = true;
	}
	else
	{
		isRunning = false;
	}
	
	myMap = new Map("assets/terrain_ss.png", 2, 32);	


	//ecs implementation
	myMap->LoadMap("assets/map.map", 25, 20);

	player.addComponent<TransformComponent>(360,280,64,64,2);
	player.addComponent<SpriteComponent>("assets/sand_storm_anim.png",true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);	
}


auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));


void Game::handleEvents()
{
	
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;

	default:
		break;
	}
}

bool Game::running() { return isRunning; }

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	cout << "Game Cleaned!" << endl;
}


void Game::update()

{
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	manager.refresh();
	manager.update();

	for (auto& c : colliders)
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol))
		{
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	camera.x += player.getComponent<TransformComponent>().velocity.x*player.getComponent<TransformComponent>().speed;
	camera.y += player.getComponent<TransformComponent>().velocity.y*player.getComponent<TransformComponent>().speed;

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;
}



void Game::render()
{
	SDL_RenderClear(renderer);
	//this is where we would add stuff to renderer
	

	manager.draw();
	for (auto& t : tiles)
	{
		t->draw();
	}

	for (auto c : colliders)
	{
		c->draw();
	}
	for (auto& p : players)
	{
		p->draw();
	}
	

	SDL_RenderPresent(renderer);
}



