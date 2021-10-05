#include "Entity.h";

#pragma once
class EntityUI : public Entity
{
protected: 
	Rectangle rectangle;

public:
	EntityUI(string name, Rectangle rectangle);
	void SetPosition(Vector2 position);
	Rectangle GetRectangle();
};

