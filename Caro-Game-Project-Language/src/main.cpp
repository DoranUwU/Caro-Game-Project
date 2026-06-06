#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "Types.h"
#include "Textures.h"
#include "States.h"
#include "Sound.h"
#include "Lang.h"
#include "TextRenderer.h"

static void UpdateGame(AppContext& ctx)
{
    switch (ctx.screen)
    {
    case SCREEN_MENU:           UpdateMenu(ctx); PlayMenuBGM(ctx);      break;
    case SCREEN_LOAD_GAME:      UpdateLoadGame(ctx);                    break;
    case SCREEN_MODE_SELECTION: UpdateModeSelection(ctx);               break;
    case SCREEN_PLAYER_SETUP:   UpdatePlayerSetup(ctx);                 break;
    case SCREEN_GAMEPLAY:       UpdateGameplay(ctx);PlayInGameBGM(ctx); break;
    case SCREEN_SETTINGS:       UpdateOverlay(ctx, ctx.prevScreen);     break;
    case SCREEN_HELP:           UpdateOverlay(ctx, SCREEN_MENU);        break;
    case SCREEN_ABOUT:          UpdateOverlay(ctx, SCREEN_MENU);        break;
    case SCREEN_DIFFICULTY:     UpdateDifficulty(ctx);                  break;
    }
}

static void DrawGame(AppContext& ctx, const TextureBank& tex)
{
    switch (ctx.screen)
    {
    case SCREEN_MENU:           DrawMenu(ctx, tex);                      break;
    case SCREEN_LOAD_GAME:      DrawLoadGame(ctx, tex);                   break;
    case SCREEN_MODE_SELECTION: DrawModeSelection(ctx, tex);            break;
    case SCREEN_PLAYER_SETUP:   DrawPlayerSetup(ctx, tex);              break;
    case SCREEN_GAMEPLAY:       DrawGameplay(ctx, tex);                  break;
    case SCREEN_SETTINGS:       DrawOverlay(ctx, tex, getText("Menu.settings", *ctx.curLanguage)); break;
    case SCREEN_HELP:           DrawOverlay(ctx, tex, getText("Menu.help", *ctx.curLanguage));     break;
    case SCREEN_ABOUT:          DrawOverlay(ctx, tex, getText("Menu.about", *ctx.curLanguage));    break;
    case SCREEN_DIFFICULTY:     DrawDifficulty(ctx, tex);                break;
    }
}

static void RunGame()
{
    InitWindow(SCREEN_W, SCREEN_H, "Game Caro");
    SetTargetFPS(TARGET_FPS);

    TextureBank tex;
    LoadAllTextures(tex);

    // Initialize pixel font + unicode fallback
    InitTextRenderer();

    InitAudioDevice();
    SoundBank sound;
    LoadAllSounds(sound);

    LanguageBank lang;
    loadAllLanguage(lang);
    
    AppContext ctx;
    ctx.language = lang;
    ctx.curLanguage = &ctx.language.langEn;
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
    UnloadTextRenderer();
    CloseWindow();
}

int main()
{
    RunGame();
    return 0;
}