#pragma once


#include "ECS.h"
#include "Components.h"
#include "../Game.h"

class KeyboardController: public Component
{
public:
	TransformComponent *transform;
	SpriteComponent* sprite;
	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite=&entity->getComponent<SpriteComponent>();
	}
	void update() override
	{
		if (Game::event.type == SDL_KEYDOWN)
		{
			sprite->Play("Walk");
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = -1;
				break;
			case SDLK_a:
				transform->velocity.x = -1;
				sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_s:
				transform->velocity.y = 1;
				break;
			case SDLK_d:
				transform->velocity.x = 1;
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				break;
			default:
				break;
			}
		}
		if (Game::event.type == SDL_KEYUP)
		{
			sprite->Play("Idle");
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = 0;
				break;
			case SDLK_a:
				transform->velocity.x = 0;
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				break;
			default:
				break;
			}
		}
	}
};

