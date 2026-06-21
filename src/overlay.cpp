#include "States.h"
#include "UI.h"
#include "TextRenderer.h"
#include "Constants.h"
#include "Overlay.h"
#include <math.h>
#include <cstring>

// ============================================================
//  Shared helpers (panel, decorations) — phong cách Load Game
// ============================================================

Rectangle GetOverlayPanelRect(float wRatio, float hRatio)
{
    float pw = GetScreenWidth() * wRatio;
    float ph = GetScreenHeight() * hRatio;
    return {
        (GetScreenWidth() - pw) / 2.0f,
        (GetScreenHeight() - ph) / 2.0f,
        pw, ph
    };
}

void DrawClassicPanel(Rectangle panel, float glow)
{
    // Bóng đổ
    DrawRectangleRec({ panel.x + 8, panel.y + 8, panel.width, panel.height }, { 0, 0, 0, 60 });
    // Nền đen
    DrawRectangleRec(panel, { 0, 0, 0, 190 });
    // Viền chính
    unsigned char ba = (unsigned char)(160 + glow * 60);
    DrawRectangleLinesEx(panel, 2, { 200, 170, 80, ba });
    // Viền phụ bên trong
    DrawRectangleLinesEx(
        { panel.x + 6, panel.y + 6, panel.width - 12, panel.height - 12 },
        1, { 200, 170, 80, (unsigned char)(ba / 2) }
    );
    // Góc trang trí
    auto dot = [](float x, float y, Color c) {
        DrawRectangle((int)x - 3, (int)y - 3, 6, 6, c);
        };
    Color cc = { 255, 220, 100, ba };
    dot(panel.x, panel.y, cc);
    dot(panel.x + panel.width, panel.y, cc);
    dot(panel.x, panel.y + panel.height, cc);
    dot(panel.x + panel.width, panel.y + panel.height, cc);
}

void DrawOverlayTitle(const char* title, float glow)
{
    Color titleCol = { 255, 220, 100, (unsigned char)(200 + glow * 55) };
    int tW = (int)(strlen(title) * 8 * FONT_SCALE_XL);
    int tX = GetScreenWidth() / 2 - tW / 2;
    DrawPixelText(title, tX + 4, PANEL_TITLE_Y + 4, FONT_SCALE_XL, { 0, 0, 0, 160 });
    DrawPixelText(title, tX, PANEL_TITLE_Y, FONT_SCALE_XL, titleCol);

    // Đường kẻ trang trí
    int lineY = PANEL_TITLE_Y + FONT_SCALE_XL * 9 + 10;
    int lineLen = 400;
    int lineX = GetScreenWidth() / 2 - lineLen / 2;
    unsigned char lineA = (unsigned char)(120 + glow * 80);
    DrawRectangle(lineX, lineY, lineLen, 2, { 255, 220, 100, lineA });
    DrawRectangle(lineX + 20, lineY + 6, lineLen - 40, 1, { 255, 200,  80, (unsigned char)(lineA / 2) });
}

void DrawPanelDivider(float x, float y, float w, float alpha)
{
    DrawRectangle((int)x, (int)y, (int)w, 1, { 200, 170, 80, (unsigned char)alpha });
}

// ============================================================
//  UpdateOverlay
// ============================================================
void UpdateOverlay(AppContext& ctx, UIScreen returnTo)
{
    Vector2   mouse = GetMousePosition();
    Rectangle rect = { BTN_BACK_X, BTN_BACK_Y, BTN_BACK_W, BTN_BACK_H };
    bool backClicked = CheckCollisionPointRec(mouse, rect)
        && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    if (backClicked || IsKeyPressed(KEY_ESCAPE))
    {
        ctx.screen = returnTo;
        return;
    }

    // ---- Settings: xử lý toggle / chọn ngôn ngữ ----
    if (ctx.screen == SCREEN_SETTINGS)
    {
        UpdateSettingsOverlay(ctx);
    }
}

// ============================================================
//  DrawOverlay  (entry point từ main dispatch)
// ============================================================
void DrawOverlay(const AppContext& ctx, const TextureBank& tex, const char* title)
{
    // Background
    DrawFullscreenTexture(tex.originBg);

    // Nội dung theo màn hình
    if (ctx.screen == SCREEN_SETTINGS) DrawSettingsOverlay(ctx);
    else if (ctx.screen == SCREEN_HELP)     DrawHelpOverlay(ctx);
    else if (ctx.screen == SCREEN_ABOUT)    DrawAboutUsOverlay(ctx);

    // Nút Back (luôn có)
    bool hovered, clicked;
    DrawBackButton(tex.buttonBackNormal, tex.buttonBackHover, hovered, clicked);
}