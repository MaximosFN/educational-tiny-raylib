#ifndef MINIRAY_H
#define MINIRAY_H

typedef struct Color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} Color;

#define WHITE (Color){255, 255, 255, 255}
#define BLACK (Color){0, 0, 0, 255}
#define RED (Color){255, 0, 0, 255}
#define GREEN (Color){0, 255, 0, 255}
#define BLUE (Color){0, 0, 255, 255}

void InitWindow(int width, int height, const char *title);

int WindowShouldClose(void);

void CloseWindow(void);

float GetFrameTime(void);

int GetFPS(void);

void BeginDrawing(void);

void EndDrawing(void);

void ClearBackground(Color color);

void DrawPixel(int x, int y, Color color);

void DrawRectangle(int x, int y, int width, int height, Color color);

void DrawCircle(int centerX, int centerY, int radius, Color color);

#endif
