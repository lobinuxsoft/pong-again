#include "EntityCircle.h"

void EntityCircle::Draw()
{
	DrawCircle(this->position.x, this->position.y, radius, color);
}

EntityCircle::EntityCircle(const string name, Vector2 position, float radius, Color color) : EntityObj{ name, position }
{
	this->radius = radius;
	this->color = color;
}

float EntityCircle::GetRadius()
{
	return this->radius;
}
