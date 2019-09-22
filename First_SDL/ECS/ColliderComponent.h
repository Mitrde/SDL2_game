#pragma once

#include<string>
#include "SDL.h"	
#include "Components.h"

class ColliderComponent : public Component
{
public:
	SDL_Rect collider;
	std::string tag;

	TransformComponent* transform;
	ColliderComponent() = default;

	ColliderComponent(std::string tag)
	{
		this->tag = tag;
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
			entity->addComponent<TransformComponent>();
		transform = &entity->getComponent<TransformComponent>();

		Game::colliders.push_back(this);
	}

	void update() override
	{
		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);
		collider.w = static_cast<int>(transform->width*transform->scale);
		collider.h = static_cast<int>(transform->height*transform->scale);
	}
};