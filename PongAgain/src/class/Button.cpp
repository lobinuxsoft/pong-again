#include "Button.h"


bool Button::IsClick()
{
	return click;
}

void Button::Draw()
{
	float width = rectangle.width + margin.x * 2;
	float height = rectangle.height + margin.y * 2;
	Rectangle temp = Rectangle{ rectangle.x - width / 2, rectangle.y - height / 2, width, height };

	if (CheckCollisionPointRec(GetMousePosition(), temp))
	{
		click = IsMouseButtonPressed(0);

		if (IsMouseButtonDown(0))
		{
			//Boton presionado
			DrawRectangleRounded(temp, roundness, segments, primaryColor);
			DrawRectangleRoundedLines(temp, roundness, segments, lineThick, secondaryColor);
			DrawTextRec(font, name.c_str(), Rectangle{ temp.x + margin.x, temp.y + margin.y, rectangle.width, rectangle.height }, fontSize, fontSpacing, false, secondaryColor);
		}
		else
		{
			//Sombre el boton
			DrawRectangleRounded(temp, roundness, segments, primaryColor);
			DrawRectangleRoundedLines(temp, roundness, segments, lineThick, secondaryColor);
			DrawTextRec(font, name.c_str(), Rectangle{ temp.x + margin.x, temp.y + margin.y, rectangle.width, rectangle.height }, fontSize, fontSpacing, false, secondaryColor);
		}
		
	}
	else
	{
		// Fuera del boton
		DrawRectangleRounded(temp, roundness, segments, secondaryColor);
		DrawRectangleRoundedLines(temp, roundness, segments, lineThick, primaryColor);
		DrawTextRec(font, name.c_str(), Rectangle{ temp.x + margin.x, temp.y + margin.y, rectangle.width, rectangle.height }, fontSize, fontSpacing, false, primaryColor);
		click = false;
	}
}

void Button::SetFontSize(float size)
{
	this->fontSize = size;
}

void Button::SetMargins(Vector2 margins)
{
	this->margin = margins;
}

void Button::SetRoundness(float roundness)
{
	this->roundness = roundness;
}

void Button::SetColors(Color primary, Color secondary)
{
	this->primaryColor = primary;
	this->secondaryColor = secondary;
}

void Button::SetText(string text)
{
	this->name = text;
	Vector2 temp = MeasureTextEx(font, text.c_str(), fontSize, fontSpacing);
	this->rectangle = Rectangle{ rectangle.x, rectangle.y, temp.x, temp.y };
}

Button::Button(string text, Vector2 position, Font font, int fontSize, Vector2 margin, Color primary, Color secondary) : EntityUI{ text, Rectangle{ 0, 0, 0, 0 } }
{
	this->font = font;
	this->fontSize = fontSize;
	this->margin = margin;
	this->primaryColor = primary;
	this->secondaryColor = secondary;

	Vector2 temp = MeasureTextEx(font, text.c_str(), fontSize, fontSpacing);
	this->rectangle = Rectangle{ position.x, position.y, temp.x, temp.y };
}
