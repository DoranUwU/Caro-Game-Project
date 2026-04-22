#pragma once
// ============================================================
//  GAME LOGIC
// ============================================================
constexpr int BOARD_SIZE = 15;
constexpr int WIN_CONDITION = 5;
constexpr int MAX_DEPTH = 4;
constexpr int TOTAL_CELLS = BOARD_SIZE * BOARD_SIZE;
const int INF = 1e6;

// ============================================================
//  CỬA SỔ
// ============================================================
constexpr int SCREEN_W = 1920;
constexpr int SCREEN_H = 1080;
constexpr int TARGET_FPS = 60;

// ============================================================
//  BÀN CỜ
// ============================================================
constexpr int CELL_SIZE = 40;

// ============================================================
//  PIXEL FONT
// ============================================================
constexpr int FONT_GLYPH_W = 5;
constexpr int FONT_GLYPH_H = 7;
constexpr int FONT_SPACING = 1;
constexpr int FONT_SCALE_SM = 3;
constexpr int FONT_SCALE_MD = 4;
constexpr int FONT_SCALE_LG = 6;
constexpr int FONT_SCALE_XL = 8;

// ============================================================
//  NUT BACK / SETTINGS
// ============================================================
constexpr int BTN_BACK_X = 30;
constexpr int BTN_BACK_Y = 30;
constexpr int BTN_MAX_W = 180;
constexpr int BTN_SETTINGS_MARGIN = 30;
constexpr int BTN_SETTINGS_OFFSET = 210;
constexpr int BTN_BACK_W = 180;
constexpr int BTN_BACK_H = 60;

// ============================================================
//  GAMEPLAY HUD
// ============================================================
constexpr int HUD_Y = 68;
constexpr int HUD_PLAYER1_X = 300;
constexpr int HUD_PLAYER2_X = 1100;

// ============================================================
//  NHAN VAT
// ============================================================
constexpr int CHAR_DISPLAY_W = 320;
constexpr int CHAR_DISPLAY_H = 480;
constexpr int CHAR_LEFT_X = 175;
constexpr int CHAR_RIGHT_X = 1200;
constexpr int CHAR_Y = 320;

// ============================================================
//  TAM GIAC CHI LUOT
// ============================================================
constexpr int TRI_HALF_W = 44;
constexpr int TRI_HEIGHT = 52;

// ============================================================
//  TIMER DEM NGUOC
// ============================================================
constexpr float TURN_TIME = 30.0f;
constexpr float TIMER_WARN = 10.0f;
constexpr int   TIMER_TEXT_SCALE = 10;
constexpr int   TIMER_CENTER_X = 962;
constexpr int   TIMER_Y = 52;

// ============================================================
//  LOAD GAME SCREEN
// ============================================================
constexpr float PANEL_W_RATIO = 0.60f;
constexpr float PANEL_H_RATIO = 0.62f;
constexpr int   PANEL_TITLE_Y = 90;

// turn indicator
constexpr int ARROW_W = 16;  
constexpr int ARROW_H = 14;  
constexpr float BOUNCE_SPEED = 3.0f;  
constexpr float BOUNCE_AMP = 5.0f;