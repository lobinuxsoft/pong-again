#include "GameManager.h"
#include "class/EntityRectangle.h";
#include "class/EntityCircle.h";
#include "class/Button.h";

enum class GameState
{
	MENU, GAMEPLAY, RESULT
};

enum class GameplayMode
{
	VS,
	CPU
};

enum class Dificulty
{
	EASY, NORMAL, HARD
};

GameState gameState = GameState::MENU;
GameplayMode gameplayMode = GameplayMode::CPU;
Dificulty dificulty = Dificulty::NORMAL;

const float maxBallSpeed = 256.0f;
Vector2 ballSpeed = { maxBallSpeed, maxBallSpeed };

const float maxTimer = 60.0f;
float timer = maxTimer;

float player1Speed = 512.0f;
int player1Score = 0;

float player2Speed = 512.0f;
int player2Score = 0;

int windowWidth = 800;
int windowHeight = 600;

Sound boingSfx;
Sound buttonSfx;
Music backMusic;

EntityRectangle* player1;
EntityRectangle* player2;
EntityCircle* ball;

Button* playButton;
Button* gameModeButton;
Button* dificultyButton;
Button* tutorialButton;

bool pause = false;
bool gameStart = false;
bool showTutorial = false;
int framesCounter = 0;

const int maxBallCounters = 50;
int ballReturnCount = 0;

const char* tutorialMessage = R"(

		---TUTORIAL---
 -Las partidas son rapidas de 60 segundos 
 -Gana quien mas veces logra que la pelota
  toque el lado opuesto de la pantalla 

 -Modo de juego PVP o CPU 
 -Player 1 es el de la izquierda (AZUL) 
	-Controles Player 1: 
		-Subir = W, Bajar = S 

 -Player 2/CPU el de la derecha (ROJO) 
	-Controles Player2: 
		-Subir = Up Arrow, Bajar = Down Arrow 

)";

/// <summary>
/// Me devuelve un valor limitado entre un minimo y un maximo
/// </summary>
/// <param name="d"></param>
/// <param name="min"></param>
/// <param name="max"></param>
/// <returns></returns>
static float clamp(float d, float min, float max) {
	const float t = d < min ? min : d;
	return t > max ? max : t;
}

/// <summary>
/// Se calcula el comportamiento de la IA
/// </summary>
/// <param name="cpuPlayer"></param>
/// <param name="ball"></param>
/// <param name="moveSpeed"></param>
/// <param name="cpuDificulty"></param>
static void cpuPlayerCalculation(EntityRectangle* cpuPlayer, EntityCircle* ball, float moveSpeed, Dificulty cpuDificulty)
{
	// Arreglar IA xD
	float vDistanceToBall = clamp(ball->GetPosition().y - cpuPlayer->GetCenter().y, -1, 1);
	float dificultyMultiplier = 1.0f;

	switch (cpuDificulty)
	{
	case Dificulty::EASY:
		dificultyMultiplier = 0.40f;
		break;
	case Dificulty::NORMAL:
		dificultyMultiplier = 0.42f;
		break;
	case Dificulty::HARD:
		dificultyMultiplier = 0.43f;
		break;
	}

	// CPU calculation
	if (cpuPlayer->GetRectangle().y > 0 && vDistanceToBall < -0.75f)
	{
		cpuPlayer->Move(Vector2{ 0, moveSpeed * (vDistanceToBall * dificultyMultiplier) });
	}
	else if (cpuPlayer->GetRectangle().y + cpuPlayer->GetRectangle().height < GetScreenHeight() && vDistanceToBall > 0.75f)
	{
		cpuPlayer->Move(Vector2{ 0, moveSpeed * (vDistanceToBall * dificultyMultiplier) });
	}
}

/// <summary>
/// Aca se inician los parametros necesarios para el correcto funcionamiento del juego.
/// </summary>
static void init()
{
	InitWindow(windowWidth, windowHeight, "Pong Again");

	InitAudioDevice();

	boingSfx = LoadSound("resources/boing.ogg");
	buttonSfx = LoadSound("resources/confirmation.wav");
	backMusic = LoadMusicStream("resources/alien_lobby.wav");

	PlayMusicStream(backMusic);
	SetMusicVolume(backMusic, 0.15f);

	player1 = new EntityRectangle( "Player1", Vector2{ (GetScreenWidth() * 0.15f) - 10, (GetScreenHeight() * 0.5f) - 50 }, Vector2{ 20, 100 }, RED );
	player2 = new EntityRectangle( "Player2", Vector2{ (GetScreenWidth() * .85f) - 10, (GetScreenHeight() * 0.5f) - 50 }, Vector2{ 20, 100 }, BLUE );
	ball = new EntityCircle( "Ball", Vector2{ GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f }, 10.0f, WHITE );
	playButton = new Button( "PLAY ", Vector2{ GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f }, GetFontDefault(), 30, Vector2{ 50, 5 }, RED, BLUE );
	gameModeButton = new Button( "CPU ", Vector2{ GetScreenWidth() * 0.5f, GetScreenHeight() * 0.6f }, GetFontDefault(), 30, Vector2{ 50, 5 }, RED, BLUE );
	dificultyButton = new Button("Dificulty ", Vector2{ GetScreenWidth() * 0.5f, GetScreenHeight() * 0.7f }, GetFontDefault(), 30, Vector2{ 50, 5 }, RED, BLUE);
	tutorialButton = new Button("Tutorial ", Vector2{ GetScreenWidth() * 0.5f, GetScreenHeight() * 0.8f }, GetFontDefault(), 30, Vector2{ 50, 5 }, RED, BLUE);

	SetTargetFPS(60);
}

/// <summary>
/// Se chequean los inputs de ambos jugadores o el comportamiento de la IA
/// </summary>
static void checkInput()
{
	switch (gameState)
	{
	case GameState::MENU:

		if (!showTutorial)
		{

			if (playButton->IsClick())
			{
				PlaySound(buttonSfx);
				gameState = GameState::GAMEPLAY;
			}

			if (gameModeButton->IsClick()) 
			{
				PlaySound(buttonSfx);
				gameplayMode = gameplayMode == GameplayMode::VS ? GameplayMode::CPU : GameplayMode::VS;
			}

			if (gameplayMode == GameplayMode::CPU)
			{
				if (dificultyButton->IsClick())
				{
					PlaySound(buttonSfx);
					switch (dificulty)
					{
					case Dificulty::EASY:
						dificulty = Dificulty::NORMAL;
						break;
					case Dificulty::NORMAL:
						dificulty = Dificulty::HARD;
						break;
					case Dificulty::HARD:
						dificulty = Dificulty::EASY;
						break;
					}
				}
			}

		}

		if (tutorialButton->IsClick())
		{
			PlaySound(buttonSfx);
			showTutorial = !showTutorial;
		}

		break;
	case GameState::GAMEPLAY:
		// Input
		if (IsKeyPressed(KEY_SPACE)) pause = !pause;

		if (!pause)
		{
			//Game start input
			if (IsKeyDown(KEY_ENTER))
			{
				gameStart = true;
			}

			// Players Inputs-----------------------------------------------------------------------------------
			
			// Player 1
			if (IsKeyDown(KEY_W))
			{
				if (player1->GetRectangle().y > 0)
					player1->Move(Vector2{ 0, -player1Speed });
			}
			else if (IsKeyDown(KEY_S))
			{
				if (player1->GetRectangle().y + player1->GetRectangle().height < GetScreenHeight())
					player1->Move(Vector2{ 0, player1Speed });
			}


			switch (gameplayMode)
			{
			case GameplayMode::VS:

				// Player 2
				if (IsKeyDown(KEY_UP))
				{
					if (player2->GetRectangle().y > 0)
						player2->Move(Vector2{ 0, -player2Speed });
				}
				else if (IsKeyDown(KEY_DOWN))
				{
					if (player2->GetRectangle().y + player2->GetRectangle().height < GetScreenHeight())
						player2->Move(Vector2{ 0, player2Speed });
				}

				break;
			case GameplayMode::CPU:

				cpuPlayerCalculation(player2, ball, player1Speed, dificulty);

				break;
			}
		}
		
		if (IsKeyPressed(KEY_Q))
		{
			pause = false;
			gameState = GameState::MENU;
		}

		break;
	case GameState::RESULT:

		if (IsKeyPressed(KEY_SPACE)) 
		{
			gameState = GameState::MENU;
		}

		break;
	}
}

/// <summary>
/// Se actualizan posiciones, se chequean colisiones.
/// </summary>
static void update()
{
	UpdateMusicStream(backMusic);

	switch (gameState)
	{
	case GameState::MENU:
		gameStart = false;
		pause = false;
		timer = maxTimer;
		player1Score = 0;
		player2Score = 0;
		ballReturnCount = 0;
		framesCounter = 0;

		break;
	case GameState::GAMEPLAY:

		if (!pause)
		{
			// Collisions--------------------------------------------------------------------------------------
			// 
			// Collision with players
			if (CheckCollisionCircleRec(ball->GetPosition(), ball->GetRadius(), player1->GetRectangle()))
			{
				ballReturnCount++;
				ballReturnCount = clamp(ballReturnCount, 0, maxBallCounters);
				ballSpeed.x *= -(1.0f + ballReturnCount * 0.01f);
				ballSpeed.y = clamp(player1->GetCenter().y - ball->GetPosition().y, -1, 1) * -maxBallSpeed;

				// Random pitch
				SetSoundPitch(boingSfx, ((float)ballReturnCount / maxBallCounters) + 1);
				PlaySound(boingSfx);

				ball->SetPosition(Vector2{ ball->GetPosition().x + ball->GetRadius(), ball->GetPosition().y });
			}
			else if(CheckCollisionCircleRec(ball->GetPosition(), ball->GetRadius(), player2->GetRectangle()))
			{
				ballReturnCount++;
				ballReturnCount = clamp(ballReturnCount, 0, maxBallCounters);
				ballSpeed.x *= -(1.0f + ballReturnCount * 0.01f);
				ballSpeed.y = clamp(player2->GetCenter().y - ball->GetPosition().y, -1, 1) * -maxBallSpeed;

				// Random pitch
				SetSoundPitch(boingSfx, ((float)ballReturnCount / maxBallCounters) + 1);
				PlaySound(boingSfx);

				ball->SetPosition(Vector2{ ball->GetPosition().x - ball->GetRadius(), ball->GetPosition().y });
			}

			// Check walls collision for bouncing
			if ((ball->GetPosition().x >= (GetScreenWidth() - ball->GetRadius())))	// Player1 point
			{ 
				ballReturnCount = 0;
				ballSpeed.x = (ballSpeed.x > 0) ? maxBallSpeed : -maxBallSpeed;
				ballSpeed.y = 0;
				player1Score++;	// Adding point to player 1;
				gameStart = false;	// Reset ball state
			}
			else if ((ball->GetPosition().x <= ball->GetRadius()))	// Player2 point
			{
				ballReturnCount = 0;
				ballSpeed.x = (ballSpeed.x > 0) ? maxBallSpeed : -maxBallSpeed;
				ballSpeed.y = 0;
				player2Score++;	// Adding point to player 2;
				gameStart = false;	// Reset ball state
			}

			if ((ball->GetPosition().y >= (GetScreenHeight() - ball->GetRadius())) || (ball->GetPosition().y <= ball->GetRadius())) ballSpeed.y *= -1.0f;

			//-------------------------------------------------------------------------------------------------

			if (gameStart)
			{
				ball->Move(ballSpeed);

				timer -= GetFrameTime();

				if (timer <= 0)
				{
					gameState = GameState::RESULT;
				}
			}
			else
			{
				ball->SetPosition(Vector2{ GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f });
			}

			//------------------------------------------
		}

		framesCounter++;

		break;
	}
}

/// <summary>
/// Se dibujan todos los elementos en pantalla
/// </summary>
static void draw()
{
	BeginDrawing();
	ClearBackground(BLACK);

	switch (gameState)
	{
	case GameState::MENU:

		DrawText("PONG AGAIN...", GetScreenWidth() * 0.5f - MeasureText("PONG AGAIN...", 100) * 0.5f, GetScreenHeight() * 0.15f, 100, ORANGE);

		playButton->Draw();

		switch (gameplayMode)
		{
		case GameplayMode::VS:
			gameModeButton->SetText("PVP ");
			break;
		case GameplayMode::CPU:
			gameModeButton->SetText("CPU ");
			dificultyButton->Draw();

			switch (dificulty)
			{
			case Dificulty::EASY:
				dificultyButton->SetText("CPU: Easy ");
				break;
			case Dificulty::NORMAL:
				dificultyButton->SetText("CPU: Normal ");
				break;
			case Dificulty::HARD:
				dificultyButton->SetText("CPU: Hard ");
				break;
			}
			break;
		}

		gameModeButton->Draw();

		if (showTutorial)
		{
			tutorialButton->SetPosition(Vector2{ GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f });
			tutorialButton->SetMargins(Vector2{ 60, 10 });
			tutorialButton->SetColors(BLACK, WHITE);
			tutorialButton->SetRoundness(0.25f);
			tutorialButton->SetFontSize(20);
			tutorialButton->SetText(tutorialMessage);
		}
		else
		{
			tutorialButton->SetPosition(Vector2{ GetScreenWidth() * 0.5f, GetScreenHeight() * 0.8f });
			tutorialButton->SetMargins(Vector2{ 50,5 });
			tutorialButton->SetColors(RED, BLUE);
			tutorialButton->SetRoundness(1.0f);
			tutorialButton->SetFontSize(30);
			tutorialButton->SetText("Tutorial ");
		}

		tutorialButton->Draw();

		DrawText("Quit game <ESC>", GetScreenWidth() - MeasureText("Quit game <ESC> ", 15), GetScreenHeight() - 20, 15, LIGHTGRAY);
		DrawText("Created by Matias Galarza  <Music by 'Tomas Guida'>", 10, GetScreenHeight() - 20, 15, LIGHTGRAY);

		DrawGrid(10, 1.0f);

		EndMode3D();

		break;
	case GameState::GAMEPLAY:

		// Mid line 
		DrawRectangle(GetScreenWidth() * 0.5f - 5, 0, 10, GetScreenHeight(), DARKGRAY);

		// Inputs draw message
		if (pause)
		{
			if ((framesCounter / 30) % 2) DrawText("PAUSED", GetScreenWidth() * 0.5f - MeasureText("PAUSED", 30)/2, GetScreenHeight() * 0.3f, 30, GRAY);

			DrawText("Press <SPACE> to resume the game...", GetScreenWidth() * 0.5f - MeasureText("Press <SPACE> to resume the game...", 20)/2, GetScreenHeight() * 0.35f, 20, LIGHTGRAY);
		}
		else
		{
			if (!gameStart && (framesCounter / 30) % 2)
			{
				DrawText(
					"PRESS ENTER TO START MATCH",
					GetScreenWidth() * 0.5f - MeasureText("PRESS ENTER TO START MATCH", 20) * 0.5f,
					GetScreenHeight() * 0.4f,
					20,
					GRAY);
			}

			DrawText(FormatText("[%02.0f]", timer), GetScreenWidth() * 0.5f - MeasureText(FormatText("[%02.0f]", timer), 30) * 0.5f, 2, 30, WHITE);

			DrawText("Up = W | Down = S", GetScreenWidth() * 0.01f, GetScreenHeight() - 20 * 2, 20, RED);

			if(gameplayMode == GameplayMode::VS)
			{
				DrawText("Up = Key Up | Down = Key Down", GetScreenWidth() * 0.99f - MeasureText("Up = Key Up | Down = Key Down", 20), GetScreenHeight() - 20 * 2, 20, BLUE);
			}

		}

		DrawText("Puase game <SPACE> // Return main menu <Q> // Quit game <ESC>", GetScreenWidth() * 0.01f, GetScreenHeight() - 10, 10, LIGHTGRAY);

		// Player 1 score
		DrawText(
			TextFormat("%s - %i", player1->GetName(), player1Score),
			GetScreenWidth() * 0.5f - MeasureText(TextFormat("%s - %i", player1->GetName(), player1Score), 30) - 20 * 2,
			GetScreenHeight() * 0.05f,
			30,
			RED
		);

		switch (gameplayMode)
		{
		case GameplayMode::VS:

			// Player 2 score
			DrawText(
				TextFormat("%i - %s", player2Score, player2->GetName()),
				GetScreenWidth() * 0.5f + 20 * 2,
				GetScreenHeight() * 0.05f,
				30,
				BLUE
			);

			break;
		case GameplayMode::CPU:

			// CPU score
			DrawText(
				TextFormat("%i - %s", player2Score, "CPU"),
				GetScreenWidth() * 0.5f + 20 * 2,
				GetScreenHeight() * 0.05f,
				30,
				BLUE
			);

			break;
		default:
			break;
		}

		

		player1->Draw();
		player2->Draw();
		ball->Draw();

		break;
	case GameState::RESULT:

		DrawText("Congratulations!!!", GetScreenWidth() * 0.5f - MeasureText("Congratulations!!!", 30) * 0.5f, GetScreenHeight() * 0.4f, 30,WHITE);

		if(player1Score > player2Score)
		{
			DrawText(FormatText("%s WIN", player1->GetName()), GetScreenWidth() * 0.5f - MeasureText(FormatText("%s WIN", player1->GetName()), 50) * 0.5f, GetScreenHeight() * 0.5f, 50, RED);
		}
		else if (player1Score < player2Score)
		{
			DrawText(FormatText("%s WIN", player2->GetName()), GetScreenWidth() * 0.5f - MeasureText(FormatText("%s WIN", player2->GetName()), 50) * 0.5f, GetScreenHeight() * 0.5f, 50, BLUE);
		}
		else
		{
			if((framesCounter / 30) % 2)
			{
				DrawText("TIED GAME", GetScreenWidth() * 0.5f - MeasureText("TIED GAME", 50) * 0.5f, GetScreenHeight() * 0.5f, 50, BLUE);
			}
			else 
			{
				DrawText("TIED GAME", GetScreenWidth() * 0.5f - MeasureText("TIED GAME", 50) * 0.5f, GetScreenHeight() * 0.5f, 50, RED);
			}

			framesCounter++;
		}

		DrawText("PRESS SPACE to continue", GetScreenWidth() * 0.5f - MeasureText("PRESS SPACE to continue", 20) * 0.5f, GetScreenHeight() - 20 * 4, 20, WHITE);

		break;
	}

	EndDrawing();
}

/// <summary>
/// Elimina o des-inicializa las variables pertinentes, y cierra la ventana de forma correcta.
/// </summary>
static void deInit()
{
	UnloadSound(boingSfx);
	UnloadSound(buttonSfx);
	UnloadMusicStream(backMusic);
	CloseAudioDevice();

	delete player1;
	delete player2;
	delete playButton;
	delete gameModeButton;
	delete dificultyButton;

	CloseWindow();
}

/// <summary>
/// Esta funcion se llama de manera externa, y ejecuta todas las funcionalidades necesarias, implementando interfas.
/// </summary>
void execute()
{
	init();

	while (!WindowShouldClose())
	{
		checkInput();

		update();

		draw();
	}

	deInit();
}
