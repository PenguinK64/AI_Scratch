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


// ==================== COLOUR PALETTE ====================

Color backgroundColor = { 220, 229, 209, 255 };

const int windowWidth = 1290;
const int windowHeight = 720;

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
	

	
	// Initialise the game state and bodies




	
	// Set Camera to follow the player with a slight offset.

	

	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		//Make sure the frames run at the same speed for every computer
		float dt = GetFrameTime();



	

		// Render the current frame.
	
	BeginDrawing();
	DrawRectangle(100, 100, 50, 50, RED);
	ClearBackground(backgroundColor);
	EndDrawing();


	}
	// Release loaded resources and close the window.
	
		CloseWindow();
		return 0;

	}