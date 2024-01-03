#ifndef AOC_DAY16_H
#define AOC_DAY16_H

#include "pretty.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <re2.h>

#include <AocDay.h>
#include <AocObject.h>

using std::cout, std::endl;

constexpr int TOTAL_TIME = 30;

struct ValveName {
    uint16_t name;

    ValveName() : name(0) {}

    ValveName(int i) : name(i & 0xFFFF) {}

    ValveName(char chars[2]) { name = chars[1] << 8 | chars[0]; }

    ValveName(const char *chars) { name = chars[1] << 8 | chars[0]; }

    operator bool() const { return name; }

    auto operator<=>(const ValveName other) const { return name <=> other.name; }
};

struct Valve {
    ValveName name;
    std::vector<ValveName> connections;
    int flow_rate;
};

struct CompactedValve {
    uint64_t connections;

    CompactedValve(Valve const &valve, std::vector<Valve> valves)
        : connections(0) {
        for (ValveName name : valve.connections) {
            for (size_t i = 0; i < valves.size(); ++i) {
                if (name == valves[i].name) {
                    connections |= 1 << i;
                }
            }
        }
    }
};

struct Action {
    enum Kind { Move, Open, None } kind;
    uint8_t valve_index;
};



std::ostream &operator<<(std::ostream &os, ValveName const &n) {
    return os << static_cast<char>(n.name & 0xFF)
              << static_cast<char>((n.name >> 8) & 8);
}

std::ostream &operator<<(std::ostream &os, Valve const &v) {
    os << "(" << v.flow_rate << ") [";
    for (auto const &c : v.connections)
        os << c << " ";
    return os << "]";
}

template <> struct std::hash<ValveName> {
    size_t operator()(ValveName const &n) const noexcept {
        return static_cast<size_t>(n.name);
    }
};

class Day16 : public aoc::AocObject<Day16>,
              public aoc::AocDay<Day16, uint64_t> {

    std::vector<Valve> m_valves;
    std::vector<CompactedValve> m_cvalves;

  public:
    static constexpr char FILENAME[] = "16";
    static constexpr char YEAR[]     = "2022";
    static constexpr char TITLE[]    = "Valves";

    static auto get_object(std::istream &stream, Day16 &day) -> bool {
        static const re2::RE2 RE_VALVE{
            R"(Valve (\w+) has flow rate=(\d+); tunnels? leads? to valves? )"};
        static const re2::RE2 RE_CONN{R"((\w+)(, )?)"};

        std::string line{};

        while (std::getline(stream, line)) {
            char name[2];
            Valve valve{};
            re2::StringPiece sp_line{line};
            re2::RE2::Consume(&sp_line, RE_VALVE, &name, &valve.flow_rate);

            cout << "adding valve " << name << endl;
            char conn[2];
            while (re2::RE2::Consume(&sp_line, RE_CONN, &conn)) {
                valve.connections.emplace_back(conn);
            }
            valve.name = name;

            day.m_valves.push_back(valve);
        }

        // Valve `AA` must be the first
        std::sort(day.m_valves.begin(), day.m_valves.end(),
            [](const Valve& a, const Valve& b) { return a.name < b.name; });

        for (auto const &valve : day.m_valves) {
            day.m_cvalves.emplace_back(valve, day.m_valves);
        }

        return true;
    }

    auto part1() const -> uint64_t override {
        // try breadth first search
        std::vector<PathSegment> tree;

        tree.emplace_back("AA", 0, 0, 0);

        while (!tree.empty()) {
        }

        return 0;
    }

    auto part2() const -> uint64_t override { return 0; }
};

#endif
