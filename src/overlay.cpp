#include "../libs/states.h"
#include "../libs/ui.h"

//  UpdateOverlay
//  returnTo: state sẽ quay về khi nhấn Back / ESC
void UpdateOverlay(AppContext& ctx, GameState returnTo)
{
    // Nút Back (vùng xấp xỉ)
    Vector2   mouse = GetMousePosition();
    Rectangle rect = { 30, 30, 180, 60 };
    bool backClicked = CheckCollisionPointRec(mouse, rect)
        && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    if (backClicked || IsKeyPressed(KEY_ESCAPE))
        ctx.state = returnTo;
}

//  DrawOverlay
void DrawOverlay(const AppContext& ctx, const TextureBank& tex, const char* title)
{
    bool wantsBack = DrawOverlayScreen(tex.originBg, title,
        tex.buttonBackNormal,
        tex.buttonBackHover);
    (void)wantsBack;
}