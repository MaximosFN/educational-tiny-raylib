#include "miniray.h"

int main(void)
{
	InitWindow(800, 600, "MiniRay Renderer");

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(BLACK);

		DrawPixel(100, 100, WHITE);

		DrawRectangle(150, 150, 200, 100, RED);

		DrawCircle(500, 300, 80, GREEN);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
