#pragma once
#include "GameState.h"
#include "headers.h"
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

inline void runTest() {
    GameState gameState;

    cout << "--- GOMOKU TEST STARTED ---\n";
    printBoard(gameState.board);

    // Note: Make sure GameStatus::ONGOING matches whatever you named it in GameState.h!
    while (gameState.status == GameStatus::ONGOING) {

        cout << "\nCurrent Turn: "
            << (gameState.currentPlayer == Player::PlayerX? "BLACK (X)" : "WHITE (O)") << "\n";

        Position move = getMoveFromUI();
        GameState nextState = playMove(gameState, move);

        // If the turn didn't change, playMove rejected the move!
        if (nextState.currentPlayer == gameState.currentPlayer) {
            cout << ">> INVALID MOVE! Spot is taken or out of bounds. Try again.\n";
        }
        else {
            // Move was valid, update the reality and print the new board
            gameState = nextState;
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
}

inline void run() {
    GameState gameState;

    while (gameState.status == GameStatus::ONGOING) {
        Position move = getMoveFromUI();
        gameState = playMove(gameState, move);
    }
}