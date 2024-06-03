#include "../../cmake-build-debug/_deps/raylib-src/src/raylib.h"

#include <compare>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>

// #include <glm/vec2.hpp>

#ifndef AOC_CPP_POS2D_H
#define AOC_CPP_POS2D_H

#include <fmttools.h>
#include <format>

namespace aoc {

/// 2-dimensional position vector.
///
/// Coordinates are `int`s. This should be enough for all AoC purposes
struct Pos2D {
    int x = 0;
    int y = 0;

    Pos2D() : x(0), y(0) {}

    explicit Pos2D(int a) : x(a), y(a) {}

    // Pos2D(glm::vec2 const &other) : x(other.x), y(other.y) {}

    Pos2D(int x, int y) : x(x), y(y) {}

    // do your magic
    // Note: this should create some kind of lexicographical order
    auto operator<=>(Pos2D const &) const = default;

    [[nodiscard]] auto operator-() const -> Pos2D { return Pos2D{-x, -y}; }

    [[nodiscard]] inline auto operator-(Pos2D const &other) const -> Pos2D {
        return Pos2D{x - other.x, y - other.y};
    }

    [[nodiscard]] inline auto operator+(Pos2D const &other) const -> Pos2D {
        return Pos2D{x + other.x, y + other.y};
    }

    auto operator+=(Pos2D const &other) -> Pos2D & {
        x += other.x;
        y += other.y;
        return *this;
    }

    auto operator-=(Pos2D const &other) -> Pos2D & {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    auto operator*(int a) const -> Pos2D { return Pos2D{x * a, y * a}; }

    /// returns the manhatten distance `|x| + |y|`
    [[nodiscard]] inline auto abs_manhatten() const -> int {
        return std::abs(x) + std::abs(y);
    }
};

auto format_value(Pos2D const &pos, auto fmt) {
    return std::format_to(fmt, "({}, {})", pos.x, pos.y);
}

} // namespace aoc

template <> struct std::hash<aoc::Pos2D> {
    size_t operator()(aoc::Pos2D const &p) const noexcept {
        return static_cast<int64_t>(p.x) | static_cast<int64_t>(p.y) << 32;
    }
};

std::ostream &operator<<(std::ostream &os, aoc::Pos2D const &p);

namespace aoc {
struct Polar2D {
    float angle;
    float mag;

    Polar2D(float angle, float mag) : angle(angle), mag(mag) {}

    explicit Polar2D(const Pos2D pos)
        : angle(2.0f * PI - std::atan2(pos.x, pos.y)),
          mag(std::sqrt(pos.x * pos.x + pos.y * pos.y)) {}

    [[nodiscard]] Pos2D to_pos2d() const {
        return Pos2D{
            static_cast<int>(mag * std::cos(angle)),
            static_cast<int>(mag * std::sin(angle)),
        };
    }
};
} // namespace aoc

#endif
