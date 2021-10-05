#pragma once
#include "EntityObj.h"

class EntityCircle : public EntityObj
{
private:
	float radius;
	Color color;

public:
	EntityCircle(const string name, Vector2 position, float radius, Color color);
	void Draw() override;
	float GetRadius();
};

