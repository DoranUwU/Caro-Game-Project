#include "GameState.h"

struct MoveList {
    Position list[BOARD_SIZE * BOARD_SIZE];
    int count = 0;
};

void append(MoveList& list, const Position& pos);

struct ScoredMove {
    Position pos;
    int score;

    constexpr ScoredMove() : score(0) {}
    constexpr ScoredMove(const Position& p, int s) : pos(p), score(s) {}
};
