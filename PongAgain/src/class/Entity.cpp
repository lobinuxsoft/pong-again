#include "Entity.h"

void Entity::Draw() {}

Entity::Entity(string name)
{
	this->name = name;
}

const char* Entity::GetName()
{
	return name.c_str();
}
