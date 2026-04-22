#ifndef POSITION_H
#define POSITION_H

struct Position {
    int row;
    int column;

    constexpr Position() : row(0), column(0) {}
    constexpr Position(int r, int c) : row(r), column(c) {}
};

constexpr inline bool operator==(const Position& lhs, const Position& rhs) {
    return lhs.row == rhs.row && lhs.column == rhs.column;
}

constexpr inline bool operator!=(const Position& lhs, const Position& rhs) {
    return !(lhs == rhs);
}

constexpr inline Position operator+(const Position& lhs, const Position& rhs) {
    return Position(lhs.row + rhs.row, lhs.column + rhs.column);
}

constexpr inline Position operator*(const Position& pos, int scalar) {
    return Position(pos.row * scalar, pos.column * scalar);
}

namespace Direction {
    constexpr Position North = Position(-1, 0);
    constexpr Position South = Position(1, 0);
    constexpr Position East = Position(0, 1);
    constexpr Position West = Position(0, -1);
    constexpr Position Northeast = North + East;
    constexpr Position Northwest = North + West;
    constexpr Position Southeast = South + East;
    constexpr Position Southwest = South + West;
} // namespace Direction

#endif // POSITION_H