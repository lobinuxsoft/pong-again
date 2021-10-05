#include "EntityObj.h"

EntityObj::EntityObj(string name, Vector2 position) : Entity{ name }
{
	this->position = position;
}

void EntityObj::SetPosition(Vector2 position)
{
	this->position = position;
}

Vector2 EntityObj::GetPosition()
{
	return this->position;
}

void EntityObj::Move(Vector2 direction)
{
	this->position.x += direction.x * GetFrameTime();
	this->position.y += direction.y * GetFrameTime();
}
