#include "raylib.h"
#include "../libs/types.h"
#include "../libs/textures.h"
#include "../libs/states.h"

//  Dispatch Update theo state hiện tại
static void UpdateGame(AppContext& ctx)
{
    switch (ctx.state)
    {
    case MENU:           UpdateMenu(ctx);                              break;
    case MODE_SELECTION: UpdateModeSelection(ctx);                    break;
    case PLAYER_SETUP:   UpdatePlayerSetup(ctx);                      break;
    case GAMEPLAY:       UpdateGameplay(ctx);                         break;
    case SETTINGS:       UpdateOverlay(ctx, ctx.prevState);           break;
    case HELP:           UpdateOverlay(ctx, MENU);                    break;
    case ABOUT:          UpdateOverlay(ctx, MENU);                    break;
    }
}

//  Dispatch Draw theo state hiện tại
static void DrawGame(const AppContext& ctx, const TextureBank& tex)
{
    switch (ctx.state)
    {
    case MENU:           DrawMenu(ctx, tex);                          break;
    case MODE_SELECTION: DrawModeSelection(ctx, tex);                 break;
    case PLAYER_SETUP:   DrawPlayerSetup(ctx, tex);                   break;
    case GAMEPLAY:       DrawGameplay(ctx, tex);                      break;
    case SETTINGS:       DrawOverlay(ctx, tex, "SETTINGS");           break;
    case HELP:           DrawOverlay(ctx, tex, "HELP");               break;
    case ABOUT:          DrawOverlay(ctx, tex, "ABOUT US");           break;
    }
}

//  main
int main()
{
    InitWindow(1920, 1080, "Game Caro");
    SetTargetFPS(60);

    TextureBank tex;
    LoadAllTextures(tex);

    AppContext ctx;   

    while (!WindowShouldClose())
    {
        UpdateGame(ctx);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawGame(ctx, tex);
        EndDrawing();
    }

    UnloadAllTextures(tex);
    CloseWindow();
    return 0;
}