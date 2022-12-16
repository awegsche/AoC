

#ifndef AOC_DAY15_H
#define AOC_DAY15_H

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include <re2.h>

#include <AocDay.h>
#include <AocObject.h>
#include <Pos2D.h>

using aoc::Pos2D;
using std::cout, std::cerr, std::endl;

constexpr int RUN_LINE            = 2000000;
constexpr int TEST_LINE           = 10;
constexpr uint64_t MAX_COORD      = 4000000;
constexpr uint64_t MAX_COORD_TEST = 20;

struct Sensor {
    Pos2D position;
    Pos2D nearest;

    Sensor(std::string const &line) {
        static const re2::RE2 RE_SENSOR{
            R"(Sensor at x=(-?\d+), y=(-?\d+): closest beacon is at x=(-?\d+), y=(-?\d+))"};
        re2::RE2::FullMatch(line, RE_SENSOR, &position.x, &position.y,
                            &nearest.x, &nearest.y);
    }
};

class Day15 : public aoc::AocObject<Day15>,
              public aoc::AocDay<Day15, uint64_t> {

    std::vector<Sensor> m_sensors;

  public:
    static constexpr char FILENAME[] = "15";
    static constexpr char YEAR[]     = "2022";
    static constexpr char TITLE[]    = "Beacon Exclusion Zone";

    static auto get_object(std::istream &stream, Day15 &day) -> bool {
        std::string line{};

        while (std::getline(stream, line)) {
            day.m_sensors.emplace_back(line);
        }
        return true;
    }

    auto part1() const -> uint64_t override {
        std::set<Pos2D> exclusion;

        for (auto const &s : m_sensors) {
            auto dist = (s.nearest - s.position).abs_manhatten();

            for (int y = -dist; y <= dist; ++y) {
                if (s.position.y + y != RUN_LINE)
                    continue;
                int distx = dist - std::abs(y) + 1;
                for (int x = -distx + 1; x < distx; ++x) {
                    Pos2D pos = s.position + Pos2D{x, y};
                    if (std::find_if(m_sensors.begin(), m_sensors.end(),
                                     [&pos](Sensor const &sensor) {
                                         return sensor.nearest == pos;
                                     }) == m_sensors.end()) {
                        exclusion.insert(pos);
                    }
                }
                cout << "exclusion.size() = " << exclusion.size() << endl;
            }
        }

        return exclusion.size();
    }

    auto part2() const -> uint64_t override {
        const uint64_t max_coord = MAX_COORD;

        std::map<int, int> exclusion;

        for (auto line = 0; line <= max_coord; ++line) {
            exclusion.clear();

            for (auto const &s : m_sensors) {
                auto dist = (s.nearest - s.position).abs_manhatten();

                int start_y = std::max(s.position.y - dist + 1, 0);
                int end_y = std::min(static_cast<uint64_t>(s.position.y + dist),
                                     max_coord + 1);

                if (line < start_y || line > end_y)
                    continue;

                int y = line;

                int distx = dist - std::abs(y - s.position.y) + 1;

                int start_x = s.position.x - distx + 1;
                int end_x   = s.position.x + distx;

                {
                    auto [value, inserted] = exclusion.insert({start_x, 0});
                    ++value->second;
                }
                {
                    auto [value, inserted] = exclusion.insert({end_x, 0});
                    --value->second;
                }
            }

            int glevel = 0;
            for (auto const &[pos, level] : exclusion) {
                glevel += level;
                if (glevel == 0 && pos >= 0 && pos <= max_coord) {
                    cout << pos << ", " << line << endl;
                    return pos * MAX_COORD + line;
                }
            }
        }

        return 0;
    }
};

#endif
