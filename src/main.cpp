#include "raylib.h"
#include "Types.h"
#include "Sound.h"
#include "Textures.h"
#include "States.h"

static void UpdateGame(AppContext& ctx)
{
    switch (ctx.screen)
    {
    case SCREEN_MENU:           UpdateMenu(ctx); PlayMenuBGM(ctx);                           break;
    case SCREEN_LOADGAME:       UpdateOverlay(ctx, SCREEN_MENU);            break;
    case SCREEN_MODE_SELECTION: UpdateModeSelection(ctx);                   break;
    case SCREEN_PLAYER_SETUP:   UpdatePlayerSetup(ctx);                     break;
    case SCREEN_GAMEPLAY:       UpdateGameplay(ctx); PlayInGameBGM(ctx);                        break;
    case SCREEN_SETTINGS:       UpdateOverlay(ctx, ctx.prevScreen);         break;
    case SCREEN_HELP:           UpdateOverlay(ctx, SCREEN_MENU);            break;
    case SCREEN_ABOUT:          UpdateOverlay(ctx, SCREEN_MENU);            break;
    }
}

static void DrawGame(const AppContext& ctx, const TextureBank& tex)
{
    switch (ctx.screen)
    {
    case SCREEN_MENU:           DrawMenu(ctx, tex);                     break;
    case SCREEN_MODE_SELECTION: DrawModeSelection(ctx, tex);            break;
    case SCREEN_PLAYER_SETUP:   DrawPlayerSetup(ctx, tex);              break;
    case SCREEN_GAMEPLAY:       DrawGameplay(ctx, tex);                 break;
    case SCREEN_LOADGAME:       DrawOverlay(ctx, tex, "Load Game");     break;
    case SCREEN_SETTINGS:       DrawOverlay(ctx, tex, "SETTINGS");      break;
    case SCREEN_HELP:           DrawOverlay(ctx, tex, "HELP");          break;
    case SCREEN_ABOUT:          DrawOverlay(ctx, tex, "ABOUT US");      break;
    }
}

static void RunGame()
{
    InitWindow(1920, 1080, "Game Caro");
    SetTargetFPS(60);

    TextureBank tex;
    LoadAllTextures(tex);

    InitAudioDevice();
    SoundBank sound;
    LoadAllSounds(sound);

    AppContext ctx;
    ctx.sound = &sound;
    ctx.curBGM = BGM_NONE;
    while (!WindowShouldClose())
    {
        UpdateCurrentMusic(ctx);
        UpdateGame(ctx);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawGame(ctx, tex);
        EndDrawing();
    }
    UnloadAllSounds(sound);
    CloseAudioDevice();
    UnloadAllTextures(tex);
    CloseWindow();
}

int main()
{
    RunGame();
    return 0;
}