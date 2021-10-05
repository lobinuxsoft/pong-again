#include "EntityUI.h";

#pragma once
class Button : public EntityUI
{
private:
	const int segments = 16;
	const float fontSpacing = 1.0f;
	const int lineThick = 5;
	float roundness = 1.0f;
	Font font;
	int fontSize;
	Vector2 margin;
	Color primaryColor;
	Color secondaryColor;
	bool click = false;

public:
	Button(string text, Vector2 position, Font font, int fontSize, Vector2 margin = Vector2{ 0,0 }, Color primary = RED, Color secondary = BLUE);
	void Draw() override;
	void SetFontSize(float size);
	void SetMargins(Vector2 margins);
	void SetRoundness(float roundness);
	void SetColors(Color primary, Color secondary);
	void SetText(string text);
	bool IsClick();
};

