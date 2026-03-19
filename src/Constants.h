#ifndef CONSTANTS_H
#define CONSTANTS_H

//  GAME LOGIC
constexpr int BOARD_SIZE = 15;
constexpr int WIN_CONDITION = 5;
constexpr int MAX_DEPTH = 4;
constexpr int TOTAL_CELLS = BOARD_SIZE * BOARD_SIZE;

//  CỬA SỔ
constexpr int SCREEN_W = 1920;
constexpr int SCREEN_H = 1080;
constexpr int TARGET_FPS = 60;


//  BÀN CỜ (gameplay)
constexpr int CELL_SIZE = 47;   


//  PIXEL FONT
constexpr int FONT_GLYPH_W = 5;   
constexpr int FONT_GLYPH_H = 7;    
constexpr int FONT_SPACING = 1;    

// Scale chữ thường dùng
constexpr int FONT_SCALE_SM = 3;    // thông tin nhỏ
constexpr int FONT_SCALE_MD = 4;    // tên player, HUD
constexpr int FONT_SCALE_LG = 6;    // menu item bình thường
constexpr int FONT_SCALE_XL = 8;    // tiêu đề màn hình


//  NÚT BACK 
constexpr int BTN_BACK_X = 30;
constexpr int BTN_BACK_Y = 30;
constexpr int BTN_MAX_W = 180;  


//  NÚT SETTINGS 
constexpr int BTN_SETTINGS_MARGIN = 30;   
constexpr int BTN_SETTINGS_OFFSET = 210; 

constexpr int BTN_BACK_W = 180;
constexpr int BTN_BACK_H = 60;


//  GAMEPLAY HUD
constexpr int HUD_Y = 68;    
constexpr int HUD_PLAYER1_X = 350;   
constexpr int HUD_PLAYER2_X = 1100; 


//  LOAD GAME SCREEN
constexpr float PANEL_W_RATIO = 0.60f; 
constexpr float PANEL_H_RATIO = 0.62f; 
constexpr int   PANEL_TITLE_Y = 90;  

#endif // CONSTANTS_H