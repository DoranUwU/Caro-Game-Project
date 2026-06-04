#include "Bot.h"
#include "AISearch.h"
#include "GameLogic.h"
#include "Position.h"

Position getBestMove(const GameState& state, int depth) {
    Position bestMove(-1, -1);

    int colourMul = (state.currentPlayer == Player::PlayerX) ? 1 : -1;
    for (int currDepth = 1; currDepth <= depth; currDepth++) {
        Position bestMoveAtCurrDepth(-1, -1);
        int bestScore = -INF;
        
        MoveList orderedMoves = getOrderedMoves(state);
        if (bestMove.row != -1) {
            for (int i = 0; i < orderedMoves.count; i++) {
                if (orderedMoves.list[i] == bestMove) {
                    Position temp = orderedMoves.list[0];
                    orderedMoves.list[0] = orderedMoves.list[i];
                    orderedMoves.list[i] = temp;
                    break;
                }
            }
        }

        for (int i = 0; i < orderedMoves.count; i++) {
          Position move = orderedMoves.list[i];
          GameState nextState = playMove(state, move);
          int score = -negamax_improved(nextState, currDepth - 1, -INF, INF, -colourMul);

          if (score > bestScore) {
            bestScore = score;
            bestMoveAtCurrDepth = move;
          }
        }

        bestMove = bestMoveAtCurrDepth;
    }
    
    return bestMove;
}