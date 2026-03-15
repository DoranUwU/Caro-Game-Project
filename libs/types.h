#ifndef TYPES_H
#define TYPES_H
#include <string>

// ============================================================
//  CONSTANTS
// ============================================================
const int BOARD_SIZE = 12;

// ============================================================
//  ENUMS
// ============================================================
enum GameState
{
    MENU,
    MODE_SELECTION,
    PLAYER_SETUP,
    GAMEPLAY,
    SETTINGS,
    HELP,
    ABOUT
};

// ============================================================
//  STRUCTS
// ============================================================
struct Player
{
    std::string name;
    int character = 0;   // 0 = knight, 1 = mage, 2 = archer
};

struct AppContext
{
    // --- Board ---
    int  board[BOARD_SIZE][BOARD_SIZE] = {};
    int  currentPlayer = 1;
    int  cursorX = 0;
    int  cursorY = 0;

    // --- State machine ---
    GameState state = MENU;
    GameState prevState = MENU;

    // --- Players ---
    Player player1;
    Player player2;
    bool   playWithBot = false;

    // --- Setup flow ---
    int         setupStage = 0;
    int         selectedChar = 0;
    std::string currentInput;

    // --- Menu selection ---
    int selectedItem = 0;
    int selectedMode = 0;   // 0 = PVP, 1 = PVE
    int selectedModeItem = 0;
};

#endif // TYPES_H