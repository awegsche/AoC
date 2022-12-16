#ifndef AOC_DAY16_H
#define AOC_DAY16_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <re2.h>

#include <AocDay.h>
#include <AocObject.h>

using std::cout, std::endl;

struct Valve {
    std::vector<std::string> connections;
    int flow_rate;

};

std::ostream& operator<<(std::ostream& os, Valve const& v) {
    os << "(" << v.flow_rate << ") [";
    for (auto const& c: v.connections)
        os << c << " ";
    return os << "]";
}

class Day16 : public aoc::AocObject<Day16>,
              public aoc::AocDay<Day16, uint64_t> {

    std::unordered_map<std::string, Valve> m_valves;

  public:
    static constexpr char FILENAME[] = "16";
    static constexpr char YEAR[]     = "2022";
    static constexpr char TITLE[]    = "Valves";

    static auto get_object(std::istream &stream, Day16 &day) -> bool {
        static const re2::RE2 RE_VALVE{R"(Valve (\w+) has flow rate=(\d+); tunnels? leads? to valves? )"};
        static const re2::RE2 RE_CONN{R"((\w+)(, )?)"};

        std::string line{};

        while(std::getline(stream, line)) {
            std::string name{};
            Valve valve{};
            re2::StringPiece sp_line{line};
            re2::RE2::Consume(&sp_line, RE_VALVE, &name, &valve.flow_rate);

            cout << "adding valve " << name << endl;
            std::string conn{};
            while (re2::RE2::Consume(&sp_line, RE_CONN, &conn)) {
                valve.connections.push_back(conn);
            }

            day.m_valves.insert({name, valve});
        }

        for (auto const &[name, valve]: day.m_valves) {
            cout << name << ": " << valve << endl;
        }

        return true;
    }

    auto part1() const -> uint64_t override {
        return ;
    }

    auto part2() const -> uint64_t override {
        return 0;
    }
};

#endif
