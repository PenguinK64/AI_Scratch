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

Color backgroundColor = { 138, 124, 96, 255 };


// ==================== VARIABLES ====================


const int windowWidth = 1290;
const int windowHeight = 720;

Camera2D camera = { 0 };

Texture2D dogLeftTexture;
Texture2D dogRightTexture;
Texture2D dogTexture;

Texture2D sheepTexture;


// ==================== PLAYER ====================


typedef struct
{
	Vector2 position;
	Vector2 size;
	Vector2 velocity;

	float speed;

	bool facingRight;

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
	float Spook_radius;

	float Separation_radius;
	

	SheepState currentState;

	Color colour;

} Sheep;


#define Sheep_Count 5
Sheep sheep [Sheep_Count];



// ==================== SHEEP AI LOGIC ====================


Vector2 CalculateSeparation(Sheep* currentSheep, Sheep sheep[])
{

	Vector2 separation = { 0.0f, 0.0f };
	int neighbourCount = 0;

	for (int i = 0; i < Sheep_Count; i++)
	{
		Sheep* otherSheep = &sheep[i];

		if (otherSheep != currentSheep)
		{
			float distance = Vector2Distance(
				currentSheep->position,
				otherSheep->position
			);

			if (distance < currentSheep->Separation_radius)
			{
				separation.x += otherSheep->position.x - currentSheep->position.x;
				separation.y += otherSheep->position.y - currentSheep->position.y;

				neighbourCount++;
			
			}
		}
	}

	if (neighbourCount == 0)
	{
		return separation;
	}

	separation.x /= neighbourCount;
	separation.y /= neighbourCount;

	separation.x *= -1;
	separation.y *= -1;

	separation = Vector2Normalize(separation);

	return separation;
}
	




void UpdateSheep(Sheep* s, Sheep sheep[], Player* p, float dt) 
{
	if (!s || !p)
	{
		return;
	}

	// Distance from sheep to player
	Vector2 difference = Vector2Subtract(s->position, p->position);
	float distance = Vector2Length(difference);

	

	Vector2 separation = CalculateSeparation(s, sheep);

	


	switch (s->currentState)
	{
		case SHEEP_CALM:
		{
			s->colour = LIGHTGRAY;

			// wander code
			if (distance < s->Spook_radius)
			{
				s->currentState = SHEEP_FLEEING;
			}

			break;
		}

		case SHEEP_FLEEING:
		{
			s->colour = RED;


			//Flee

			Vector2 desiredvelocity = Vector2Subtract(s->position, p->position);

			desiredvelocity = Vector2Normalize(desiredvelocity);

			desiredvelocity = Vector2Scale(desiredvelocity, s->speed);

			Vector2 steering = Vector2Subtract(desiredvelocity, s->velocity);

			s->velocity = Vector2Add(s->velocity, steering);


			// Separation

			float separationWeight = 100.0f;

			s->velocity.x += separation.x * separationWeight;
			s->velocity.y += separation.y * separationWeight;


			//Move
			s->position.x += s->velocity.x * dt;
			s->position.y += s->velocity.y * dt;

			//Keep sheep hitbox with sheep
			s->hitbox.x = s->position.x;
			s->hitbox.y = s->position.y;

			//Switch States
			if (distance > s->Flee_radius)
			{
				s->currentState = SHEEP_CALM;
			}

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

	dogLeftTexture = LoadTexture("dog-Left.png");
	dogRightTexture = LoadTexture("dog-Right.png");

	// Starting texture
	dogTexture = dogRightTexture;

	p->position = (Vector2){ 600.0f, 350.0f };
	p->size = (Vector2){ 80.0f, 75.0f };
	p->velocity = (Vector2){ 0.0f, 0.0f };

	p->speed = 250.0f;

	p->facingRight = true;

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

	s->Flee_radius = 300;
	s->Spook_radius = 150;

	s->Separation_radius = 100.f;
	

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
	for (int i = 0; i < Sheep_Count; i++)
	{
		InitialiseSheep(&sheep[i]);

		sheep[i].position.x += i * 100.0f;
		
		sheep[i].hitbox.x = sheep[i].position.x;
		sheep[i].hitbox.y = sheep[i].position.y;
	
	}

	
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
		dogTexture = dogLeftTexture;

	}

	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
	{
		force.x += 1.0f;
		dogTexture = dogRightTexture;
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

	//camera.target = (Vector2){ p->position.x, p->position.y - 100 };


	// Player / sheep collision
	for (int i = 0; i < Sheep_Count; i++)
	{
		if (CheckCollisionRecs(p->hitbox, sheep[i].hitbox))
		{
			p->position = oldPos;

			p->hitbox.x = oldPos.x;
			p->hitbox.y = oldPos.y;
		}
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

	DrawTextureEx(
		dogTexture,
		p->position, 0,
		0.3,
		WHITE
	);
	
}



void DrawGame()
{
	BeginDrawing();

	BeginMode2D(camera);

	ClearBackground(backgroundColor);

	DrawPlayer(&player);

	for (int i = 0; i < Sheep_Count; i++)
	{
		DrawSheep(&sheep[i]);
	}

	// Debug hitboxes
	DrawRectangleLinesEx(player.hitbox, 2.0f, RED);

	for (int i = 0; i < Sheep_Count; i++)
	{
		DrawRectangleLinesEx(sheep[i].hitbox, 2.0f, BLUE);
	}
	

	DrawText("Sheep Herding AI", 20, 20, 20, DARKGRAY);

	EndMode2D();

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


	camera.target = (Vector2){ 0,0 };
	camera.offset = (Vector2){ (float)windowWidth / 2, (float)windowHeight / 2 , };
	camera.rotation = 0.f;
	camera.zoom = 1.f;
	camera.target = (Vector2){ player.position.x, player.position.y - 100 };

	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		//Make sure the frames run at the same speed for every computer
		float dt = GetFrameTime();


		HandleInput(&player, dt);

		for (int i = 0; i < Sheep_Count; i++)
		{
			UpdateSheep(&sheep[i], sheep, &player, dt);
		}

		DrawGame();


	}
	// Release loaded resources and close the window.
	
		CloseWindow();
		return 0;

	}