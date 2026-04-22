#include "AISearch.h"
#include "AIUtils.h"
#include <stdlib.h>
#include <time.h>

//Chặn bot xét nước đi ở các ô quá xa bên ngoài
MoveList getRelevantMoves(const GameState& state) {
    srand(time(nullptr)); //gen nước đi
    MoveList l;
    bool occupied = false;

    bool adj[BOARD_SIZE][BOARD_SIZE] = { false };

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            Position currPos(r, c);

            if (!isCellEmpty(state.board, currPos)) {
                occupied = true;

                for (int temp_r = -2; temp_r <= 2; temp_r++) {
                    for (int temp_c = -2; temp_c <= 2; temp_c++) {
                        Position adjacent(r + temp_r, c + temp_c);

                        if (!isOutsideBound(adjacent) && isCellEmpty(state.board, adjacent)) {
                            adj[adjacent.row][adjacent.column] = true;
                        }
                    }
                }
            }
        }
    }

    //Nếu bàn cờ trống, chơi random ở trung tâm.
    if (!occupied) {
        int a = 14, b = 16;
        append(l, Position((rand() % (b - a + 1)) + a, (rand() % (b - a + 1)) + a));//random khoảng 3x3 trung tâm
        return l;
    }

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (adj[r][c]) {
                append(l, Position(r, c));
            }
        }
    }

    return l;
}

/*
Sort điểm của nước đi để chạy alpha-beta prunning hiệu quả 
-> attack(tạo quân liên tiếp) và defence move(block đối thủ)
*/
MoveList getOrderedMoves(const GameState& state) {
    MoveList filtered = getRelevantMoves(state);

    ScoredMove sm[BOARD_SIZE * BOARD_SIZE];

    int count = filtered.count;
    int colourMul = state.currentPlayer == Player::PlayerX ? 1 : -1;

    for (int i = 0; i < count; i++) {
        Position move = filtered.list[i];

        GameState nextState = playMove(state, move);
        int score = colourMul * evalBoard(nextState);

        sm[i] = {move, score};
    }

    //insertion sort
    for (int i = 1; i < count; i++) {
        ScoredMove k = sm[i];
        int j = i - 1;

        while (j >= 0 && sm[j].score > k.score) {
            sm[j + 1] = sm[j];
            j--;
        }

        sm[j + 1] = k;
    }

    MoveList ordered;
    for (int i = 0; i < count; i++) {
        append(ordered, sm[i].pos);
    }

    return ordered;
}

int negamax(const GameState& state, int depth, int alpha, int beta, int colourMul) {
    if (depth == 0 || state.status != GameStatus::ONGOING) {
        return colourMul * evalBoard(state);
    }

    int max = -INF;

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            Position move(r, c);

            if (isValidMove(state.board, move)) {
                GameState nextState = playMove(state, move);

                int score = -negamax(nextState, depth - 1, -beta, -alpha, -colourMul);
                max = std::max(max, score);

                alpha = std::max(alpha, score);

                if (alpha >= beta) return max;
            }
        }
    }

    return max;
}

int negamax_improved(const GameState& state, int depth, int alpha, int beta, int colourMul) {
    if (depth == 0 || state.status != GameStatus::ONGOING) {
        return colourMul * evalBoard(state);
    }

    int max = -INF;

    MoveList orderedMoves = getOrderedMoves(state);
    for (int i = 0; i < orderedMoves.count; i++) {
        Position move = orderedMoves.list[i];
        GameState nextState = playMove(state, move);
        int score = -negamax_improved(nextState, depth - 1, -beta, -alpha, -colourMul);

        max = std::max(max, score);
        alpha = std::max(alpha, score);

        if (alpha >= beta) return max;
    }
    return max;
}