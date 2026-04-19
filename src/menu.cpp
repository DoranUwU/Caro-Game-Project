#include "States.h"
#include "UI.h"
#include "TextRenderer.h"
#include "Constants.h"
#include <math.h>
#include "Sound.h"

struct MenuItem { const char* text; int x; int y; };

static const MenuItem menuItems[] =
{
    { "New Game",  1320, 185 },
    { "Load Game", 1300, 320 },
    { "Settings",  1310, 450 },
    { "Help",      1380, 580 },
    { "About Us",  1320, 710 },
    { "Exit",      1380, 830 }
};
static const int itemCount = (int)(sizeof(menuItems) / sizeof(menuItems[0]));

void UpdateMenu(AppContext& ctx)
{
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        ctx.selectedItem++;
        if (ctx.selectedItem >= itemCount) ctx.selectedItem = 0;
    }
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        ctx.selectedItem--;
        if (ctx.selectedItem < 0) ctx.selectedItem = itemCount - 1;
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        PlaySfx(ctx.sound->clickSfx);
        switch (ctx.selectedItem)
        {
        case 0: ctx.prevScreen = SCREEN_MENU; ctx.screen = SCREEN_MODE_SELECTION; break;
        case 1: ctx.prevScreen = SCREEN_MENU; ctx.screen = SCREEN_LOAD_GAME;      break;
        case 2: ctx.prevScreen = SCREEN_MENU; ctx.screen = SCREEN_SETTINGS;       break;
        case 3: ctx.prevScreen = SCREEN_MENU; ctx.screen = SCREEN_HELP;           break;
        case 4: ctx.prevScreen = SCREEN_MENU; ctx.screen = SCREEN_ABOUT;          break;
        case 5: CloseWindow();                                                     break;
        }
    }

    // Hover + click chuột
    Vector2 mouse = GetMousePosition();
    for (int i = 0; i < itemCount; i++)
    {
        Rectangle rect = { (float)menuItems[i].x, (float)menuItems[i].y, 350, 80 };
        if (CheckCollisionPointRec(mouse, rect))
        {
            ctx.selectedItem = i;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                PlaySfx(ctx.sound->clickSfx);
                if (i == 0) { ctx.prevScreen = SCREEN_MENU; ctx.screen = SCREEN_MODE_SELECTION; }
                if (i == 1) { ctx.prevScreen = SCREEN_MENU; ctx.screen = SCREEN_LOAD_GAME; }
                if (i == 2) { ctx.prevScreen = SCREEN_MENU; ctx.screen = SCREEN_SETTINGS; }
                if (i == 3) { ctx.prevScreen = SCREEN_MENU; ctx.screen = SCREEN_HELP; }
                if (i == 4) { ctx.prevScreen = SCREEN_MENU; ctx.screen = SCREEN_ABOUT; }
                if (i == 5) { CloseWindow(); }
            }
        }
    }
}

//  DrawMenu
void DrawMenu(const AppContext& ctx, const TextureBank& tex)
{
    DrawFullscreenTexture(tex.background);

    for (int i = 0; i < itemCount; i++)
    {
        if (i == ctx.selectedItem)
        {
            float glow = (sinf((float)GetTime() * 4.0f) + 1.0f) / 2.0f;
            Color glowColor = { 255, 220, 120, (unsigned char)(200 + glow * 55) };
            int   scale = (int)(FONT_SCALE_LG + glow);

            DrawPixelText(menuItems[i].text,
                menuItems[i].x + 3, menuItems[i].y + 3, scale, BLACK);
            DrawPixelText(menuItems[i].text,
                menuItems[i].x, menuItems[i].y, scale, glowColor);
        }
        else
        {
            DrawPixelTextStyled(menuItems[i].text,
                menuItems[i].x, menuItems[i].y, FONT_SCALE_LG);
        }
    }
}