#pragma once

#include "Components.h"
#include "../Vector2D.h"

class TransformComponent :public Component
{
public:
	Vector2D position;
	Vector2D velocity;
	int speed = 3;
	int height = 32;
	int width = 32;
	int scale = 1;
	
	bool blocked = false;

	TransformComponent()
	{
		position.Zero();
	}
	TransformComponent(int sc)
	{
		position.Zero();
		scale = sc;
	}
	TransformComponent(float x, float y)
	{
		position.x = x;
		position.y = y;
	}
	TransformComponent(float x, float y, int h, int w, int sc)
	{
		position.x = x;
		position.y = y;
		width = w;
		height = h;
		scale = sc;
	}

	void init() override
	{
		
		velocity.Zero();
	}

	void update() override
	{
		if (position.x + velocity.x*speed > 10 && position.x + velocity.x*speed <730)
			position.x+= static_cast<int>(velocity.x*speed);
		if (position.y + velocity.y*speed > 10 && position.y+ velocity.y*speed <530)
			position.y+= static_cast<int>(velocity.y*speed);
	}
	
};