#include "States.h"
#include "UI.h"

//  UpdateOverlay — dùng chung cho Settings / Help / About
//  returnTo: UIScreen sẽ quay về khi nhấn Back / ESC
void UpdateOverlay(AppContext& ctx, UIScreen returnTo)
{
    Vector2   mouse = GetMousePosition();
    Rectangle rect = { BTN_BACK_X, BTN_BACK_Y, BTN_BACK_W, BTN_BACK_H };
    bool backClicked = CheckCollisionPointRec(mouse, rect)
        && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    if (backClicked || IsKeyPressed(KEY_ESCAPE))
        ctx.screen = returnTo;
}

//  DrawOverlay
void DrawOverlay(const AppContext& ctx, const TextureBank& tex, const char* title)
{
    DrawOverlayScreen(tex.originBg, title,
        tex.buttonBackNormal,
        tex.buttonBackHover);
}