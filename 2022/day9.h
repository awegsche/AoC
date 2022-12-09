//
// Created by andiw on 09/12/2022.
//

#ifndef AOC_DAY9_H
#define AOC_DAY9_H

#include <vector>
#include <string>
#include <iostream>
#include <compare>
#include <set>
#include <array>

#include "AocDay.h"
#include "AocObject.h"

using std::cout, std::endl;

struct Position {
    int x = 0;
    int y = 0;

    // do your magic
    auto operator<=>(Position const &) const = default;

    auto operator-(Position const &other) const -> Position {
        return Position{x - other.x, y - other.y};
    }

    auto operator+=(Position const &other) {
        x += other.x;
        y += other.y;
    }

    auto follow(Position const &other) {
        Position diff = other - *this;

        int signx = diff.x < 0 ? -1 : 1;
        int signy = diff.y < 0 ? -1 : 1;

        int absx = signx * diff.x;
        int absy = signy * diff.y;

        int abs = absx + absy;

        if (abs >= 3) {
            this->x += absx ? signx : 0;
            this->y += absy ? signy : 0;
        } else if (abs == 2) {
            this->x += diff.x ? diff.x - signx : 0;
            this->y += diff.y ? diff.y - signy : 0;
        }
    }
};

char sharp(bool cond, char sc = '#') {
    return cond ? sc : '.';
}

template<int DIM>
void print_positions(std::set<Position> const &pos) {
    for (int y = -DIM; y < DIM; ++y) {
        for (int x = -DIM; x < DIM; ++x) {
            cout << sharp(pos.contains({x, y}));
        }
        cout << endl;
    }
}

template<int DIM, int N>
void print_rope(std::array<Position, N> const &rope) {
    for (int y = -DIM; y < DIM; ++y) {
        for (int x = -DIM; x < DIM; ++x) {
            cout << sharp(std::find(rope.begin(), rope.end(), Position{x, y}) != rope.end());
        }
        cout << endl;
    }
}

std::ostream &operator<<(std::ostream &os, Position const &p) {
    return os << "(" << p.x << "," << p.y << ")";
}

struct Movement {
    char dir = 'L';
    int distance = 0;

    template<size_t N>
    auto move(std::array<Position, N> &rope, std::set<Position> &touched) const {
        Position step;

        switch (dir) {
            case 'L':
                step.x = -1;
                break;
            case 'U':
                step.y = -1;
                break;
            case 'R':
                step.x = 1;
                break;
            case 'D':
                step.y = 1;
                break;
        }

        for (int i = 0; i < distance; ++i) {
            rope[0] += step;
            for (int s = 0; s < N - 1; ++s) {
                rope[s + 1].follow(rope[s]);
            }
            touched.insert(rope.back());
        }
    }
};

class Day9 : public aoc::AocObject<Day9>, public aoc::AocDay<Day9, size_t> {
    std::vector<Movement> m_movements;

public:
    static constexpr char FILENAME[] = "9";
    static constexpr char YEAR[] = "2022";
    static constexpr char TITLE[] = "Rope Bridge";

    static auto get_object(std::istream &stream, Day9 &day) -> bool {
        day.m_movements.clear();

        std::string line{};
        Movement m{};
        while (stream >> m.dir) {

            stream >> m.distance;
            day.m_movements.push_back(m);
        }
        return true;
    }

    template<size_t N>
    size_t part_n() const {
        std::array<Position, N> rope{};
        std::set<Position> touched;

        for (auto const &m: m_movements) {
            m.move(rope, touched);
        }
        return touched.size();
    }

    size_t part1() const override {
        return part_n<2>();
    }

    size_t part2() const override {
        return part_n<10>();
    }
};

#endif //AOC_DAY9_H
