#include <stack>
#include <stdint.h>
#include "raylib.h"

typedef struct s_components {


	uint8_t		registers[16];				// The registers of the processor

	uint16_t	index_register;				// Points to location in memory
	uint16_t    program_counter;			// Points to the current instruction in memory
	uint8_t		delay_timer;				// Will decrease from 60 to 0 every second
	uint8_t		sound_timer;				// Can go up to 60 aswell and decreases at the same rate, should play a sound when > 0

	std::stack<uint16_t> function_stack;	// Store return point from subroutines

	uint8_t		memory[4096];				// RAM available (4Kb)

	uint8_t		screen[32][64];				// Display (can only be 1 or 0, on or off)

}   t_components;

int main (int ac, char **av) {

	(void) ac;
	(void) av;
	
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 640;
    const int screenHeight = 320;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK); // other color is RAYWHITE

			DrawRectangle(76, 12, 10, 10, RAYWHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}