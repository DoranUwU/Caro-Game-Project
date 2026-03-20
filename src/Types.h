#pragma once
#include <string>
#include "GameState.h"

enum UIScreen
{
    SCREEN_MENU,
    SCREEN_LOADGAME,
    SCREEN_MODE_SELECTION,
    SCREEN_PLAYER_SETUP,
    SCREEN_GAMEPLAY,
    SCREEN_SETTINGS,
    SCREEN_HELP,
    SCREEN_ABOUT
};

struct PlayerInfo
{
    std::string name;
    int character = 0;   // 0 = knight, 1 = mage, 2 = archer
};

struct AppContext
{
    // Game logic — Board + currentPlayer + status nằm trong GameState
    GameState gameState;

    // Cursor UI
    int cursorX = 7;
    int cursorY = 7;

    // State machine màn hình
    UIScreen screen = SCREEN_MENU;
    UIScreen prevScreen = SCREEN_MENU;

    // Thông tin hiển thị player
    PlayerInfo player1;
    PlayerInfo player2;
    bool       playWithBot = false;

    // Setup flow
    int         setupStage = 0;
    int         selectedChar = 0;
    std::string currentInput;

    // Menu selection
    int selectedItem = 0;
    int selectedMode = 0;
};