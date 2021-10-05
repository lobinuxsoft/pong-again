#include "EntityRectangle.h"

EntityRectangle::EntityRectangle(const string name, Vector2 position, Vector2 bound, Color color) : EntityObj{ name, position }
{
	this->bound = bound;
	this->color = color;
}

void EntityRectangle::Draw()
{
	DrawRectangle(this->position.x, this->position.y, bound.x, bound.y, color);
}

Rectangle EntityRectangle::GetRectangle()
{
	return Rectangle { this->position.x, this->position.y, this->bound.x, this->bound.y };
}

Vector2 EntityRectangle::GetCenter()
{
	return Vector2{ position.x + bound.x * 0.5f, position.y + bound.y * 0.5f };
}
