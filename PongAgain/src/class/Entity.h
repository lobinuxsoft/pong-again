#include <raylib.h>
#include <string>
using namespace std;

#pragma once
class Entity
{
protected:
	string name;

public:
	Entity(string name);
	const char* GetName();
	virtual void Draw();
};

