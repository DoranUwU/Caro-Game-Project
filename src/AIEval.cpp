#include "AIEval.h"
#include "math.h"


int getPatternScore(int consecutivePieces, int openEnds) {
    if (consecutivePieces >= 5) return 1e5; //thắng
    else if (consecutivePieces == 4) {
        if (openEnds == 2) return 1e4; //4 quân, mở 2 bên, hết cứu
        if (openEnds == 1) return 1e3; //4 quân, mở 1 bên, còn cứu được
    }
    else if (consecutivePieces == 3) {
        if (openEnds == 2) return 1e3; //3 quân, không block là hẹo
        if (openEnds == 1) return 1e2; //3 quân, block 1 bên, tạm ổn
    }
    else if (consecutivePieces == 2) {
        if (openEnds == 2) return 1e2; //mấy cái 2 quân này không đáng kể
        if (openEnds == 1) return 1e1;
    }
    if (consecutivePieces == 1) {
        if (openEnds == 2) return 1;
    }
    return 0;
}


// ---------------------------------------------------------------------------
// scoreThreat
// Counts the score of placing `player`'s piece hypothetically at `pos`.
// Scans all 4 axes (8 directions paired) just like evalBoard does for real
// pieces, but treats `pos` as if it already holds `player`.
// ---------------------------------------------------------------------------
int scoreThreat(const Board& board, Position pos, Player player) {
    static const Position dirs[4][2] = {
        { Direction::North,     Direction::South     },
        { Direction::East,      Direction::West      },
        { Direction::Northeast, Direction::Southwest },
        { Direction::Northwest, Direction::Southeast }
    };

    int total = 0;

    for (int i = 0; i < 4; i++) {
        // Count consecutive same-colour pieces along each axis,
        // treating `pos` as already occupied by `player`.
        int fwd = 0, bwd = 0;
        Position p;

        p = pos + dirs[i][0];
        while (!isOutsideBound(p) && getPlayerAt(board, p) == player) { fwd++; p = p + dirs[i][0]; }
        Position fwdEnd = p; // first cell past the run forward

        p = pos + dirs[i][1];
        while (!isOutsideBound(p) && getPlayerAt(board, p) == player) { bwd++; p = p + dirs[i][1]; }
        Position bwdEnd = p; // first cell past the run backward

        int consecutive = fwd + bwd + 1; // +1 for `pos` itself

        int openEnds = 0;
        if (!isOutsideBound(fwdEnd) && isCellEmpty(board, fwdEnd)) openEnds++;
        if (!isOutsideBound(bwdEnd) && isCellEmpty(board, bwdEnd)) openEnds++;

        total += getPatternScore(consecutive, openEnds);
    }

    return total;
}

int evalBoard(const GameState& state) {
    int total = 0;

    Position direction[4] = {
        Direction::South,
        Direction::East,
        Direction::Southeast,
        Direction::Southwest
    };

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            Position currPos(r, c);
            Player player = getPlayerAt(state.board, currPos);

            if (player == Player::NONE) continue; //skip if empty cell

            int centre = BOARD_SIZE / 2;
            int dist = std::abs(r - centre) + abs(c - centre);
            int posScore = 20 - dist;

            if (player == Player::PlayerX) {
                total += posScore;
            }
            else {
                total -= posScore;
            }

            for (int i = 0; i < 4; i++) {
                //find the piece behind curr piece
                Position prevPos(currPos.row - direction[i].row, currPos.column - direction[i].column);

                if (!isOutsideBound(prevPos) && getPlayerAt(state.board, prevPos) == player) continue;

                int count = 1;
                Position nextPos = currPos + direction[i];

                while (!isOutsideBound(nextPos) && getPlayerAt(state.board, nextPos) == player) {
                    count++;
                    nextPos = nextPos + direction[i];
                }

                int openEnds = 0;

                //check before/after the current line
                if (!isOutsideBound(prevPos) && getPlayerAt(state.board, prevPos) == Player::NONE) openEnds++;
                if (!isOutsideBound(nextPos) && getPlayerAt(state.board, nextPos) == Player::NONE) openEnds++;

                int s = getPatternScore(count, openEnds);

                // Weight the opponent's threats 1.5x so defence takes
                // priority over attack when scores are otherwise equal.
                if (player == Player::PlayerX) total += s;
                else if (player == Player::PlayerO) total -= (s + s / 2);
            }
        }
    }

    return total;
}