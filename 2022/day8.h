//
// Created by andiw on 08/12/2022.
//

#ifndef AOC_DAY8_H
#define AOC_DAY8_H

#include <iostream>
#include <vector>

#include <AocDay.h>
#include <AocObject.h>

class Day8: public aoc::AocObject<Day8>, public  aoc::AocDay<Day8, int> {
    std::vector<std::vector<int>> m_trees;
    int w;
    int h;

public:
    static constexpr char FILENAME[] = "8";
    static constexpr char YEAR[] = "2022";
    static constexpr char TITLE[] = "Treetop Tree House";

    static auto get_object(std::istream& stream, Day8& day) -> bool {

        day.m_trees.clear();

        std::string line{};
        while(std::getline(stream, line)) {
            auto& row = day.m_trees.emplace_back();
            for (char c: line) {
                row.push_back(c - '0');
            }
        }
        day.w = day.m_trees[0].size();
        day.h = day.m_trees.size();

        return true;
    }

    int part1() const override {

        auto visible = [this](int x, int y) -> bool {
            int tree = m_trees[y][x];
            bool visible_left = true;
            bool visible_right = true;
            bool visible_up = true;
            bool visible_down = true;

            // to left
            for (int i = x-1; i >= 0; --i)
                visible_left &= m_trees[y][i] < tree;
            // to right
            for (int i = x+1; i < w; ++i)
                visible_right &= m_trees[y][i] < tree;
            // up
            for (int i = y-1; i >= 0; --i)
                visible_up &= m_trees[i][x] < tree;
            // down
            for (int i = y+1; i < h; ++i)
                visible_down &= m_trees[i][x] < tree;

            return visible_left | visible_right | visible_up | visible_down;
        };
        int count = 0;
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x) {
                if (visible(x, y)) ++count;
            }
        return count;
    }

    int part2() const override {
        auto score = [this](int x, int y) -> int {
            int tree = m_trees[y][x];
            int distance_left = 0;
            int distance_right = 0;
            int distance_up = 0;
            int distance_down = 0;

            // to left
            for (int i = x-1; i >= 0; --i) {
                ++distance_left;
                if(m_trees[y][i] >= tree) break;

            }
            // to right
            for (int i = x+1; i < w; ++i) {
                ++distance_right;
                if (m_trees[y][i] >= tree) break;
            }
            // up
            for (int i = y-1; i >= 0; --i) {
                ++distance_up;
                if (m_trees[i][x] >= tree) break;
            }
            // down
            for (int i = y+1; i < h; ++i) {
                ++distance_down;
                if (m_trees[i][x] >= tree) break;
            }

            return distance_left * distance_right * distance_up * distance_down;
        };
        int max = 0;
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x) {
                max = std::max(max, score(x,y));
            }
        return max;
    }
};

#endif //AOC_DAY8_H
