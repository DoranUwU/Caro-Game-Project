#pragma once
#include "GameState.h"
#include "Save.h"
#include "Load.h"
#include "headers.h"
#include <mutex>
#include <sstream>
#include <string>
using namespace std;

inline void printBoard(const Board& board) {
    cout << "   ";
    for (int c = 0; c < BOARD_SIZE; ++c) {
        if (c < 10) cout << c << "  ";
        else cout << c << " ";
    }
    cout << "\n";

    for (int r = 0; r < BOARD_SIZE; ++r) {
        if (r < 10) cout << r << "  ";
        else cout << r << " ";

        for (int c = 0; c < BOARD_SIZE; ++c) {
            Player p = getPlayerAt(board, Position(r, c));
            if (p == Player::NONE) cout << ".  ";
            else if (p == Player::PlayerX) cout << "X  ";
            else if (p == Player::PlayerO) cout << "O  ";
        }
        cout << "\n";
    }
    cout << "----------------------------------------------\n";
}

inline Position getMoveFromUI() {
    int r, c;
    cout << "Enter row and column (e.g. 7 7): ";
    cin >> r >> c;
    return Position(r, c); // Placeholder for UI input, to be implemented later
}

inline bool tryParseMove(const string& input, Position& outMove) {
    istringstream iss(input);
    int row = -1;
    int column = -1;
    char extra = '\0';

    if (!(iss >> row >> column)) {
        return false;
    }
    if (iss >> extra) {
        return false;
    }

    outMove = Position(row, column);
    return true;
}

inline void writeSharedState(GameState* sharedState, std::mutex* sharedMutex, const GameState& value) {
    if (sharedState == nullptr || sharedMutex == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(*sharedMutex);
    *sharedState = value;
}

inline void readSharedState(GameState* sharedState, std::mutex* sharedMutex, GameState& outValue) {
    if (sharedState == nullptr || sharedMutex == nullptr) {
        return;
    }
    std::lock_guard<std::mutex> lock(*sharedMutex);
    outValue = *sharedState;
}

inline GameState runTest(
    const GameState& initialState = GameState(),
    const char* saveFile = "saves/current_game.json",
    GameState* sharedState = nullptr,
    std::mutex* sharedMutex = nullptr
) {
    GameState gameState = initialState;
    writeSharedState(sharedState, sharedMutex, gameState);

    cout << "--- GOMOKU TEST STARTED ---\n";
    cout << "Commands: <row col> | save | load | quit\n";
    printBoard(gameState.board);

    // Note: Make sure GameStatus::ONGOING matches whatever you named it in GameState.h!
    while (gameState.status == GameStatus::ONGOING) {
        readSharedState(sharedState, sharedMutex, gameState);

        cout << "\nCurrent Turn: "
            << (gameState.currentPlayer == Player::PlayerX? "BLACK (X)" : "WHITE (O)") << "\n";
        cout << "Enter command: ";

        string input;
        if (!getline(cin >> ws, input)) {
            cout << ">> INPUT CLOSED. STOP GAME.\n";
            break;
        }

        if (input == "save") {
            if (saveGameState(gameState, saveFile)) {
                cout << ">> SAVED: " << saveFile << "\n";
            }
            else {
                cout << ">> SAVE FAILED!\n";
            }
            continue;
        }

        if (input == "load") {
            GameState loadedState;
            if (loadGameState(saveFile, loadedState)) {
                gameState = loadedState;
                writeSharedState(sharedState, sharedMutex, gameState);
                cout << ">> LOADED: " << saveFile << "\n";
                printBoard(gameState.board);
            }
            else {
                cout << ">> LOAD FAILED! File missing or invalid.\n";
            }
            continue;
        }

        if (input == "quit") {
            cout << ">> EXIT CURRENT GAME.\n";
            break;
        }

        Position move;
        if (!tryParseMove(input, move)) {
            cout << ">> INVALID INPUT! Use: row col | save | load | quit\n";
            continue;
        }

        GameState nextState = playMove(gameState, move);

        // If the turn didn't change, playMove rejected the move!
        if (nextState.currentPlayer == gameState.currentPlayer) {
            cout << ">> INVALID MOVE! Spot is taken or out of bounds. Try again.\n";
        }
        else {
            // Move was valid, update the reality and print the new board
            gameState = nextState;
            writeSharedState(sharedState, sharedMutex, gameState);
            printBoard(gameState.board);
        }
    }

    // The loop broke! Let's print the final result.
    if (gameState.status == GameStatus::WIN_X) {
        cout << "\n*** BLACK WINS! ***\n";
    }
    else if (gameState.status == GameStatus::WIN_O) {
        cout << "\n*** WHITE WINS! ***\n";
    }
    else {
        cout << "\n*** IT'S A DRAW! ***\n";
    }
    writeSharedState(sharedState, sharedMutex, gameState);
    return gameState;
}

inline void run() {
    GameState gameState;

    while (gameState.status == GameStatus::ONGOING) {
        Position move = getMoveFromUI();
        gameState = playMove(gameState, move);
    }
}   