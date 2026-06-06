#include "States.h"
#include "UI.h"
#include "Board.h"
#include "TextRenderer.h"
#include "Save.h"
#include "Load.h"
#include "GameLogic.h"
#include "GameState.h"
#include "Constants.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <filesystem>
#include <ctime>
#include "Sound.h"
#include "Bot.h"

static const char* SAVE_FILE = "saves/current_game.json";

// ============================================================
//  Helpers
// ============================================================
static void GetBoardOrigin(int& startX, int& startY)
{
    int boardPx = BOARD_SIZE * CELL_SIZE;
    startX = (GetScreenWidth() - boardPx) / 2;
    startY = (GetScreenHeight() - boardPx) / 2;
}

static Texture2D GetCharTexture(const TextureBank& tex, int charIndex, bool faceLeft)
{
    switch (charIndex)
    {
    case 1:  return faceLeft ? tex.spriteMage_L : tex.spriteMage_R;
    case 2:  return faceLeft ? tex.spriteArcher_L : tex.spriteArcher_R;
    case 3:  return tex.spriteGoblin;   // Easy bot
    case 4:  return tex.spriteDragon;   // Hard bot
    default: return faceLeft ? tex.spriteKnight_L : tex.spriteKnight_R;
    }
}

static const char* GetCharName(int charIndex, const map<string, string>& langMap)
{
    switch (charIndex) {
    case 1:  return getText("Gameplay.mage", langMap);
    case 2:  return getText("Gameplay.archer", langMap);
    case 3:  return getText("Gameplay.goblin", langMap);
    case 4:  return getText("Gameplay.dragon", langMap);
    default: return getText("Gameplay.knight", langMap);
    }
}

// ============================================================
//  DrawCharacterSide
// ============================================================
static void DrawCharacterSide(
    Texture2D sprite, int x, int y,
    bool isCurrentTurn, float time,
    const char* charName,
    int moveCount, int winCount,
    int overrideX,
    const map<string, string>& langMap,
    int charIndex = 0)
{

    float scaleX = (float)CHAR_DISPLAY_W / sprite.width;
    float scaleY = (float)CHAR_DISPLAY_H / sprite.height;
    float scale = fminf(scaleX, scaleY) * 1.5f;

    if (charIndex == 4) scale *= 1.6f;

    float drawW = sprite.width * scale;
    float drawH = sprite.height * scale;

    float offsetX = x + (CHAR_DISPLAY_W - drawW) / 2;
    float offsetY = y + (CHAR_DISPLAY_H - drawH);

    if (charIndex == 4 && x > SCREEN_W / 2) offsetX += 100;

    // --- Sprite (flip hoac binh thuong) ---
    DrawTextureEx(sprite,
        Vector2{ offsetX, offsetY },
        0, scale, WHITE);

    // --- Khung thong tin xanh dam ben duoi ---
    int infoX = overrideX; 
    int infoY = y + CHAR_DISPLAY_H + 70; 

    // Vẽ tên nhân vật
    DrawPixelText(charName, infoX, infoY, FONT_SCALE_MD, { 255,220,80,255 });

    // Vẽ Move và Win
    char buf[32];
    snprintf(buf, sizeof(buf), "%s %d", getText("Gameplay.move_label", langMap), moveCount);
    DrawPixelText(buf, infoX, infoY + 40, FONT_SCALE_SM, WHITE);

    snprintf(buf, sizeof(buf), "%s %d", getText("Gameplay.win_label", langMap), winCount);
    DrawPixelText(buf, infoX, infoY + 70, FONT_SCALE_SM, WHITE);
}

// ============================================================
//  DrawTimerText
// ============================================================
static void DrawTimerText(float timeLeft, float time)
{
    int boxW = 220;
    int boxH = 90;
    int boxX = TIMER_CENTER_X - boxW / 2;
    int boxY = TIMER_Y - 10;

    char buf[8];
    int secs = (int)ceilf(timeLeft);
    if (secs < 0) secs = 0;
    snprintf(buf, sizeof(buf), "%d", secs);

    bool warn = timeLeft <= TIMER_WARN;
    float glow = (sinf(time * (warn ? 8.0f : 3.0f)) + 1.0f) / 2.0f;

    int tScale = TIMER_TEXT_SCALE;
    int tW = (int)(strlen(buf) * (FONT_GLYPH_W + FONT_SPACING) * tScale);
    int tH = FONT_GLYPH_H * tScale;
    int tx = TIMER_CENTER_X - tW / 2;
    int ty = TIMER_Y;

    Color col = warn
        ? Color{ 255, 80, 80, (unsigned char)(200 + glow * 55) }
    : Color{ 255, 215, 40, (unsigned char)(210 + glow * 45) };

    DrawPixelText(buf, tx + 2, ty + 2 + 40, tScale, { 0, 0, 0, 130 });
    DrawPixelText(buf, tx, ty + 40, tScale, col);
}

// ============================================================
//  UpdateGameplay
// ============================================================
void UpdateGameplay(AppContext& ctx)
{
    float dt = GetFrameTime();

    if (ctx.enterGuard)
    {
        ctx.enterGuard = false;
        return;
    }

    // --- Đếm ngược thông báo kết quả save/load ---
    if (ctx.saveLoadMsgTimer > 0.0f)
    {
        ctx.saveLoadMsgTimer -= dt;
        if (ctx.saveLoadMsgTimer < 0.0f) ctx.saveLoadMsgTimer = 0.0f;
    }

    // --- Xử lý dialog Save / Load khi đang nhập tên file ---
    if (ctx.saveLoadMode != AppContext::SaveLoadMode::NONE)
    {
        // ESC huỷ dialog
        if (IsKeyPressed(KEY_ESCAPE))
        {
            ctx.saveLoadMode = AppContext::SaveLoadMode::NONE;
            ctx.saveLoadInput.clear();
            return;
        }

        // Nhập ký tự
        int key = GetCharPressed();
        while (key > 0)
        {
            // Chỉ cho phép ký tự hợp lệ trong tên file
            if (((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z') ||
                (key >= '0' && key <= '9') || key == '_' || key == '-')
                && ctx.saveLoadInput.size() < 20)
            {
                ctx.saveLoadInput.push_back((char)key);
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !ctx.saveLoadInput.empty())
            ctx.saveLoadInput.pop_back();

        // ENTER: thực hiện save hoặc load
        if (IsKeyPressed(KEY_ENTER) && !ctx.saveLoadInput.empty())
        {
            char filepath[64];
            snprintf(filepath, sizeof(filepath), "saves/%s.json", ctx.saveLoadInput.c_str());

            if (ctx.saveLoadMode == AppContext::SaveLoadMode::SAVING)
            {
                std::filesystem::create_directories("saves");
                ctx.gameState.saveTime = (long long)std::time(nullptr);
                if (saveGameState(ctx.gameState, filepath))
                    ctx.saveLoadMsg = string(getText("Gameplay.saved_prefix", *ctx.curLanguage)) + ctx.saveLoadInput;
                else
                    ctx.saveLoadMsg = getText("Gameplay.save_failed", *ctx.curLanguage);
            }
            else // LOADING
            {
                GameState loaded;
                if (loadGameState(filepath, loaded))
                {
                    ctx.hasPlayedWinsfx = false;
                    ctx.gameState = loaded;
                    ctx.turnTimer = TURN_TIME;
                    ctx.saveLoadMsg = string(getText("Gameplay.loaded_prefix", *ctx.curLanguage)) + ctx.saveLoadInput;
                }
                else
                    ctx.saveLoadMsg = getText("Gameplay.file_not_found", *ctx.curLanguage);
            }

            ctx.saveLoadMsgTimer = 2.5f;
            ctx.saveLoadMode = AppContext::SaveLoadMode::NONE;
            ctx.saveLoadInput.clear();
        }
        return; 
    }

    // --- Xử lý dialog xác nhận thoát ---
    if (ctx.showExitConfirm)
    {
        // ESC đóng dialog (hủy thoát)
        if (IsKeyPressed(KEY_ESCAPE))
        {
            ctx.showExitConfirm = false;
            return;
        }

        // Điều hướng trái/phải giữa YES và NO
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
        {
            ctx.exitConfirmSelected = 0;
        }
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
        {
            ctx.exitConfirmSelected = 1;
        }

        // Xử lý click chuột vào nút YES / NO
        {
            Vector2 mouse = GetMousePosition();
            int pw = 700, ph = 280;
            int px = GetScreenWidth() / 2 - pw / 2;
            int py = GetScreenHeight() / 2 - ph / 2;
            int btnW = 200, btnH = 50;
            int btnY = py + ph - 80;
            int btnYesX = px + pw / 2 - btnW - 30;
            int btnNoX = px + pw / 2 + 30;

            Rectangle yesRect = { (float)btnYesX, (float)btnY, (float)btnW, (float)btnH };
            Rectangle noRect = { (float)btnNoX, (float)btnY, (float)btnW, (float)btnH };

            if (CheckCollisionPointRec(mouse, yesRect))
            {
                ctx.exitConfirmSelected = 0;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    ctx.showExitConfirm = false;
                    ctx.screen = SCREEN_MENU;
                    return;
                }
            }
            if (CheckCollisionPointRec(mouse, noRect))
            {
                ctx.exitConfirmSelected = 1;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    ctx.showExitConfirm = false;
                    return;
                }
            }
        }

        // ENTER xác nhận lựa chọn
        if (IsKeyPressed(KEY_ENTER))
        {
            if (ctx.exitConfirmSelected == 0)
            {
                // YES - thoát về menu
                ctx.showExitConfirm = false;
                ctx.screen = SCREEN_MENU;
            }
            else
            {
                // NO - đóng dialog, tiếp tục chơi
                ctx.showExitConfirm = false;
            }
            return;
        }

        return; // Chặn mọi input gameplay khi dialog đang hiện
    }

    // --- Nút Home (Back) / ESC: hiện dialog xác nhận thoát ---
    if (ctx.gameState.status == GameStatus::ONGOING)
    {
        Vector2   mouse = GetMousePosition();
        Rectangle rect = { BTN_BACK_X, BTN_BACK_Y, BTN_BACK_W, BTN_BACK_H };
        if ((CheckCollisionPointRec(mouse, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            || IsKeyPressed(KEY_ESCAPE))
        {
            ctx.showExitConfirm = true;
            ctx.exitConfirmSelected = 1; // mặc định chọn NO
            return;
        }
    }
    else
    {
        // Game đã kết thúc -> thoát thẳng về menu
        Vector2   mouse = GetMousePosition();
        Rectangle rect = { BTN_BACK_X, BTN_BACK_Y, BTN_BACK_W, BTN_BACK_H };
        if ((CheckCollisionPointRec(mouse, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            || IsKeyPressed(KEY_ENTER))
        {
            ctx.screen = SCREEN_MENU;
            return;
        }
    }

    if (ctx.gameState.status != GameStatus::ONGOING)
    {
        if (!ctx.hasPlayedWinsfx) {
            StopMusicStream(ctx.sound->ingame);
            PlaySfx(ctx.sound->win);
            ctx.hasPlayedWinsfx = true;
        }

        if (IsKeyPressed(KEY_ENTER))
            ctx.screen = SCREEN_MENU;
        return;
    }

    int startX, startY;
    GetBoardOrigin(startX, startY);

    // --- Bot turn logic ---
    if (ctx.gameState.status == GameStatus::ONGOING &&
        ctx.playWithBot &&
        ctx.gameState.currentPlayer == Player::PlayerO &&
        ctx.saveLoadMode == AppContext::SaveLoadMode::NONE &&
        !ctx.showExitConfirm)
    {
        Position botMove(-1, -1);
        if (ctx.difficulty == 0)
        {
            // Easy Bot: Random move in relevant moves
            MoveList moves = getRelevantMoves(ctx.gameState);
            if (moves.count > 0)
            {
                botMove = moves.list[GetRandomValue(0, moves.count - 1)];
            }
            else
            {
                // Fallback to random cell on board
                do {
                    botMove.row = GetRandomValue(0, BOARD_SIZE - 1);
                    botMove.column = GetRandomValue(0, BOARD_SIZE - 1);
                } while (!isValidMove(ctx.gameState.board, botMove));
            }
        }
        else
        {
            // Hard Bot: Negamax Search
            botMove = getBestMove(ctx.gameState, MAX_DEPTH);
        }

        if (botMove.row != -1 && botMove.column != -1)
        {
            GameState next = playMove(ctx.gameState, botMove);
            ctx.player2.moveCount++;
            ctx.gameState = next;
            ctx.turnTimer = TURN_TIME;
            PlaySound(ctx.sound->placeSfx);
        }
        return; // skip human update this frame
    }

    // --- Timer ---
    ctx.turnTimer -= dt;
    if (ctx.turnTimer <= 0.0f)
    {
        // Het gio: ben kia thang
        ctx.gameState.status =
            (ctx.gameState.currentPlayer == Player::PlayerX)
            ? GameStatus::WIN_O    
            : GameStatus::WIN_X;   
        ctx.turnTimer = 0.0f;
        return;
    }

    // --- Di chuyen cursor ---
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        ctx.cursorY--; if (ctx.cursorY < 0)          ctx.cursorY = BOARD_SIZE - 1;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        ctx.cursorY++; if (ctx.cursorY >= BOARD_SIZE) ctx.cursorY = 0;
    }
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
    {
        ctx.cursorX--; if (ctx.cursorX < 0)          ctx.cursorX = BOARD_SIZE - 1;
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
    {
        ctx.cursorX++; if (ctx.cursorX >= BOARD_SIZE) ctx.cursorX = 0;
    }

    // --- Danh bang phim ---
    auto tryMove = [&](int col, int row) {
        GameState next = playMove(ctx.gameState, Position(row, col));
        bool moveApplied = (next.currentPlayer != ctx.gameState.currentPlayer);

        if (moveApplied)
        {
            if (ctx.gameState.currentPlayer == Player::PlayerX)
                ctx.player1.moveCount++;
            else
                ctx.player2.moveCount++;

            ctx.gameState = next;
            ctx.turnTimer = TURN_TIME;
            PlaySound(ctx.sound->placeSfx);
        }
    };

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        tryMove(ctx.cursorX, ctx.cursorY);
        PlaySound(ctx.sound->placeSfx);
    }

    // --- Cursor theo chuot ---
    {
        Vector2 mouse = GetMousePosition();
        int mc = (int)(mouse.x - startX) / CELL_SIZE;
        int mr = (int)(mouse.y - (startY + 20)) / CELL_SIZE;
        if (mc >= 0 && mc < BOARD_SIZE && mr >= 0 && mr < BOARD_SIZE)
        {
            ctx.cursorX = mc; ctx.cursorY = mr;
        }
    }

    // --- Danh bang chuot ---
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouse = GetMousePosition();
        int mc = (int)(mouse.x - startX) / CELL_SIZE;
        int mr = (int)(mouse.y - (startY + 20)) / CELL_SIZE;
        if (mc >= 0 && mc < BOARD_SIZE && mr >= 0 && mr < BOARD_SIZE)
            tryMove(mc, mr);
    }

    // --- Cap nhat win count khi game ket thuc ---
    if (ctx.gameState.status == GameStatus::WIN_X)
        ctx.player1.winCount++;
    else if (ctx.gameState.status == GameStatus::WIN_O)
        ctx.player2.winCount++;

    // --- L: mo dialog Save, T: mo dialog Load ---
    if (IsKeyPressed(KEY_L) && ctx.saveLoadMode == AppContext::SaveLoadMode::NONE)
    {
        ctx.saveLoadMode = AppContext::SaveLoadMode::SAVING;
        ctx.saveLoadInput.clear();
        ctx.saveLoadMsg.clear();
    }
    if (IsKeyPressed(KEY_T) && ctx.saveLoadMode == AppContext::SaveLoadMode::NONE)
    {
        ctx.saveLoadMode = AppContext::SaveLoadMode::LOADING;
        ctx.saveLoadInput.clear();
        ctx.saveLoadMsg.clear();
    }

    // --- Nut Settings ---
    {
        Vector2   mouse = GetMousePosition();
        Rectangle rect = { (float)(SCREEN_W - BTN_SETTINGS_OFFSET),
                             (float)BTN_BACK_Y, (float)BTN_MAX_W, (float)BTN_BACK_H };
        if (CheckCollisionPointRec(mouse, rect)
            && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            ctx.prevScreen = SCREEN_GAMEPLAY; ctx.screen = SCREEN_SETTINGS;
        }
    }
}

// ============================================================
//  DrawSaveLoadDialog
// ============================================================
static void DrawSaveLoadDialog(const AppContext& ctx)
{
    if (ctx.saveLoadMode == AppContext::SaveLoadMode::NONE) return;

    float t = (float)GetTime();
    float glow = (sinf(t * 4.0f) + 1.0f) / 2.0f;

    bool isSave = (ctx.saveLoadMode == AppContext::SaveLoadMode::SAVING);

    // Nền mờ toàn màn hình
    DrawRectangle(0, 0, 1920, 1080, { 0, 0, 0, 160 });

    // Panel
    int pw = 680, ph = 240;
    int px = 620;
    int py = 420;

    DrawRectangle(px + 6, py + 6, pw, ph, { 0, 0, 0, 60 });
    DrawRectangle(px, py, pw, ph, { 0, 0, 0, 200 });
    unsigned char ba = (unsigned char)(160 + glow * 60);
    DrawRectangleLinesEx({ (float)px, (float)py, (float)pw, (float)ph }, 2,
        { 200, 170, 80, ba });
    DrawRectangleLinesEx({ (float)(px + 5), (float)(py + 5), (float)(pw - 10), (float)(ph - 10) }, 1,
        { 200, 170, 80, (unsigned char)(ba / 2) });

    // Tiêu đề
    const char* title = isSave
        ? getText("Gameplay.save_game", *ctx.curLanguage)
        : getText("Gameplay.load_game", *ctx.curLanguage);
    unsigned char tb = (unsigned char)(200 + glow * 55);
    Color titleCol = { 255, 220, 100, tb };
    DrawPixelText(title, 852, py + 28, FONT_SCALE_LG, titleCol);

    // Label hướng dẫn
    const char* prompt = getText("Gameplay.enter_filename_no_spaces", *ctx.curLanguage);
    DrawPixelText(prompt, 710, py + 95, FONT_SCALE_SM,
        { 200, 190, 150, 220 });

    // Ô nhập tên file
    int boxW = 480, boxH = 46;
    int boxX = 720;
    int boxY = py + 130;
    DrawRectangle(boxX, boxY, boxW, boxH, { 15, 12, 5, 220 });
    DrawRectangleLinesEx({ (float)boxX, (float)boxY, (float)boxW, (float)boxH }, 2,
        { 200, 170, 80, (unsigned char)(140 + glow * 80) });

    // Text đang nhập
    std::string display = ctx.saveLoadInput + ".json";
    int dW = (int)(display.size() * (FONT_GLYPH_W + FONT_SPACING) * FONT_SCALE_SM);
    DrawPixelText(display.c_str(), boxX + 14, boxY + 10, FONT_SCALE_SM,
        { 255, 240, 180, 255 });

    // Con trỏ nhấp nháy
    float blink = (sinf(t * 5.0f) + 1.0f) / 2.0f;
    if (blink > 0.4f)
    {
        int curX = boxX + 14 + (int)(ctx.saveLoadInput.size() * (FONT_GLYPH_W + FONT_SPACING) * FONT_SCALE_SM);
        DrawRectangle(curX, boxY + 8, 3, FONT_GLYPH_H * FONT_SCALE_SM, { 255, 220, 80, 220 });
    }

    // Hint ENTER / ESC
    const char* hint = getText("Gameplay.enter_confirm_esc_cancel", *ctx.curLanguage);
    DrawPixelText(hint, 780, py + ph - 34, 2,
        { 150, 140, 100, 160 });
}

// ============================================================
//  DrawGameplay
// ============================================================
void DrawGameplay(const AppContext& ctx, const TextureBank& tex)
{
    float t = (float)GetTime();

    DrawFullscreenTexture(tex.gameplayBg);

    int startX, startY;
    GetBoardOrigin(startX, startY);

    bool p1Turn = (ctx.gameState.currentPlayer == Player::PlayerX)
        && (ctx.gameState.status == GameStatus::ONGOING);
    bool p2Turn = (ctx.gameState.currentPlayer == Player::PlayerO)
        && (ctx.gameState.status == GameStatus::ONGOING);

    // --- Nhan vat trai ---
    DrawCharacterSide(
        GetCharTexture(tex, ctx.player1.character, true),
        CHAR_LEFT_X + 70, CHAR_Y + 70,
        p1Turn, t,
        GetCharName(ctx.player1.character, *ctx.curLanguage),
        ctx.player1.moveCount, ctx.player1.winCount,
        330,
        *ctx.curLanguage,
        ctx.player1.character
    );

    // --- Nhan vat phai (P2 hoac Bot) ---
    int p2CharIndex = (ctx.playWithBot)
        ? (ctx.difficulty == 0 ? 3 : 4)
        : ctx.player2.character;
    DrawCharacterSide(
        GetCharTexture(tex, p2CharIndex, false),
        CHAR_RIGHT_X + 150, CHAR_Y + 70,
        p2Turn, t,
        GetCharName(p2CharIndex, *ctx.curLanguage),
        ctx.player2.moveCount, ctx.player2.winCount,
        SCREEN_W - 450,
        *ctx.curLanguage,
        p2CharIndex
    );

    // --- Ban co ---
    DrawBoard(startX, startY + 20,
        ctx.gameState,
        tex.tileLight, tex.tileDark,
        tex.spriteX, tex.spriteO,
        ctx.cursorX, ctx.cursorY);

    // --- Ten nguoi choi ---
    const std::string& nameL = ctx.player1.name.empty() ? getText("Gameplay.player_1", *ctx.curLanguage) : ctx.player1.name;
    const std::string& nameR = ctx.player2.name.empty()
        ? (ctx.playWithBot ? getText("Gameplay.bot", *ctx.curLanguage) : getText("Gameplay.player_2", *ctx.curLanguage))
        : ctx.player2.name;
    DrawPixelText(nameL.c_str(), HUD_PLAYER1_X + 40, HUD_Y, FONT_SCALE_MD, WHITE);
    DrawPixelText(nameR.c_str(), HUD_PLAYER2_X, HUD_Y, FONT_SCALE_MD, WHITE);

    // --- Timer ---
    if (ctx.gameState.status == GameStatus::ONGOING)
        DrawTimerText(ctx.turnTimer, t);

    // --- Man hinh ket thuc ---
    if (ctx.gameState.status != GameStatus::ONGOING)
    {
        const char* msg = "";
        if (ctx.gameState.status == GameStatus::WIN_X) msg = getText("Gameplay.x_wins", *ctx.curLanguage);
        else if (ctx.gameState.status == GameStatus::WIN_O) msg = getText("Gameplay.o_wins", *ctx.curLanguage);
        else if (ctx.gameState.status == GameStatus::DRAW)  msg = getText("Gameplay.draw", *ctx.curLanguage);
        // Overlay toi
        DrawRectangle(0, 0, 1920, 1080, { 0, 0, 0, 160 });

        // --- Ve lai highlight duong thang SAU overlay de no hien tren cung ---
        if (ctx.gameState.winLineCount > 0)
        {
            float pulse = (sinf(t * 6.0f) + 1.0f) / 2.0f;

            for (int i = 0; i < ctx.gameState.winLineCount; i++)
            {
                int drawX = startX + ctx.gameState.winLine[i].column * CELL_SIZE;
                int drawY = (startY + 20) + ctx.gameState.winLine[i].row * CELL_SIZE;

                // Highlight vang chop nhay
                unsigned char a1 = (unsigned char)(120 + pulse * 135);
                DrawRectangle(drawX, drawY, CELL_SIZE, CELL_SIZE, { 255, 215, 0, a1 });

                // Vien sang
                unsigned char a2 = (unsigned char)(180 + pulse * 75);
                DrawRectangleLinesEx(
                    { (float)drawX, (float)drawY, (float)CELL_SIZE, (float)CELL_SIZE },
                    3, { 255, 240, 80, a2 });
            }

            // Ve lai quan co tren o thang
            for (int i = 0; i < ctx.gameState.winLineCount; i++)
            {
                int r = ctx.gameState.winLine[i].row;
                int c = ctx.gameState.winLine[i].column;
                Player cell = ctx.gameState.board.cell[r * BOARD_SIZE + c];
                if (cell == Player::NONE) continue;

                int drawX = startX + c * CELL_SIZE;
                int drawY = (startY + 20) + r * CELL_SIZE;
                Texture2D sprite = (cell == Player::PlayerX) ? tex.spriteX : tex.spriteO;
                float sprScale = (float)CELL_SIZE / sprite.width;
                DrawTextureEx(sprite, Vector2{ (float)drawX, (float)drawY }, 0, sprScale, WHITE);
            }
        }

        DrawPixelText(msg, 844, 154,
            FONT_SCALE_XL, { 0, 0, 0, 140 });
        DrawPixelText(msg, 840, 150,
            FONT_SCALE_XL, GOLD);

        const char* hint = getText("Gameplay.press_enter_or_click_home_to_return", *ctx.curLanguage);
        DrawPixelText(hint,
            700,
            940,
            FONT_SCALE_MD, WHITE);
    }

    // --- Nut Home (Back) ---
    bool homeHovered, homeClicked;
    DrawBackButton(tex.buttonBackNormal, tex.buttonBackHover, homeHovered, homeClicked);

    // --- Nut Settings ---
    bool hovered, clicked;
    DrawSettingButton(tex.buttonSettingNormal, tex.buttonSettingHover,
        GetScreenWidth(), hovered, clicked);

    // --- Goi y phim ---
    int barH = 30;
    DrawRectangle(0, 1050,
        1920, barH, { 0,0,0,200 });
    DrawPixelText(getText("Gameplay.l_save_t_load", *ctx.curLanguage),
        700, 1050, FONT_SCALE_SM - 1, WHITE);

    // --- Thong bao ket qua save/load ---
    if (ctx.saveLoadMsgTimer > 0.0f && !ctx.saveLoadMsg.empty())
    {
        float alpha01 = ctx.saveLoadMsgTimer > 0.4f ? 1.0f : ctx.saveLoadMsgTimer / 0.4f;
        unsigned char ma = (unsigned char)(220 * alpha01);
        bool isErr = (ctx.saveLoadMsg.find("FAILED") != std::string::npos ||
            ctx.saveLoadMsg.find("NOT FOUND") != std::string::npos);
        Color mc = isErr ? Color{ 255, 80, 80, ma } : Color{ 120, 255, 120, ma };
        DrawRectangle(0, 1020, 1920, 26, { 0,0,0,(unsigned char)(140 * alpha01) });
        DrawPixelText(ctx.saveLoadMsg.c_str(),
            850,
            1022,
            FONT_SCALE_SM, mc);
    }

    // --- Dialog nhập tên file save/load ---
    DrawSaveLoadDialog(ctx);

    // --- Dialog xác nhận thoát ---
    if (ctx.showExitConfirm)
    {
        float t0 = (float)GetTime();
        float glow0 = (sinf(t0 * 4.0f) + 1.0f) / 2.0f;

        // Nền mờ toàn màn hình
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, 180 });

        // Panel chính
        int pw = 700, ph = 280;
        int px = GetScreenWidth() / 2 - pw / 2;
        int py = GetScreenHeight() / 2 - ph / 2;

        // Bóng đổ panel
        DrawRectangle(px + 6, py + 6, pw, ph, { 0, 0, 0, 80 });
        // Nền panel tối
        DrawRectangle(px, py, pw, ph, { 15, 12, 8, 230 });
        // Viền ngoài vàng nhấp nháy
        unsigned char ba0 = (unsigned char)(160 + glow0 * 60);
        DrawRectangleLinesEx({ (float)px, (float)py, (float)pw, (float)ph }, 3,
            { 200, 170, 80, ba0 });
        // Viền trong mờ
        DrawRectangleLinesEx({ (float)(px + 6), (float)(py + 6), (float)(pw - 12), (float)(ph - 12) }, 1,
            { 200, 170, 80, (unsigned char)(ba0 / 2) });

        // === Tiêu đề ===
        const char* title0 = getText("Gameplay.exit_title", *ctx.curLanguage);
        unsigned char tb0 = (unsigned char)(200 + glow0 * 55);
        Color titleCol0 = { 255, 100, 80, tb0 };
        int tW0 = (int)(GetUTF8Length(title0) * (FONT_GLYPH_W + FONT_SPACING) * FONT_SCALE_LG);
        DrawPixelText(title0, GetScreenWidth() / 2 - tW0 / 2 + 2, py + 30 + 2, FONT_SCALE_LG, { 0, 0, 0, 180 });
        DrawPixelText(title0, GetScreenWidth() / 2 - tW0 / 2, py + 30, FONT_SCALE_LG, titleCol0);

        // === Dòng cảnh báo ===
        const char* line1 = getText("Gameplay.exit_warn", *ctx.curLanguage);
        int l1W = (int)(GetUTF8Length(line1) * (FONT_GLYPH_W + FONT_SPACING) * FONT_SCALE_SM);
        DrawPixelText(line1, GetScreenWidth() / 2 - l1W / 2, py + 95, FONT_SCALE_SM,
            { 220, 200, 160, 230 });

        const char* line2 = getText("Gameplay.exit_confirm", *ctx.curLanguage);
        int l2W = (int)(GetUTF8Length(line2) * (FONT_GLYPH_W + FONT_SPACING) * FONT_SCALE_SM);
        DrawPixelText(line2, GetScreenWidth() / 2 - l2W / 2, py + 130, FONT_SCALE_SM,
            { 200, 190, 150, 200 });

        // === Nút YES / NO ===
        int btnW = 200, btnH = 50;
        int btnY = py + ph - 80;
        int btnYesX = px + pw / 2 - btnW - 30;
        int btnNoX = px + pw / 2 + 30;

        // --- Nút YES ---
        bool yesSelected = (ctx.exitConfirmSelected == 0);
        {
            Color bgCol = yesSelected ? Color{ 140, 40, 30, 230 } : Color{ 40, 30, 20, 200 };
            DrawRectangle(btnYesX, btnY, btnW, btnH, bgCol);

            unsigned char borderA = yesSelected ? (unsigned char)(180 + glow0 * 75) : (unsigned char)120;
            Color borderCol = yesSelected ? Color{ 255, 100, 80, borderA } : Color{ 150, 130, 80, borderA };
            DrawRectangleLinesEx({ (float)btnYesX, (float)btnY, (float)btnW, (float)btnH }, 2, borderCol);

            const char* yesText = getText("Gameplay.yes", *ctx.curLanguage);
            int yw = (int)(GetUTF8Length(yesText) * (FONT_GLYPH_W + FONT_SPACING) * FONT_SCALE_MD);
            Color yesCol = yesSelected ? Color{ 255, 220, 120, (unsigned char)(200 + glow0 * 55) } : Color{ 180, 160, 120, 200 };
            DrawPixelText(yesText, btnYesX + btnW / 2 - yw / 2, btnY + 12, FONT_SCALE_MD, yesCol);
        }

        // --- Nút NO ---
        bool noSelected = (ctx.exitConfirmSelected == 1);
        {
            Color bgCol = noSelected ? Color{ 30, 80, 50, 230 } : Color{ 40, 30, 20, 200 };
            DrawRectangle(btnNoX, btnY, btnW, btnH, bgCol);

            unsigned char borderA = noSelected ? (unsigned char)(180 + glow0 * 75) : (unsigned char)120;
            Color borderCol = noSelected ? Color{ 100, 255, 120, borderA } : Color{ 150, 130, 80, borderA };
            DrawRectangleLinesEx({ (float)btnNoX, (float)btnY, (float)btnW, (float)btnH }, 2, borderCol);

            const char* noText = getText("Gameplay.no", *ctx.curLanguage);
            int nw = (int)(GetUTF8Length(noText) * (FONT_GLYPH_W + FONT_SPACING) * FONT_SCALE_MD);
            Color noCol = noSelected ? Color{ 255, 220, 120, (unsigned char)(200 + glow0 * 55) } : Color{ 180, 160, 120, 200 };
            DrawPixelText(noText, btnNoX + btnW / 2 - nw / 2, btnY + 12, FONT_SCALE_MD, noCol);
        }
    }
}