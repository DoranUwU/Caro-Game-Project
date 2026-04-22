#include "Bot.h"

Position getBestMove(const GameState& state, int depth) {
    Position bestMove(-1, -1);
    int bestScore = -INF;

    int colourMul;
    if (state.currentPlayer == Player::PlayerX) colourMul = 1;
    else colourMul = -1;

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            Position move(r, c);

            if (isValidMove(state.board, move)) {
                GameState nextState = playMove(state, move);
                int score = -negamax(nextState, depth - 1, -INF, INF, colourMul);

                if (score > bestScore) {
                    bestScore = score;
                    bestMove = move;
                }
            }
        }
    }

    return bestMove;
}