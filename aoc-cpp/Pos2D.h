#include <compare>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>

#ifndef AOC_CPP_POS2D_H
#define AOC_CPP_POS2D_H

namespace aoc {

    struct Pos2D {
        int x = 0;
        int y = 0;

        // do your magic
        auto operator<=>(Pos2D const &) const = default;

        inline auto operator-(Pos2D const &other) const -> Pos2D {
            return Pos2D{x - other.x, y - other.y};
        }
        inline auto operator+(Pos2D const &other) const -> Pos2D {
            return Pos2D{x + other.x, y + other.y};
        }

        inline auto operator+=(Pos2D const &other) {
            x += other.x;
            y += other.y;
        }
    };

}

template<>
struct std::hash<aoc::Pos2D> {
    size_t operator()(aoc::Pos2D const& p) const noexcept {
        return static_cast<int64_t>(p.x) | static_cast<int64_t>(p.y) << 32;
    }
};

std::ostream &operator<<(std::ostream &os, aoc::Pos2D const &p);

#endif
