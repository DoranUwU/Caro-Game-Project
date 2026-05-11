#include "States.h"
#include "UI.h"
#include "TextRenderer.h"
#include "Constants.h"
#include <math.h>

struct MenuItem { const char* text; int x; int y; };

static const MenuItem menuItems[] =
{
<<<<<<< HEAD
    { "New Game",  1320, 185 },
    { "Load Game", 1300, 320 },
    { "Settings",  1310, 450 },
    { "Help",      1380, 580 },
    { "About Us",  1320, 710 },
    { "Exit",      1380, 830 }
=======
    { "Menu.new_game",  1300, 185 },
    { "Menu.load_game", 1300, 318 },
    { "Menu.settings",  1300, 446 },
    { "Menu.help",      1300, 576 },
    { "Menu.about_us",  1300, 704 },
    { "Menu.exit",      1300, 826 }
>>>>>>> 426b846 (Adjust UI location)
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
                if (i == 0) { ctx.prevScreen = SCREEN_MENU; ctx.screen = SCREEN_MODE_SELECTION; }
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