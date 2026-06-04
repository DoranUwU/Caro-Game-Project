#include "GameLogic.h"

int countConsecutive(const Board& board, const Position& startPos, const Position& direction, Player player) {
    int count = 0;
    Position currentPos = startPos + direction;
    while (!isOutsideBound(currentPos) && getPlayerAt(board, currentPos) == player) {
        count++;
        currentPos = currentPos + direction;
    }
    return count;
}

bool checkWin(const Board& board, const Position& pos, Player player) {
    if (player == Player::NONE) return false;

    if (countConsecutive(board, pos, Direction::North, player) + countConsecutive(board, pos, Direction::South, player) + 1 >= WIN_CONDITION) {
        return true;
    }
    if (countConsecutive(board, pos, Direction::East, player) + countConsecutive(board, pos, Direction::West, player) + 1 >= WIN_CONDITION) {
        return true;
    }
    if (countConsecutive(board, pos, Direction::Northeast, player) + countConsecutive(board, pos, Direction::Southwest, player) + 1 >= WIN_CONDITION) {
        return true;
    }
    if (countConsecutive(board, pos, Direction::Northwest, player) + countConsecutive(board, pos, Direction::Southeast, player) + 1 >= WIN_CONDITION) {
        return true;
    }
    return false;
}

// Tìm đường thắng và lưu vào winLine[], winLineCount
// Trả về số ô trong đường thắng (0 nếu không tìm thấy)
int findWinLine(const Board& board, const Position& pos, Player player, Position outLine[], int maxLen) {
    if (player == Player::NONE) return 0;

    // 4 hướng: dọc, ngang, chéo chính, chéo phụ
    const Position dirs[4][2] = {
        { Direction::North, Direction::South },
        { Direction::East,  Direction::West  },
        { Direction::Northeast, Direction::Southwest },
        { Direction::Northwest, Direction::Southeast }
    };

    for (int d = 0; d < 4; d++) {
        int countFwd = countConsecutive(board, pos, dirs[d][0], player);
        int countBwd = countConsecutive(board, pos, dirs[d][1], player);
        int total = countFwd + countBwd + 1;

        if (total >= WIN_CONDITION) {
            int idx = 0;
            // Thêm các ô theo hướng ngược (backward) trước
            for (int i = countBwd; i >= 1 && idx < maxLen; i--) {
                outLine[idx++] = pos + dirs[d][1] * i;
            }
            // Thêm ô trung tâm
            if (idx < maxLen) outLine[idx++] = pos;
            // Thêm các ô theo hướng thuận (forward)
            for (int i = 1; i <= countFwd && idx < maxLen; i++) {
                outLine[idx++] = pos + dirs[d][0] * i;
            }
            return idx;
        }
    }
    return 0;
}

bool isBoardFull(const Board& board) {
    for (int i = 0; i < TOTAL_CELLS; i++) {
        if (board.cell[i] == Player::NONE) {
            return false;
        }
    }
    return true;
}

GameState playMove(const GameState& currState, const Position& movePos) {
    if (currState.status != GameStatus::ONGOING || !isValidMove(currState.board, movePos)) {
        return currState;
    }

    GameState nextState;
    nextState.board = makeMove(currState.board, movePos, currState.currentPlayer);
    nextState.currentPlayer = getOpponent(currState.currentPlayer);
    nextState.lastMove = movePos;
    nextState.winLineCount = 0;

    if (checkWin(nextState.board, movePos, currState.currentPlayer)) {
        nextState.status = (currState.currentPlayer == Player::PlayerX) ? GameStatus::WIN_X : GameStatus::WIN_O;
        // Tìm và lưu đường thắng
        nextState.winLineCount = findWinLine(nextState.board, movePos, currState.currentPlayer,
                                              nextState.winLine, MAX_WIN_LINE);
    }
    else if (isBoardFull(nextState.board)) {
        nextState.status = GameStatus::DRAW;
    }
    else {
        nextState.status = GameStatus::ONGOING;
    }

    return nextState;
}
