#include "States.h"
#include "UI.h"
#include "TextRenderer.h"
#include "Constants.h"
#include <math.h>
#include <cstring>

// ============================================================
//  Layout: 2 card trái/phải chiếm phần lớn màn hình
//  EASY (Goblin) bên trái, HARD (Dragon) bên phải
// ============================================================

static void GetDifficultyRects(Rectangle& easyRect, Rectangle& hardRect)
{
    int sw = GetScreenWidth(), sh = GetScreenHeight();
    int cardW = sw / 2 - 120;
    int cardH = (int)(sh * 0.68f);
    int cardY = sh / 2 - cardH / 2 + 30;

    easyRect = { 80.0f,           (float)cardY, (float)cardW, (float)cardH };
    hardRect = { (float)(sw / 2 + 40), (float)cardY, (float)cardW, (float)cardH };
}

// ============================================================
//  DrawDifficultyCard
// ============================================================
static void DrawDifficultyCard(
    Texture2D sprite,
    const char* label,
    const char* sublabel,       
    const char* desc[],         
    int         descCount,
    Rectangle   rect,
    bool        selected,
    bool        hovered,
    float       t,
    Color       accentColor,    
    bool        isDragon = false)
{
    float glow = (sinf(t * 4.0f) + 1.0f) / 2.0f;

    // --- Nền card ---
    unsigned char bgA = selected ? 220 : (hovered ? 195 : 180);
    Color bgBase = { 0, 0, 0, bgA };
    DrawRectangleRec(rect, bgBase);

    // unselected vẫn có màu nhẹ để phân biệt
    Color bgAccent = {
        accentColor.r, accentColor.g, accentColor.b,
        (unsigned char)(selected ? 35 : (hovered ? 22 : 15))
    };
    DrawRectangleRec(rect, bgAccent);

    // --- Viền ---
    if (selected)
    {
        unsigned char ba = (unsigned char)(180 + glow * 75);
        DrawRectangleLinesEx(rect, 3,
            { accentColor.r, accentColor.g, accentColor.b, ba });
        DrawRectangleLinesEx(
            { rect.x + 5, rect.y + 5, rect.width - 10, rect.height - 10 },
            1, { accentColor.r, accentColor.g, accentColor.b,
                 (unsigned char)(ba / 2) });
    }
    else if (hovered)
    {
        DrawRectangleLinesEx(rect, 2,
            { accentColor.r, accentColor.g, accentColor.b, 160 });
    }
    else
    {
        // Viền rõ hơn khi normal
        DrawRectangleLinesEx(rect, 2,
            { accentColor.r, accentColor.g, accentColor.b, 120 });
    }

    // --- Góc trang trí khi selected ---
    if (selected)
    {
        unsigned char ca = (unsigned char)(160 + glow * 80);
        Color cc = { accentColor.r, accentColor.g, accentColor.b, ca };
        int cs = 12;
        DrawRectangle((int)rect.x - 2, (int)rect.y - 2, cs, 3, cc);
        DrawRectangle((int)rect.x - 2, (int)rect.y - 2, 3, cs, cc);
        DrawRectangle((int)(rect.x + rect.width) - cs, (int)rect.y - 2, cs, 3, cc);
        DrawRectangle((int)(rect.x + rect.width) - 1, (int)rect.y - 2, 3, cs, cc);
        DrawRectangle((int)rect.x - 2, (int)(rect.y + rect.height) - 1, cs, 3, cc);
        DrawRectangle((int)rect.x - 2, (int)(rect.y + rect.height) - cs, 3, cs, cc);
        DrawRectangle((int)(rect.x + rect.width) - cs, (int)(rect.y + rect.height) - 1, cs, 3, cc);
        DrawRectangle((int)(rect.x + rect.width) - 1, (int)(rect.y + rect.height) - cs, 3, cs, cc);
    }

    // --- Sprite --- 
    float spriteAreaH = rect.height * 0.52f;
    float spriteScale = spriteAreaH / sprite.height;
    if (isDragon) spriteScale *= 1.2f;
    if (!selected && !hovered) spriteScale *= 0.88f;

    float drawW = sprite.width * spriteScale;
    float drawH = sprite.height * spriteScale;
    float sx = rect.x + (rect.width - drawW) / 2.0f;
    float sy = rect.y + 20.0f + (spriteAreaH - drawH) / 2.0f;

    // Tint: selected = WHITE, unselected = hơi xám
    Color spriteTint = selected ? WHITE
        : (hovered ? Color{ 220, 210, 195, 245 }
    : Color{ 180, 175, 165, 210 });
    DrawTextureEx(sprite, Vector2{ sx, sy }, 0, spriteScale, spriteTint);

    // --- Đường kẻ ngang phân cách sprite / text ---
    float divY = rect.y + spriteAreaH + 28.0f;
    unsigned char divA = selected ? (unsigned char)(100 + glow * 60) : 50;
    DrawRectangle((int)(rect.x + 30), (int)divY,
        (int)(rect.width - 60), 1,
        { accentColor.r, accentColor.g, accentColor.b, divA });

    // --- Label (EASY / HARD) ---
    float textY = divY + 16.0f;
    int labelScale = 6;
    int labelW = (int)(strlen(label) * (FONT_GLYPH_W + FONT_SPACING) * labelScale);
    int labelX = (int)(rect.x + rect.width / 2 - labelW / 2);

    Color labelCol;
    if (selected)
    {
        unsigned char lb = (unsigned char)(200 + glow * 55);
        labelCol = { accentColor.r, accentColor.g, accentColor.b, lb };
        DrawPixelText(label, labelX + 2, (int)textY + 2, labelScale, { 0, 0, 0, 100 });
    }
    else if (hovered)
        labelCol = { accentColor.r, accentColor.g, accentColor.b, 200 };
    else
        labelCol = { 140, 130, 110, 160 };
    DrawPixelText(label, labelX, (int)textY, labelScale, labelCol);

    textY += labelScale * (FONT_GLYPH_H + 2) + 8.0f;

    // --- Sublabel ---
    int subScale = 3;
    int subW = (int)(strlen(sublabel) * (FONT_GLYPH_W + FONT_SPACING) * subScale);
    int subX = (int)(rect.x + rect.width / 2 - subW / 2);
    Color subCol = selected
        ? Color{ accentColor.r, accentColor.g, accentColor.b, 180 }
    : Color{ 130, 120, 100, 120 };
    DrawPixelText(sublabel, subX, (int)textY, subScale, subCol);
    textY += subScale * (FONT_GLYPH_H + 2) + 12.0f;

    // --- Mô tả dòng ---
    int dScale = 2;
    for (int i = 0; i < descCount; i++)
    {
        int dW = (int)(strlen(desc[i]) * (FONT_GLYPH_W + FONT_SPACING) * dScale);
        int dX = (int)(rect.x + rect.width / 2 - dW / 2);
        Color dCol = selected
            ? Color{ 200, 190, 160, 200 }
        : Color{ 110, 105, 90, 120 };
        DrawPixelText(desc[i], dX, (int)textY, dScale, dCol);
        textY += dScale * (FONT_GLYPH_H + 2) + 6.0f;
    }
}

// ============================================================
//  UpdateDifficulty
// ============================================================
void UpdateDifficulty(AppContext& ctx)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        ctx.screen = SCREEN_MODE_SELECTION;
        return;
    }
    {
        Vector2   mouse = GetMousePosition();
        Rectangle rect = { BTN_BACK_X, BTN_BACK_Y, BTN_BACK_W, BTN_BACK_H };
        if (CheckCollisionPointRec(mouse, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            ctx.screen = SCREEN_MODE_SELECTION;
            return;
        }
    }

    bool keyLeft = IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A);
    bool keyRight = IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D);
    if (keyLeft || keyRight)
        ctx.difficulty = !ctx.difficulty;  

    if (IsKeyPressed(KEY_ENTER))
    {
        ctx.screen = SCREEN_PLAYER_SETUP;
        return;
    }
}

// ============================================================
//  DrawDifficulty
// ============================================================
void DrawDifficulty(AppContext& ctx, const TextureBank& tex)
{
    float t = (float)GetTime();
    float glow = (sinf(t * 3.0f) + 1.0f) / 2.0f;

    DrawFullscreenTexture(tex.originBg);

    // --- Tiêu đề ---
    const char* title = "SELECT DIFFICULTY";
    Color titleCol = { 255, 220, 100, (unsigned char)(200 + glow * 55) };
    int tW = (int)(strlen(title) * (FONT_GLYPH_W + FONT_SPACING) * FONT_SCALE_LG);
    int tX = GetScreenWidth() / 2 - tW / 2;
    DrawPixelText(title, tX + 3, PANEL_TITLE_Y + 3, FONT_SCALE_LG, { 0, 0, 0, 140 });
    DrawPixelText(title, tX, PANEL_TITLE_Y, FONT_SCALE_LG, titleCol);

    // Đường kẻ dưới tiêu đề
    int lineLen = 500;
    int lineX = GetScreenWidth() / 2 - lineLen / 2;
    int lineY = PANEL_TITLE_Y + FONT_SCALE_LG * (FONT_GLYPH_H + 1) + 8;
    unsigned char la = (unsigned char)(100 + glow * 80);
    DrawRectangle(lineX, lineY, lineLen, 2, { 255, 220, 100, la });
    DrawRectangle(lineX + 20, lineY + 6, lineLen - 40, 1, { 255, 200,  80, (unsigned char)(la / 2) });

    // --- Tính rect 2 card ---
    Rectangle easyRect, hardRect;
    GetDifficultyRects(easyRect, hardRect);

    // --- Hover chuột ---
    Vector2 mouse = GetMousePosition();
    bool hovEasy = CheckCollisionPointRec(mouse, easyRect);
    bool hovHard = CheckCollisionPointRec(mouse, hardRect);

    if (hovEasy) ctx.difficulty = 0;
    if (hovHard) ctx.difficulty = 1;

    // Click confirm
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (hovEasy) { ctx.difficulty = 0; ctx.screen = SCREEN_PLAYER_SETUP; return; }
        if (hovHard) { ctx.difficulty = 1; ctx.screen = SCREEN_PLAYER_SETUP; return; }
    }

    // --- Mô tả EASY ---
    const char* easyDesc[] = {
        "Bot moves randomly",
        "Great for beginners",
        "Relax and have fun!"
    };
    // --- Mô tả HARD ---
    const char* hardDesc[] = {
        "Bot looks 4 moves ahead",
        "Blocks and attacks smart",
        "Only for the brave!"
    };

    // --- Accent colors ---
    Color easyAccent = { 80,  200, 100, 255 };   // xanh lá
    Color hardAccent = { 220,  80,  60, 255 };   // đỏ cam

    // --- Vẽ 2 card ---
    DrawDifficultyCard(
        tex.spriteGoblin,
        "EASY", "Beginner Friendly",
        easyDesc, 3,
        easyRect,
        ctx.difficulty == 0,
        hovEasy && ctx.difficulty != 0,
        t, easyAccent, false);

    DrawDifficultyCard(
        tex.spriteDragon,
        "HARD", "True Challenge",
        hardDesc, 3,
        hardRect,
        ctx.difficulty == 1,
        hovHard && ctx.difficulty != 1,
        t, hardAccent, true);

    // --- VS ở giữa 2 card ---
    {
        float vsX = (easyRect.x + easyRect.width + hardRect.x) / 2.0f;
        float vsY = GetScreenHeight() / 2.0f;
        float pulse = (sinf(t * 3.0f) + 1.0f) / 2.0f;
        unsigned char vsA = (unsigned char)(160 + pulse * 80);
        int vsScale = 5;
        int vsW = (int)(2 * (FONT_GLYPH_W + FONT_SPACING) * vsScale);
        DrawPixelText("VS",
            (int)(vsX - vsW / 2),
            (int)(vsY - FONT_GLYPH_H * vsScale / 2),
            vsScale, { 255, 220, 80, vsA });
    }

    // --- Hint phím ---
    {
        const char* hint = "A/D or CLICK to select   ENTER to confirm";
        int hw = (int)(strlen(hint) * (FONT_GLYPH_W + FONT_SPACING) * 2);
        DrawPixelText(hint,
            GetScreenWidth() / 2 - hw / 2,
            GetScreenHeight() - 50,
            2, { 180, 160, 90, 160 });
    }

    // --- Back button ---
    bool hovered, clicked;
    DrawBackButton(tex.buttonBackNormal, tex.buttonBackHover, hovered, clicked);
}