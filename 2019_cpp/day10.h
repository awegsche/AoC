//
// Created by andiw on 04/01/2024.
//

#ifndef DAY10_H
#define DAY10_H

#include <fmttools.h>
#include <format>
#include <iostream>
#include <numeric>
#include <raylib-cpp.hpp>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <Pos2D.h>
#include <algorithm>
#include <ranges>

struct Line {
    aoc::Pos2D start;
    aoc::Pos2D end;
    aoc::Polar2D dir;
};

struct Asteroid {
    aoc::Pos2D position                               = {0, 0};
    std::unordered_set<aoc::Pos2D> blocked_directions = {};

    size_t number_of_visible_asteroids() const {
        return blocked_directions.size();
    }
};

auto format_value(const Asteroid &asteroid, auto fmt) {
    return std::format_to(fmt, "{{{}, {}}}", asteroid.position,
                          asteroid.blocked_directions.size());
}

class AsteroidField {
  public:
    explicit AsteroidField(std::string_view code) {
        auto current = code.begin();

        int line_length = 0;
        while (current != code.end()) {
            if (*current == '\n') {
                width_ = line_length;
                ++height_;
                line_length = 0;
            } else {
                if (*current == '#') {
                    asteroids_.push_back({{line_length, height_}, {}});
                }
                ++line_length;
                cells_.push_back(*current);
            }
            ++current;
        }
        ++height_;
    }

    [[nodiscard]] int width() const { return width_; }
    [[nodiscard]] int height() const { return height_; }

    [[nodiscard]] std::vector<uint8_t> const &cells() const { return cells_; }

    [[nodiscard]] std::vector<Asteroid> const &asteroids() const {
        return asteroids_;
    }

    [[nodiscard]] uint8_t cell(int x, int y) const {
        return cells_[y * width_ + x];
    }

    void draw(raylib::Window const &window) const {
        raylib::Rectangle{-CELLPAD - 0.5f, -CELLPAD - 0.5f,
                          static_cast<float>(width_) + CELLPAD + 1.0f,
                          static_cast<float>(height_) + CELLPAD + 1.0f}
            .DrawLines({200, 200, 200, 255}, 0.5f);
        for (const auto &asteroid : asteroids_) {
            const auto x_ = static_cast<float>(asteroid.position.x);
            const auto y_ = static_cast<float>(asteroid.position.y);

            auto const rect = raylib::Rectangle{x_, y_, CELLSIZE, CELLSIZE};
            rect.Draw({128, 128, 128, 255});

            // raylib::DrawText(std::format("{}", asteroid.visible_asteroids),
            // x_, y_, 10, LIGHTGRAY);
        }

        uint8_t col = 0;
        int step    = 255 / lines_of_sight.size();
        step        = step == 0 ? 1 : step;

        for (const auto &l : lines_of_sight) {
            DrawLineV({l.start.x + 0.5f, l.start.y + 0.5f},
                      {l.end.x + 0.5f, l.end.y + 0.5f}, {col, 50, 50, 255});
            if (col < 255 - step)
                col += step;
        }
    }

    //static void

        /// \brief shoots a laser beam at the next asteroid
        void
        shoot() {}

    void get_visible_asteroids() {
        auto normalize = [](aoc::Pos2D const &pos) -> aoc::Pos2D {
            // if (pos.x == 0 || pos.y == 0)
            //    return pos;
            const auto gcd = std::gcd(pos.x, pos.y);
            return aoc::Pos2D{pos.x / gcd, pos.y / gcd};
        };

        for (auto current = asteroids_.begin(); current != asteroids_.end();
             ++current) {
            for (auto test = current + 1; test != asteroids_.end(); ++test) {
                const auto test_norm =
                    normalize(test->position - current->position);

                if (!current->blocked_directions.contains(test_norm)) {
                    current->blocked_directions.insert(test_norm);
                    test->blocked_directions.insert(-test_norm);
                }
            }
        }

        const auto &[position, blocked_directions] = best();
        // try to find line of sight
        for (const auto norm : blocked_directions) {
            auto line = get_next_asteroid(position, norm);

            if (line) lines_of_sight.push_back(*line);
        }

        std::sort(lines_of_sight.begin(), lines_of_sight.end(),
                  [](Line const &a, Line const &b) {
                      return a.dir.angle < b.dir.angle;
                  });
    }

    std::optional<Line> get_next_asteroid(const aoc::Pos2D& position, const aoc::Pos2D& norm ) {

        for (int i = 1;; ++i) {
            const auto dir = position + norm * i;

            if (dir.x < 0 || dir.y < 0 || dir.x > width() || dir.y > height()) {
                return {};
            }

            auto found = std::find_if(
                asteroids_.begin(), asteroids_.end(),
                [&dir](Asteroid const &a) { return a.position == dir; });

            if (found != asteroids_.end()) {
                lines_of_sight.push_back({position, dir, aoc::Polar2D{norm}});
                return Line{position, dir, aoc::Polar2D{norm}};
            }
        }
    }

    void shoot_asteroid() {
        auto const line = lines_of_sight[0];
        auto aim = get_next_asteroid(line.start, line.dir.to_pos2d());


    }

    [[nodiscard]] auto best() -> const Asteroid & {

        if (!best_) {
            best_ = *std::max_element(asteroids_.begin(), asteroids_.end(),
                                     [](Asteroid const &a, Asteroid const &b) {
                                         return a.blocked_directions.size() <
                                                b.blocked_directions.size();
                                     });
        }

        return *best_;
    }

    static constexpr float CELLSIZE = 0.8f;
    static constexpr float CELLPAD  = 1.0f - CELLSIZE;

  private:
    int width_  = 0;
    int height_ = 0;

    std::vector<uint8_t> cells_      = {};
    std::vector<Asteroid> asteroids_ = {};
    std::vector<Line> lines_of_sight = {};
    std::optional<Asteroid> best_ = {};
    // std::unordered_map<aoc::Pos2D, int> asteroids_;
};

inline auto format_value(AsteroidField const &field, auto fmt_it) {
    auto fmt = std::format_to(fmt_it, "AsteroidField width = {}, height = {}\n",
                              field.width(), field.height());

    for (int i = 0; i < field.height(); ++i) {
        for (int j = 0; j < field.width(); ++j) {
            fmt = std::format_to(fmt, "{}", (char)field.cell(j, i));
        }
        fmt = std::format_to(fmt, "\n");
    }

    fmt = std::format_to(fmt, "asteroids: ");
    for (const auto &asteroid : field.asteroids()) {
        fmt = std::format_to(fmt, "{}, ", asteroid);
    }
    return fmt;
}

inline void day10() {

    AsteroidField field{"#....#.....#...#.#.....#.#..#....#\n"
                        "#..#..##...#......#.....#..###.#.#\n"
                        "#......#.#.#.....##....#.#.....#..\n"
                        "..#.#...#.......#.##..#...........\n"
                        ".##..#...##......##.#.#...........\n"
                        ".....#.#..##...#..##.....#...#.##.\n"
                        "....#.##.##.#....###.#........####\n"
                        "..#....#..####........##.........#\n"
                        "..#...#......#.#..#..#.#.##......#\n"
                        ".............#.#....##.......#...#\n"
                        ".#.#..##.#.#.#.#.......#.....#....\n"
                        ".....##.###..#.....#.#..###.....##\n"
                        ".....#...#.#.#......#.#....##.....\n"
                        "##.#.....#...#....#...#..#....#.#.\n"
                        "..#.............###.#.##....#.#...\n"
                        "..##.#.........#.##.####.........#\n"
                        "##.#...###....#..#...###..##..#..#\n"
                        ".........#.#.....#........#.......\n"
                        "#.......#..#.#.#..##.....#.#.....#\n"
                        "..#....#....#.#.##......#..#.###..\n"
                        "......##.##.##...#...##.#...###...\n"
                        ".#.....#...#........#....#.###....\n"
                        ".#.#.#..#............#..........#.\n"
                        "..##.....#....#....##..#.#.......#\n"
                        "..##.....#.#......................\n"
                        ".#..#...#....#.#.....#.........#..\n"
                        "........#.............#.#.........\n"
                        "#...#.#......#.##....#...#.#.#...#\n"
                        ".#.....#.#.....#.....#.#.##......#\n"
                        "..##....#.....#.....#....#.##..#..\n"
                        "#..###.#.#....#......#...#........\n"
                        "..#......#..#....##...#.#.#...#..#\n"
                        ".#.##.#.#.....#..#..#........##...\n"
                        "....#...##.##.##......#..#..##...."};

    std::cout << std::format("{}", field);
    std::cout << "\n";

    field.get_visible_asteroids();

    std::cout << std::format("{}", field);
    std::cout << "\n";

    auto const best = field.best();

    std::cout << std::format("{}\n", best);

    // return;

    raylib::Window window{400, 400, "Day10"};
    SetTargetFPS(60);
    Camera2D cam{{10.0f, 30.0f}, {-1.0f, -1.0f}, 0.0f, 8.0f};

    int ticks = 0;

    while (!window.ShouldClose()) {
        BeginDrawing();

        window.ClearBackground(RAYWHITE);

        BeginMode2D(cam);
        field.draw(window);
        EndMode2D();

        DrawText("Day 10", 10, 5, 10, LIGHTGRAY);
        DrawText(std::format("{}", best).c_str(), 10, 15, 10, LIGHTGRAY);
        EndDrawing();
    }
}

#endif // DAY10_H
