
//
// Created by andiw on 10/12/2022.
//

#ifndef AOC_DAY12_H
#define AOC_DAY12_H

#include <functional>
#include <iostream>
#include <set>
#include <istream>
#include <limits>
#include <ostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <limits.h>
#include <unordered_map>
#include <compare>

#include <AocObject.h>
#include <AocDay.h>
#include <Pos2D.h>

using aoc::Pos2D;
using std::cout, std::endl;

struct finding_record {
    int length;
    Pos2D pos;

    auto operator<=>(finding_record const& other) const {
        return length <=> other.length;
    }
};

std::ostream& operator<<(std::ostream& os, finding_record const& r) {
    os << "( cost: " << r.length << ", pos: " << r.pos << ")";
    return os;
}


class Day12 : public aoc::AocObject<Day12>, public aoc::AocDay<Day12, int> {
    std::vector<int> heightmap;
    int width;
    int height;
    Pos2D start;
    Pos2D end;


    public:
    static constexpr char FILENAME[] = "12";
    static constexpr char YEAR[] = "2022";
    static constexpr char TITLE[] = "Hill Climbing Algorithm";

    static auto get_object(std::istream& stream, Day12& day) -> bool {

        day.heightmap.clear();
        std::string line{};

        int row = 0;

        while(std::getline(stream, line)) {
            day.width = line.size();
            for(int col = 0; col < line.size(); ++col) {
                char c = line[col];
                switch (c) {
                    case 'S':
                        day.start = {col, row};
                        day.heightmap.push_back(0);
                        break;
                    case 'E':
                        day.end = {col, row};
                        day.heightmap.push_back(26);
                        break;
                    default:
                        day.heightmap.push_back(static_cast<int>(c - 'a'));
                        break;
                }
            }
            ++row;
        }
        day.height = row;

        return true;
    }

    auto print_path(std::vector<Pos2D> const& path, std::unordered_map<Pos2D, Pos2D> const& prev) const {
        auto find = [&path] (Pos2D const& p) -> int {
            for (int i = 0; i < path.size(); ++i) {
                if (p == path[i]) return i;
            }
            return -1;
        };

        cout << "path len: " << path.size() << endl;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {

                char c = static_cast<char>(heightmap[y*width+x] + 'a');
                int h = heightmap[y*width+x] * 255 / 26;
                int found = find({x,y});
                cout <<"\33[48;2;" << h << ";" << h << ";" << h << "m";
                   if (found != -1) {
                //auto col = 255 -255 * found / path.size();
                int col = 255;
                cout << "\33[38;2;" << col << ";50;50m";
                }
                Pos2D p0 = {x,y};
                Pos2D const& p = prev.at(p0);
                if (p == Pos2D{-1,-1})
                    cout << c;
                else if (p.x < p0.x) {
                    cout << "<";
                }
                else if (p.x > p0.x) {
                    cout << ">";
                }
                else if (p.y < p0.y) {
                    cout << "^";
                }
                else if (p.y > p0.y) {
                    cout << "V";
                }
                else
                    cout << c;
                cout << "\33[0m";
            }
            cout <<endl;
        }

    }


    template<typename Comp>
    auto dijkstra(Comp const& comp) const -> int {

        cout << "start: " << start << endl;
        cout << "end: " << end << endl;

        static const std::array<Pos2D, 4> DELTA_POS{{{-1,0}, {1,0}, {0, -1}, {0,1}}};

        std::vector<finding_record> queue;
        std::unordered_map<Pos2D, int> best;
        std::unordered_map<Pos2D, Pos2D> prev;

        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x){
                best.insert({{x,y}, std::numeric_limits<int>::max()});
                prev.insert({Pos2D{x,y}, Pos2D{-1,-1}});
            }

        queue.push_back({0, start});
        int steps = 0;

        while (!queue.empty()) {

            auto current = queue.back();
            queue.pop_back();

            std::vector<Pos2D> path{};
            for (auto const& dPos: DELTA_POS) {
                auto p = current.pos + dPos;


				if (p.x >= 0 && p.y >= 0 && p.x < width && p.y < height
					&& (heightmap[p.x + p.y * width] - heightmap[current.pos.x + current.pos.y * width]) <= 1) {
                    auto alt = current.length + 1;
                    if (alt < best[p]) {
                        best[p] = alt;
                        prev[p] = current.pos;
                        queue.push_back({alt, p});
                        std::push_heap(queue.begin(), queue.end(), std::greater<>());
                    }
                }
            }
        }

        std::vector<Pos2D> path{};
        auto p0 = end;
        while (comp(p0) && p0 != Pos2D{-1,-1}) {
            path.push_back(p0);
            p0 = prev[p0];
        }
        print_path(path, prev);
        return path.size();
    }

    int part1() const override {
        return dijkstra([this](Pos2D const& p) { return this->start != p;});
    }

    int part2() const override {
        return dijkstra([this](Pos2D const& p) { return this->heightmap[p.x + this->width * p.y] != 0; });
    }

};

#endif
