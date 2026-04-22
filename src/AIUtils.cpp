#include "AIUtils.h"

void append(MoveList& list, const Position& pos) {
    if (list.count < (BOARD_SIZE * BOARD_SIZE)) {
        list.list[list.count] = pos;
        list.count++;
    }
}