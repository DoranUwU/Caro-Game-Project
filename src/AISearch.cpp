#include "AISearch.h"

using namespace std;

//Chặn bot xét nước đi ở các ô quá xa bên ngoài
MoveList getRelevantMoves(const GameState& state) {
    MoveList l;
    bool occupied = false;

    bool adj[BOARD_SIZE][BOARD_SIZE] = { false };

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            Position currPos(r, c);

            if (!isCellEmpty(state.board, currPos)) {
                occupied = true;

                for (int temp_r = -1; temp_r <= 1; temp_r++) {
                    for (int temp_c = -1; temp_c <= 1; temp_c++) {
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
        append(l, Position(BOARD_SIZE / 2, BOARD_SIZE / 2));
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
#include <cmath> // Cần thiết để dùng hàm trị tuyệt đối abs

MoveList getOrderedMoves(const GameState& state) {
    MoveList rawMoves = getRelevantMoves(state);

    ScoredMove scoredMoves[BOARD_SIZE * BOARD_SIZE];
    int count = rawMoves.count;

    // 1. Chấm điểm các nước đi dựa trên mật độ cờ xung quanh
    for (int i = 0; i < count; ++i) {
        Position move = rawMoves.list[i];

        // Gọi hàm đánh giá cục bộ thay vì tính khoảng cách tới tâm
        int score = evaluateMove(state, move);

        scoredMoves[i] = { move, score };
    }

    // 2. Sắp xếp giảm dần theo điểm số (Insertion Sort)
    for (int i = 1; i < count; ++i) {
        ScoredMove key = scoredMoves[i];
        int j = i - 1;

        while (j >= 0 && scoredMoves[j].score < key.score) {
            scoredMoves[j + 1] = scoredMoves[j];
            j--;
        }

        scoredMoves[j + 1] = key;
    }

    // 3. Trả về danh sách đã sắp xếp
    MoveList orderedMoves;

    int MAX_MOVES_TO_CHECK = 20; 
    int limit = (count < MAX_MOVES_TO_CHECK) ? count : MAX_MOVES_TO_CHECK;

    for (int i = 0; i < limit; ++i) {
        append(orderedMoves, scoredMoves[i].pos);
    }

    return orderedMoves;
}

int negamax(const GameState& state, int depth, int alpha, int beta, int colourMul) {
    if (depth == 0 || state.status != GameStatus::ONGOING) {
        return colourMul * evalBoard(state);
    }

    int MAX = -INF;

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            Position move(r, c);

            if (isValidMove(state.board, move)) {
                GameState nextState = playMove(state, move);

                int score = -negamax(nextState, depth - 1, -beta, -alpha, -colourMul);
                MAX = max(MAX, score);

                alpha = max(alpha, score);

                if (alpha >= beta) return MAX;
            }
        }
    }

    return MAX;
}

int negamax_improved(const GameState& state, int depth, int alpha, int beta, int colourMul) {
    if (depth == 0 || state.status != GameStatus::ONGOING) {
        return colourMul * evalBoard(state);
    }

    int MAX = -INF;

    MoveList orderedMoves = getOrderedMoves(state);
    for (int i = 0; i < orderedMoves.count; i++) {
        Position move = orderedMoves.list[i];
        GameState nextState = playMove(state, move);
        int score = -negamax_improved(nextState, depth - 1, -beta, -alpha, -colourMul);

        MAX = max(MAX, score);
        alpha = max(alpha, score);

        if (alpha >= beta) return MAX;
    }
    return MAX;
}

int evaluateMove(const GameState& state, Position pos) {
    Player self     = state.currentPlayer;
    Player opponent = (self == Player::PlayerX) ? Player::PlayerO : Player::PlayerX;

    int attackScore  = scoreThreat(state.board, pos, self);
    int defenceScore = scoreThreat(state.board, pos, opponent);

    // Defence weighted 1.5x: prefer blocking over building when close in value. This score CANNOT be negative.
    return attackScore + defenceScore + defenceScore / 2;
}
