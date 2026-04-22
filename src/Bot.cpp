#include "Bot.h"
#include "AISearch.h"

Position getBestMove(const GameState& state, int depth) {
    Position bestMove(-1, -1);
    int bestScore = -INF;

    int colourMul;
    if (state.currentPlayer == Player::PlayerX) colourMul = 1;
    else colourMul = -1;

    MoveList orderedMoves = getOrderedMoves(state);

    for (int i = 0; i < orderedMoves.count; i++) {
        Position move = orderedMoves.list[i];
        GameState nextState = playMove(state, move);
        int score = -negamax(nextState, depth - 1, -INF, INF, -colourMul);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    if (bestMove.row == -1 && orderedMoves.count > 0) {
        bestMove = orderedMoves.list[0];
    }

    return bestMove;
}