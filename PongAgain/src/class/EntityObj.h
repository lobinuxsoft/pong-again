#pragma once
#include "Entity.h";

class EntityObj : public Entity
{
protected:
	Vector2 position;

public:
	EntityObj(string name, Vector2 position);
	void SetPosition(Vector2 position);
	Vector2 GetPosition();
	void Move(Vector2 direction);
};

