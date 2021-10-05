#pragma once
#include "EntityObj.h"

class EntityRectangle : public EntityObj
{
private:
	Vector2 bound;
	Color color;

public:
	EntityRectangle(const string name, Vector2 position, Vector2 bound, Color color);
	void Draw() override;
	Rectangle GetRectangle();
	Vector2 GetCenter();
};

