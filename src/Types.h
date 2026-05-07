#pragma once
#include <string>
#include <vector>
#include "GameState.h"
#include "Constants.h"
#include "Sound.h"
#include "lang.h"

using namespace std;

enum BGMType {
    BGM_NONE,
    BGM_MENU,
    BGM_INGAME
};

enum UIScreen
{
    SCREEN_MENU,
    SCREEN_MODE_SELECTION,
    SCREEN_PLAYER_SETUP,
    SCREEN_GAMEPLAY,
    SCREEN_SETTINGS,
    SCREEN_HELP,
    SCREEN_ABOUT,
    SCREEN_LOAD_GAME,
    SCREEN_DIFFICULTY
};

struct PlayerInfo
{
    string name;
    int character = 0;    // 0=knight 1=mage 2=archer
    int moveCount = 0;    // tong so nuoc da danh
    int winCount = 0;    // so van thang

    // Reset match-specific data (called between games)
    void ResetMatchData()
    {
        moveCount = 0;
    }

    // Reset all player data
    void ResetAll()
    {
        name.clear();
        character = 0;
        moveCount = 0;
        winCount = 0;
    }
};

struct AppContext
{
    GameState   gameState;
    int         cursorX = 7;
    int         cursorY = 7;
    UIScreen    screen = SCREEN_MENU;
    UIScreen    prevScreen = SCREEN_MENU;
    PlayerInfo  player1;
    PlayerInfo  player2;
    bool        playWithBot = false;
    int         setupStage = 0;
    int         selectedChar = 0;
    string currentInput;
    int         selectedItem = 0;
    int         selectedMode = 0;
    float       turnTimer = TURN_TIME;

    // Save / Load dialog
    enum class SaveLoadMode { NONE, SAVING, LOADING } saveLoadMode = SaveLoadMode::NONE;
    string saveLoadInput;
    string saveLoadMsg;
    float       saveLoadMsgTimer = 0.0f;

    bool        enterGuard = false;  
    int         difficulty = 1;       

    // Load Game screen
    int         loadSelectedIndex = 0;
    vector<string> saveFileList;

    // Music
    SoundBank* sound;
    BGMType curBGM;
    bool hasPlayedWinsfx = false;

    // Volumn của nhạc
    float musicVolume;
    float sfxVolume;

    // Ngôn ngữ
    LanguageBank language;
    const map<string,string>* curLanguage;

    AppContext()
        : gameState()
        , cursorX(7)
        , cursorY(7)
        , screen(SCREEN_MENU)
        , prevScreen(SCREEN_MENU)
        , playWithBot(false)
        , setupStage(0)
        , selectedChar(0)
        , currentInput()
        , selectedItem(0)
        , selectedMode(0)
        , turnTimer(TURN_TIME)
        , saveLoadMode(SaveLoadMode::NONE)
        , saveLoadInput()
        , saveLoadMsg()
        , saveLoadMsgTimer(0.0f)
        , enterGuard(false)
        , difficulty(1)
        , loadSelectedIndex(0)
        , saveFileList()
        , musicVolume(0.2f)
        , sfxVolume(0.2f)
        , curLanguage(&language.langEn)
    {
    }
};