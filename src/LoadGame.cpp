#include "States.h"
#include "UI.h"
#include "TextRenderer.h"
#include "Constants.h"
#include "Load.h"
#include "Save.h"
#include <math.h>
#include <cstring>
#include <filesystem>
#include <algorithm>
#include <map>

static const int MAX_VISIBLE_SAVES = 6;

// Cache trạng thái preview để không load file mỗi frame
struct CachedPreview {
    bool valid = false;
    GameStatus status = GameStatus::ONGOING;
};
static std::map<std::string, CachedPreview> cachedPreviews;

static Rectangle GetPanelRect()
{
    float pw = GetScreenWidth() * PANEL_W_RATIO;
    float ph = GetScreenHeight() * PANEL_H_RATIO;
    return {
        (GetScreenWidth() - pw) / 2.0f,
        (GetScreenHeight() - ph) / 2.0f,
        pw, ph
    };
}

// Quét thư mục saves/ và cập nhật danh sách file vào ctx
static void RefreshSaveFileList(AppContext& ctx)
{
    ctx.saveFileList.clear();
    ctx.loadSelectedIndex = 0;

    if (!std::filesystem::exists("saves") || !std::filesystem::is_directory("saves"))
        return;

    // Thu thập tất cả file .json trong saves/
    struct SaveEntry {
        std::string filename;
        std::filesystem::file_time_type lastWrite;
    };
    std::vector<SaveEntry> entries;

    for (const auto& entry : std::filesystem::directory_iterator("saves"))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".json")
        {
            entries.push_back({
                entry.path().filename().string(),
                entry.last_write_time()
            });
        }
    }

    // Sắp xếp theo thời gian sửa mới nhất lên trước
    std::sort(entries.begin(), entries.end(),
        [](const SaveEntry& a, const SaveEntry& b) {
            return a.lastWrite > b.lastWrite;
        });

    for (const auto& e : entries)
        ctx.saveFileList.push_back(e.filename);

    // Cache preview status cho mỗi file
    cachedPreviews.clear();
    for (const auto& fname : ctx.saveFileList) {
        std::string filepath = "saves/" + fname;
        GameState preview;
        CachedPreview cp;
        if (loadGameState(filepath.c_str(), preview)) {
            cp.valid = true;
            cp.status = preview.status;
        }
        cachedPreviews[fname] = cp;
    }
}

void UpdateLoadGame(AppContext& ctx)
{
    // --- Lần đầu vào Load Game screen: quét saves/ ---
    // Dùng static để track trạng thái (chỉ refresh khi mới vào screen)
    static bool needRefresh = true;
    static UIScreen lastScreen = SCREEN_MENU;
    if (lastScreen != SCREEN_LOAD_GAME)
    {
        needRefresh = true;
    }
    lastScreen = ctx.screen;

    if (needRefresh)
    {
        RefreshSaveFileList(ctx);
        needRefresh = false;
    }

    // --- Nút Back ---
    {
        Vector2   mouse = GetMousePosition();
        Rectangle rect = { BTN_BACK_X, BTN_BACK_Y, BTN_BACK_W, BTN_BACK_H };
        bool backClicked = CheckCollisionPointRec(mouse, rect)
            && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        if (backClicked || IsKeyPressed(KEY_ESCAPE))
        {
            needRefresh = true; // Reset cho lần sau
            ctx.screen = SCREEN_MENU;
            return;
        }
    }

    int fileCount = (int)ctx.saveFileList.size();
    if (fileCount == 0) return;

    // --- Điều hướng bằng phím ---
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        ctx.loadSelectedIndex--;
        if (ctx.loadSelectedIndex < 0)
            ctx.loadSelectedIndex = fileCount - 1;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        ctx.loadSelectedIndex++;
        if (ctx.loadSelectedIndex >= fileCount)
            ctx.loadSelectedIndex = 0;
    }

    // --- Hover + Click chuột trên danh sách ---
    Rectangle panel = GetPanelRect();
    float listStartY = panel.y + 90.0f;
    float itemH = 54.0f;
    float listX = panel.x + 40.0f;
    float listW = panel.width - 80.0f;

    Vector2 mouse = GetMousePosition();
    for (int i = 0; i < fileCount && i < MAX_VISIBLE_SAVES; i++)
    {
        Rectangle itemRect = { listX, listStartY + i * itemH, listW, itemH - 4 };
        if (CheckCollisionPointRec(mouse, itemRect))
        {
            ctx.loadSelectedIndex = i;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                // Load file được chọn
                std::string filepath = "saves/" + ctx.saveFileList[i];
                GameState loaded;
                if (loadGameState(filepath.c_str(), loaded))
                {
                    ctx.hasPlayedWinsfx = false;
                    ctx.gameState = loaded;
                    ctx.cursorX = BOARD_SIZE / 2;
                    ctx.cursorY = BOARD_SIZE / 2;
                    ctx.turnTimer = TURN_TIME;
                    ctx.prevScreen = SCREEN_LOAD_GAME;
                    ctx.screen = SCREEN_GAMEPLAY;
                    needRefresh = true;
                }
                return;
            }
        }
    }

    // --- Enter để load file đang chọn ---
    if (IsKeyPressed(KEY_ENTER) && ctx.loadSelectedIndex >= 0 && ctx.loadSelectedIndex < fileCount)
    {
        std::string filepath = "saves/" + ctx.saveFileList[ctx.loadSelectedIndex];
        GameState loaded;
        if (loadGameState(filepath.c_str(), loaded))
        {
            ctx.hasPlayedWinsfx = false;
            ctx.gameState = loaded;
            ctx.cursorX = BOARD_SIZE / 2;
            ctx.cursorY = BOARD_SIZE / 2;
            ctx.turnTimer = TURN_TIME;
            ctx.prevScreen = SCREEN_LOAD_GAME;
            ctx.screen = SCREEN_GAMEPLAY;
            needRefresh = true;
        }
    }
}

void DrawLoadGame(const AppContext& ctx, const TextureBank& tex)
{
    float t = (float)GetTime();

    DrawFullscreenTexture(tex.originBg);

    // --- Tiêu đề ---
    const char* title = "LOAD GAME";
    float       glow = (sinf(t * 3.0f) + 1.0f) / 2.0f;
    Color       titleCol = { 255, 220, 100,
                             (unsigned char)(200 + glow * 55) };
    int         tW = (int)(strlen(title) * 8 *  FONT_SCALE_XL);
    int         tX = GetScreenWidth() / 2 - tW / 2;

    DrawPixelText(title, tX + 4, PANEL_TITLE_Y + 4,  FONT_SCALE_XL, { 0, 0, 0, 160 });
    DrawPixelText(title, tX, PANEL_TITLE_Y,  FONT_SCALE_XL, titleCol);

    // --- Đường kẻ dưới tiêu đề ---
    int lineY = PANEL_TITLE_Y +  FONT_SCALE_XL * 9 + 10;
    int lineLen = 400;
    int lineX = GetScreenWidth() / 2 - lineLen / 2;
    unsigned char lineA = (unsigned char)(120 + glow * 80);
    DrawRectangle(lineX, lineY, lineLen, 2, { 255, 220, 100, lineA });
    DrawRectangle(lineX + 20, lineY + 6, lineLen - 40, 1, { 255, 200, 80, (unsigned char)(lineA / 2) });

    // --- Panel ---
    Rectangle panel = GetPanelRect();

    DrawRectangleRec({ panel.x + 8, panel.y + 8, panel.width, panel.height },
        { 0, 0, 0, 60 });
    DrawRectangleRec(panel, { 0, 0, 0, 180 });

    unsigned char borderA = (unsigned char)(160 + glow * 60);
    DrawRectangleLinesEx(panel, 2, { 200, 170, 80, borderA });
    DrawRectangleLinesEx(
        { panel.x + 6, panel.y + 6, panel.width - 12, panel.height - 12 },
        1, { 200, 170, 80, (unsigned char)(borderA / 2) }
    );

    // --- Góc trang trí ---
    auto DrawCornerDot = [](float x, float y, Color c) {
        DrawRectangle((int)x - 3, (int)y - 3, 6, 6, c);
        };
    Color cornerCol = { 255, 220, 100, borderA };
    DrawCornerDot(panel.x, panel.y, cornerCol);
    DrawCornerDot(panel.x + panel.width, panel.y, cornerCol);
    DrawCornerDot(panel.x, panel.y + panel.height, cornerCol);
    DrawCornerDot(panel.x + panel.width, panel.y + panel.height, cornerCol);

    int fileCount = (int)ctx.saveFileList.size();
    float cx = panel.x + panel.width / 2.0f;

    if (fileCount == 0)
    {
        // --- Không có file save ---
        float contentY = panel.y + 100.0f;

        // Icon thanh kiếm
        {
            int sx = (int)cx - 16;
            int sy = (int)contentY;
            DrawRectangle(sx + 15, sy, 2, 40, { 200, 200, 210, 220 });
            DrawRectangle(sx + 6, sy + 38, 20, 4, { 180, 140, 60, 220 });
            DrawRectangle(sx + 14, sy - 6, 4, 8, { 220, 220, 230, 220 });
        }
        contentY += 70.0f;

        const char* noSave = "NO SAVE FILES FOUND";
        int nsW = (int)(strlen(noSave) * 8 * 4);
        DrawPixelText(noSave, (int)(cx - nsW / 2), (int)contentY,
            4, { 200, 80, 80, 220 });

        contentY += 60.0f;
        const char* hint = "PLAY A GAME AND PRESS L TO SAVE";
        int hW = (int)(strlen(hint) * 8 * 3);
        DrawPixelText(hint, (int)(cx - hW / 2), (int)contentY,
            3, { 150, 140, 110, 180 });
    }
    else
    {
        // --- Header: danh sách saves ---
        float headerY = panel.y + 30.0f;
        const char* header = "SELECT A SAVE FILE";
        int hdrW = (int)(strlen(header) * 8 * FONT_SCALE_SM);
        DrawPixelText(header, (int)(cx - hdrW / 2), (int)headerY,
            FONT_SCALE_SM, { 200, 190, 150, 220 });

        // Đường kẻ phân cách
        DrawRectangle((int)(panel.x + 40), (int)(headerY + 38),
            (int)(panel.width - 80), 1, { 200, 170, 80, 80 });

        // --- Danh sách file ---
        float listStartY = panel.y + 90.0f;
        float itemH = 54.0f;
        float listX = panel.x + 40.0f;
        float listW = panel.width - 80.0f;
        Vector2 mouse = GetMousePosition();

        int displayCount = (fileCount < MAX_VISIBLE_SAVES) ? fileCount : MAX_VISIBLE_SAVES;

        for (int i = 0; i < displayCount; i++)
        {
            Rectangle itemRect = { listX, listStartY + i * itemH, listW, itemH - 4 };
            bool isSelected = (i == ctx.loadSelectedIndex);
            bool isHovered = CheckCollisionPointRec(mouse, itemRect);

            // Nền item
            if (isSelected)
            {
                float selGlow = (sinf(t * 5.0f) + 1.0f) / 2.0f;
                unsigned char selA = (unsigned char)(60 + selGlow * 40);
                DrawRectangleRec(itemRect, { 255, 220, 80, selA });
                unsigned char brdA = (unsigned char)(180 + selGlow * 75);
                DrawRectangleLinesEx(itemRect, 2, { 255, 220, 80, brdA });
            }
            else if (isHovered)
            {
                DrawRectangleRec(itemRect, { 255, 220, 80, 25 });
                DrawRectangleLinesEx(itemRect, 1, { 200, 170, 80, 100 });
            }
            else
            {
                DrawRectangleRec(itemRect, { 0, 0, 0, 40 });
                DrawRectangleLinesEx(itemRect, 1, { 200, 170, 80, 50 });
            }

            // Tên file (bỏ đuôi .json để hiển thị gọn hơn)
            std::string displayName = ctx.saveFileList[i];
            if (displayName.size() > 5 && displayName.substr(displayName.size() - 5) == ".json")
                displayName = displayName.substr(0, displayName.size() - 5);

            // Số thứ tự
            char indexStr[8];
            snprintf(indexStr, sizeof(indexStr), "%d.", i + 1);

            Color textCol = isSelected
                ? Color{ 255, 240, 120, 255 }
                : Color{ 200, 190, 150, 220 };

            DrawPixelText(indexStr, (int)(listX + 12), (int)(listStartY + i * itemH + 14),
                FONT_SCALE_SM, textCol);
            DrawPixelText(displayName.c_str(), (int)(listX + 60), (int)(listStartY + i * itemH + 14),
                FONT_SCALE_SM, textCol);

            // Trạng thái game (từ cache, không load file mỗi frame)
            auto it = cachedPreviews.find(ctx.saveFileList[i]);
            if (it != cachedPreviews.end() && it->second.valid)
            {
                const char* statusStr = "ONGOING";
                Color statusCol = { 120, 200, 120, 200 };
                if (it->second.status == GameStatus::WIN_X) { statusStr = "X WINS"; statusCol = { 255, 200, 80, 200 }; }
                else if (it->second.status == GameStatus::WIN_O) { statusStr = "O WINS"; statusCol = { 255, 200, 80, 200 }; }
                else if (it->second.status == GameStatus::DRAW) { statusStr = "DRAW"; statusCol = { 200, 150, 80, 200 }; }

                int statusW = (int)(strlen(statusStr) * 8 * 2);
                DrawPixelText(statusStr,
                    (int)(listX + listW - statusW - 20),
                    (int)(listStartY + i * itemH + 18),
                    2, isSelected ? Color{ 255, 240, 120, 255 } : statusCol);
            }
        }

        // Thêm thông tin nếu có nhiều file hơn MAX_VISIBLE
        if (fileCount > MAX_VISIBLE_SAVES)
        {
            char moreStr[32];
            snprintf(moreStr, sizeof(moreStr), "... and %d more", fileCount - MAX_VISIBLE_SAVES);
            int mW = (int)(strlen(moreStr) * 8 * 2);
            DrawPixelText(moreStr, (int)(cx - mW / 2),
                (int)(listStartY + displayCount * itemH + 10),
                2, { 150, 140, 110, 160 });
        }

        // --- Nút LOAD ở dưới panel ---
        float btnW = 260, btnH = 54;
        float btnX = panel.x + (panel.width - btnW) / 2.0f;
        float btnY = panel.y + panel.height - btnH - 60.0f;
        Rectangle btnRect = { btnX, btnY, btnW, btnH };

        bool btnHover = CheckCollisionPointRec(mouse, btnRect);
        Color btnBg = btnHover ? Color{ 80, 70, 30, 220 } : Color{ 40, 35, 15, 200 };
        DrawRectangleRec(btnRect, btnBg);

        float btnGlow = btnHover ? (sinf(t * 6.0f) + 1.0f) / 2.0f : 0.0f;
        unsigned char ba = (unsigned char)(160 + btnGlow * 80);
        DrawRectangleLinesEx(btnRect, 2, { 200, 170, 80, ba });

        const char* btnLabel = "LOAD";
        Color btnTCol = { 255, 220, 100, (unsigned char)(200 + btnGlow * 55) };
        int lblW = (int)(strlen(btnLabel) * 8 * 5);
        DrawPixelText(btnLabel,
            (int)(btnX + btnW / 2 - lblW / 2),
            (int)(btnY + btnH / 2 - 5 * 4),
            5, btnTCol);

        // Hint
        const char* hintText = "UP/DOWN Select   ENTER/CLICK Load";
        int hw = (int)(strlen(hintText) * 8 * 2);
        DrawPixelText(hintText,
            (int)(btnX + btnW / 2 - hw / 2),
            (int)(btnY + btnH + 10),
            2, { 150, 140, 100, 140 });
    }

    bool hovered, clicked;
    DrawBackButton(tex.buttonBackNormal, tex.buttonBackHover, hovered, clicked);
}