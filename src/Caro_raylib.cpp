#include "raylib.h"
#include "text_renderer.h"

int main()
{
    int screenWidth = 1920;
    int screenHeight = 1080;
    InitWindow(screenWidth, screenHeight, "Game Caro");
    Texture2D background = LoadTexture("assets/background_castle.jpg");
    Font menuFont = LoadFont("assets/fonts/PressStart2P.ttf");

    const char* items[] = {
        "New Game",
        "Load Game",
        "Settings",
        "Help",
        "About Us",
        "Exit"
    };
    const int itemCount = sizeof(items) / sizeof(items[0]);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        Rectangle src = { 0, 0, (float)background.width, (float)background.height };   // cắt toàn bộ ảnh  
        Rectangle dst = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() }; // phủ cả cửa sổ  
        Vector2 origin = { 0, 0 };

        DrawTexturePro(background, src, dst, origin, 0.0f, WHITE);

        DrawPixelTextStyled("New Game", 1320, 185, 6);
        DrawPixelTextStyled("Load Game", 1300, 320, 6);
        DrawPixelTextStyled("Settings", 1310, 450, 6);
        DrawPixelTextStyled("Help", 1380, 580, 6);
        DrawPixelTextStyled("About Us", 1320, 710, 6);
        DrawPixelTextStyled("Exit", 1380, 830, 6);

        EndDrawing();
    }
    UnloadTexture(background);
    CloseWindow();
}