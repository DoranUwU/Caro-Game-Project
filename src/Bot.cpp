#include "Bot.h"
#include "AISearch.h"
#include "GameLogic.h"
#include "AIEval.h"
#include "Position.h"

// Threshold at which a pattern score means "can win / must block"
// getPatternScore(4, 1) == 1000, getPatternScore(4, 2) == 10000
static const int THREAT_WIN      = 90000; // >= 5-in-a-row (win)
static const int THREAT_OPEN4    =  9000; // open-4 or blocked-4 – must act now

Position getBestMove(const GameState& state, int depth) {
    Player self     = state.currentPlayer;
    Player opponent = (self == Player::PlayerX) ? Player::PlayerO : Player::PlayerX;

    MoveList candidates = getOrderedMoves(state);

    // ------------------------------------------------------------------
    // Pass 1 – Immediate win: can the bot win right now?
    // ------------------------------------------------------------------
    for (int i = 0; i < candidates.count; i++) {
        Position move = candidates.list[i];
        if (scoreThreat(state.board, move, self) >= THREAT_WIN)
            return move;
    }

    // ------------------------------------------------------------------
    // Pass 2 – Immediate block: would the opponent win on the next move?
    // ------------------------------------------------------------------
    for (int i = 0; i < candidates.count; i++) {
        Position move = candidates.list[i];
        if (scoreThreat(state.board, move, opponent) >= THREAT_WIN)
            return move;
    }

    // ------------------------------------------------------------------
    // Pass 3 – Urgent threat: block any open-4 or winning-threat pattern
    //           before spending time on deep search.
    //           (Handles cases like opponent having an open-3 that becomes
    //            an open-4 on their next turn if not blocked now.)
    // ------------------------------------------------------------------
    for (int i = 0; i < candidates.count; i++) {
        Position move = candidates.list[i];
        if (scoreThreat(state.board, move, opponent) >= THREAT_OPEN4)
            return move;
    }

    // ------------------------------------------------------------------
    // Pass 4 – Iterative-deepening negamax with alpha-beta + move ordering
    // ------------------------------------------------------------------
    Position bestMove(-1, -1);
    int colourMul = (self == Player::PlayerX) ? 1 : -1;

    for (int currDepth = 1; currDepth <= depth; currDepth++) {
        Position bestMoveAtCurrDepth(-1, -1);
        int bestScore = -INF;

        // Re-fetch ordered moves each iteration; front-load the best move
        // found in the previous depth so alpha-beta prunes more aggressively.
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