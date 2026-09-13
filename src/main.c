/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include <stdlib.h>

#include <math.h>

#include "raymath.h"


// ==================== COLOUR PALETTE ====================

Color backgroundColor = { 220, 229, 209, 255 };


// ==================== VARIABLES ====================


const int windowWidth = 1290;
const int windowHeight = 720;


// ==================== PLAYER ====================


typedef struct
{
	Vector2 position;
	Vector2 size;
	Vector2 velocity;

	float speed;

	Rectangle hitbox;

	Color colour;

} Player;

Player player;



// ==================== SHEEP ====================

typedef enum
{
	SHEEP_CALM,
	SHEEP_FLEEING

} SheepState;



typedef struct
{
	Vector2 position;
	Vector2 size;
	Vector2 velocity;

	float speed;

	Rectangle hitbox;

	float Flee_radius;

	SheepState currentState;

	Color colour;

} Sheep;

Sheep sheep;



// ==================== SHEEP STATE MAHCINE ====================

void UpdateSheep(Sheep* s, Player* p, float dt)
{ 
	if (!s || !p)
	{
		return;
	}

	// Distance from sheep to player
	Vector2 difference = Vector2Subtract(s->position, p->position);
	float distance = Vector2Length(difference);

	// Switch states depending on how close the player is
	if (distance < s->Flee_radius)
	{
		s->currentState = SHEEP_FLEEING;
	}
	else
	{
		s->currentState = SHEEP_CALM;
	}


	switch (s->currentState)
	{
		case SHEEP_CALM:
		{
			s->colour = LIGHTGRAY;

			// wander code

			break;
		}

		case SHEEP_FLEEING:
		{
			s->colour = RED;

			//flee code

			break;
		}
	}

}

// ==================== INITIALISATION ====================

void InitialisePlayer(Player* p)
{
	if (!p)
	{
		return;
	}

	p->position = (Vector2){ 600.0f, 350.0f };
	p->size = (Vector2){ 40.0f, 40.0f };
	p->velocity = (Vector2){ 0.0f, 0.0f };

	p->speed = 250.0f;

	p->hitbox = (Rectangle)
	{
		p->position.x,
		p->position.y,
		p->size.x,
		p->size.y
	};

	p->colour = BROWN;
}



void InitialiseSheep(Sheep* s)
{
	if (!s)
	{
		return;
	}

	s->position = (Vector2){ 850.0f, 350.0f };
	s->size = (Vector2){ 40.0f, 40.0f };
	s->velocity = (Vector2){ 0.0f, 0.0f };

	s->speed = 180.0f;

	s->Flee_radius = 150;

	s->currentState = SHEEP_CALM;

	s->hitbox = (Rectangle)
	{
		s->position.x,
		s->position.y,
		s->size.x,
		s->size.y
	};

	s->colour = LIGHTGRAY;

}


void InitialiseGame()
{
	InitialisePlayer(&player);
	InitialiseSheep(&sheep);
}


// ==================== PLAYER INPUT ====================

void HandleInput(Player* p, float dt)
{
	if (!p)
	{
		return;
	}

	Vector2 force = { 0.0f, 0.0f };

	Vector2 oldPos = p->position;

	// Vertical movement
	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
	{
		force.y -= 1.0f;
	}

	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
	{
		force.y += 1.0f;
	}

	// Horizontal movement
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
	{
		force.x -= 1.0f;
	}

	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
	{
		force.x += 1.0f;
	}

	// Normalise movement so diagonal movement is not faster
	if (Vector2Length(force) > 0.0f)
	{
		force = Vector2Normalize(force);
	}

	p->velocity = Vector2Scale(force, p->speed);

	// Apply velocity to position using delta time
	p->position.x += p->velocity.x * dt;
	p->position.y += p->velocity.y * dt;

	// Keep hitbox attached to player
	p->hitbox.x = p->position.x;
	p->hitbox.y = p->position.y;

	// Player / sheep collision
	if (CheckCollisionRecs(p->hitbox, sheep.hitbox))
	{
		p->position = oldPos;

		p->hitbox.x = oldPos.x;
		p->hitbox.y = oldPos.y;
	}
}

// ==================== DRAWING ====================

void DrawSheep(Sheep* s)
{
	if (!s)
	{
		return;
	}

	DrawRectangleV(
		s->position,
		s->size,
		s->colour
	);
}



void DrawPlayer(Player* p)
{
	if (!p)
	{
		return;
	}

	DrawRectangleV(
		p->position,
		p->size,
		p->colour
	);
}


void DrawGame()
{
	BeginDrawing();

	ClearBackground(backgroundColor);

	DrawPlayer(&player);

	DrawSheep(&sheep);

	// Debug hitboxes
	DrawRectangleLinesEx(player.hitbox, 2.0f, RED);
	DrawRectangleLinesEx(sheep.hitbox, 2.0f, BLUE);

	DrawText("Sheep Herding AI", 20, 20, 20, DARKGRAY);

	EndDrawing();
}







// ==================== MAIN ====================

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");
	
	// Create the window and set FPS
	InitWindow(windowWidth, windowHeight, "AI from scratch");
	
	//remove unneeded info in the console
	SetTraceLogLevel(LOG_WARNING);

	SetTargetFPS(60);
	
	InitialiseGame();


	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		//Make sure the frames run at the same speed for every computer
		float dt = GetFrameTime();


		HandleInput(&player, dt);

		UpdateSheep(&sheep, &player, dt);

		DrawGame();


	}
	// Release loaded resources and close the window.
	
		CloseWindow();
		return 0;

	}