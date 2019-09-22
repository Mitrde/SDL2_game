#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Collision.h"
//#include "Vector2D.h"



Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

std::vector<ColliderComponent*> Game::colliders;

using namespace std;


auto& player(manager.addEntity());
auto& wall(manager.addEntity());



enum groupLabels : std::size_t
{
	groupMap,
	groupPlayers,
	groupEnemies,
	groupColliders
};



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
	
	map = new Map();


	//ecs implementation
	Map::LoadMap("assets/p16x16.txt", 16, 16);

	player.addComponent<TransformComponent>();
	player.addComponent<SpriteComponent>("assets/sand_storm.png");
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

	wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 20, 1);
	wall.addComponent<SpriteComponent>("assets/dirt.png");
	wall.addComponent<ColliderComponent>("wall");
	wall.addGroup(groupMap);

	
}

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
	manager.refresh();
	manager.update();
	for (auto cc : colliders)
	{
		Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
	}
}

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));

void Game::render()
{
	SDL_RenderClear(renderer);
	//this is where we would add stuff to renderer
	

	manager.draw();
	for (auto& t : tiles)
	{
		t->draw();
	}
	for (auto& p : players)
	{
		p->draw();
	}
	for (auto& e : enemies)
	{
		e->draw();
	}

	SDL_RenderPresent(renderer);
}

void Game::AddTile(int id, int x, int y)
{
	auto &tile(manager.addEntity());
	tile.addComponent<TileComponent>(x, y, 32, 32, id);
	tile.addGroup(groupMap);
}

