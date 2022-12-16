#include <compare>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>

#ifndef AOC_CPP_POS2D_H
#define AOC_CPP_POS2D_H

namespace aoc {

/// 2-dimensional position vector.
///
/// Coordinates are `int`s. This should be enough for all AoC purposes
struct Pos2D {
    int x = 0;
    int y = 0;

    Pos2D() : x(0), y(0) {}

    explicit Pos2D(int a) : x(a), y(a) {}

    Pos2D(int x, int y) : x(x), y(y) {}

    // do your magic
    // Note: this should create some kind of lexicographical order
    auto operator<=>(Pos2D const &) const = default;

    [[nodiscard]] inline auto operator-(Pos2D const &other) const -> Pos2D {
        return Pos2D{x - other.x, y - other.y};
    }

    [[nodiscard]] inline auto operator+(Pos2D const &other) const -> Pos2D {
        return Pos2D{x + other.x, y + other.y};
    }

    inline auto operator+=(Pos2D const &other) -> Pos2D & {
        x += other.x;
        y += other.y;
        return *this;
    }

    inline auto operator-=(Pos2D const &other) -> Pos2D & {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    /// returns the manhatten distance `|x| + |y|`
    [[nodiscard]] inline auto abs_manhatten() const -> int {
        return std::abs(x) + std::abs(y);
    }
};

} // namespace aoc

template <> struct std::hash<aoc::Pos2D> {
    size_t operator()(aoc::Pos2D const &p) const noexcept {
        return static_cast<int64_t>(p.x) | static_cast<int64_t>(p.y) << 32;
    }
};

std::ostream &operator<<(std::ostream &os, aoc::Pos2D const &p);

#endif
