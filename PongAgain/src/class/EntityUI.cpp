#include "EntityUI.h"

EntityUI::EntityUI(string name, Rectangle rectangle) : Entity{ name }
{
	this->rectangle = rectangle;
}

void EntityUI::SetPosition(Vector2 position)
{
	this->rectangle.x = position.x;
	this->rectangle.y = position.y;
}

Rectangle EntityUI::GetRectangle()
{
	return this->rectangle;
}
