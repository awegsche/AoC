
#ifndef AOC_DAY17_H
#define AOC_DAY17_H

#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <unordered_map>

#ifndef NGUI
#include <raylib-cpp.hpp>
#include <raylib.h>
#endif
#include <glm/vec2.hpp>

#include <AocDay.h>
#include <AocObject.h>
#include <Pos2D.h>

using aoc::Pos2D;

namespace day17 {

constexpr int WIDTH             = 1024;
constexpr int HEIGHT            = 1024;
constexpr size_t PARTICLE_COUNT = 128;
constexpr float PIXELSIZE       = 15.0f;
constexpr float HELLO           = 1.0f;

/// Helper struct for timed events (e.g. Animations).
///
/// ```
/// Ticker step_tick{60}; // once a second for 60 fps
///
/// // in main loop
/// if (step_tick.tick()) {
///     // do something once per second
/// }
/// ```
struct Ticker {
    int m_tick;
    int m_interval;

    explicit Ticker(int interval) : m_tick(0), m_interval(interval) {}

    bool tick() {
        ++m_tick;
        if (m_tick == m_interval) {
            m_tick = 0;
            return true;
        }
        return false;
    }
};

#ifndef NGUI
class PixelEngine {

    int pixel_size;

  public:
    explicit PixelEngine(int pixelsize) : pixel_size(pixelsize) {}

    void draw_rect(int x, int y, int width, int height, Color color) {
        ::DrawRectangle(x * pixel_size, y * pixel_size, width * pixel_size,
                        height * pixel_size, color);
    }
    void draw_rect_line(int x, int y, int width, int height, Color color) {
        ::DrawRectangleLines(x * pixel_size, y * pixel_size, width * pixel_size,
                             height * pixel_size, color);
    }
    void draw_text(std::string const &text, int x, int y, int fontsize,
                   Color color) {
        DrawText(text.c_str(), x * pixel_size, y * pixel_size,
                 fontsize * pixel_size, color);
    }
};
#endif

constexpr float G = 9.81f;

struct Particle {
    glm::vec2 pos;
    glm::vec2 vel;
    glm::vec2 acc;

    void update(float dt) {
        acc -= G;
        pos += dt * vel + 0.5f * dt * dt * acc;
    }
};

enum Cell { Rock, Empty };

enum Movement { Falling, CollidedWithEdge, AtRest };

struct Rock {
    static std::array<Pos2D, 4> Minus(Pos2D const &pos) {
        return {{
            {pos.x, pos.y},
            {pos.x + 1, pos.y},
            {pos.x + 2, pos.y},
            {pos.x + 3, pos.y},
        }};
    }

    static std::array<Pos2D, 5> Plus(Pos2D const &pos) {
        return {{
            {pos.x, pos.y + 1},
            {pos.x + 1, pos.y},
            {pos.x + 1, pos.y + 1},
            {pos.x + 2, pos.y + 1},
            {pos.x + 1, pos.y + 2},
        }};
    }

    static std::array<Pos2D, 5> Hook(Pos2D const &pos) {
        return {{
            {pos.x, pos.y},
            {pos.x + 1, pos.y},
            {pos.x + 2, pos.y},
            {pos.x + 2, pos.y + 1},
            {pos.x + 2, pos.y + 2},
        }};
    }

    static std::array<Pos2D, 4> Pipe(Pos2D const &pos) {
        return {{
            {pos.x, pos.y},
            {pos.x, pos.y + 1},
            {pos.x, pos.y + 2},
            {pos.x, pos.y + 3},
        }};
    }

    static std::array<Pos2D, 4> Square(Pos2D const &pos) {
        return {{
            {pos.x, pos.y},
            {pos.x, pos.y + 1},
            {pos.x + 1, pos.y + 1},
            {pos.x + 1, pos.y},
        }};
    }

    static bool falling(Pos2D &pos, int step,
                        std::unordered_map<Pos2D, Cell> &chamber) {
        pos.y -= 1;

        if (check_rock_falling(pos, step, chamber)) {
            pos.y += 1;
            return true;
        }
        return false;
    }

    static bool push(Pos2D &pos, int step, int direction,
                     std::unordered_map<Pos2D, Cell> &chamber) {

        pos.x += direction;
        if (check_rock_edge(pos, step, chamber)) {
            pos.x -= direction;
            return false;
        }
        return true;
    }

    static std::vector<Pos2D> get_rock(Pos2D const &pos, int step) {
        switch (step) {
        case 0: {

            auto arr = Minus(pos);
            return std::vector<Pos2D>{arr.begin(), arr.end()};
        }
        case 1: {

            auto arr = Plus(pos);
            return std::vector<Pos2D>{arr.begin(), arr.end()};
        }
        case 2: {
            auto arr = Hook(pos);
            return std::vector<Pos2D>{arr.begin(), arr.end()};
        }
        case 3: {
            auto arr = Pipe(pos);
            return std::vector<Pos2D>{arr.begin(), arr.end()};
        }
        case 4: {
            auto arr = Square(pos);
            return std::vector<Pos2D>{arr.begin(), arr.end()};
        }
        }
        return {};
    }

    static bool check_rock_falling(Pos2D &pos, int step,
                                   std::unordered_map<Pos2D, Cell> &chamber) {
        switch (step) {
        case 0:
            return check_falling(Minus(pos), chamber);
        case 1:
            return check_falling(Plus(pos), chamber);
        case 2:
            return check_falling(Hook(pos), chamber);
        case 3:
            return check_falling(Pipe(pos), chamber);
        case 4:
            return check_falling(Square(pos), chamber);
        }
        throw std::runtime_error("unknown shape id");
    }

    static bool check_rock_edge(Pos2D &pos, int step,
                                std::unordered_map<Pos2D, Cell> &chamber) {
        switch (step) {
        case 0:
            return check_edge(Minus(pos), chamber);
        case 1:
            return check_edge(Plus(pos), chamber);
        case 2:
            return check_edge(Hook(pos), chamber);
        case 3:
            return check_edge(Pipe(pos), chamber);
        case 4:
            return check_edge(Square(pos), chamber);
        }
        throw std::runtime_error("unknown shape id");
        return false;
    }

    template <size_t Size>
    static bool check_falling(std::array<Pos2D, Size> const &rock,
                              std::unordered_map<Pos2D, Cell> &chamber) {
        if (rock[0].y < 0)
            return true;
        for (auto const &p : rock) {
            if (chamber.contains(p))
                return true;
        }
        return false;
    }

    template <size_t Size>
    static bool check_edge(std::array<Pos2D, Size> const &rock,
                           std::unordered_map<Pos2D, Cell> &chamber) {
        for (auto const &p : rock) {
            if (p.x < 0 || p.x >= 7 || chamber.contains(p))
                return true;
        }
        return false;
    }
};

class Day17 : public aoc::AocObject<Day17>,
              public aoc::AocDay<Day17, uint64_t> {

    std::string wind;

  public:
    static constexpr char FILENAME[] = "17";
    static constexpr char YEAR[]     = "2022";
    static constexpr char TITLE[]    = "Wind Tetris";

    static auto get_object(std::istream &stream, Day17 &day) -> bool {

        std::getline(stream, day.wind);

        return true;
    }

    auto part1() const -> uint64_t {

        // all the gui stuff here
#ifndef NGUI
        auto window = raylib::Window(WIDTH, HEIGHT, TITLE);
        window.SetTargetFPS(60);

        std::default_random_engine r{};
        int screen_width = WIDTH / static_cast<int>(PIXELSIZE);
        std::uniform_int_distribution<int> particle_dist{0, screen_width};
        std::uniform_int_distribution<int> velocity{0, 1};

        std::vector<aoc::Pos2D> particles;
        particles.reserve(PARTICLE_COUNT);
        for (size_t i = 0; i < PARTICLE_COUNT; ++i) {
            particles.emplace_back(particle_dist(r), particle_dist(r));
        }

        int wind_step = 0;
        int rock_step = 0;
        int direction = wind[0] == '<' ? -1 : 1;

        Ticker wind_ticker{1};
        Ticker particle_ticker{3};
        PixelEngine pixel{static_cast<int>(PIXELSIZE)};

        std::string stepstring{};
        std::string nb_rocks;
        std::string total_height;
        int rock_count = 0;
        int skip_ticks = 100;

        std::unordered_map<Pos2D, Cell> chamber;
        std::vector<Pos2D> the_rock;
        Pos2D pos{2, 2};

        while (!window.ShouldClose() && rock_count < 3000) {

            for (int i = 0; i < skip_ticks; ++i) {
                if (particle_ticker.tick()) {
                    for (auto &p : particles) {
                        p.x += direction * velocity(r);
                        if (p.x > screen_width)
                            p.x = 0;
                        if (p.x < 0)
                            p.x = screen_width - 1;
                        if (p.y > screen_width)
                            p.y = 0;
                        if (p.y < 0)
                            p.y = screen_width - 1;
                    }
                }

                if (wind_ticker.tick()) {
                    for (auto &p : particles)
                        p.y += velocity(r);

                    if (++wind_step == wind.size())
                        wind_step = 0;

                    direction = wind[wind_step] == '<' ? -1 : 1;

                    {
                        std::stringstream ss{};
                        ss << std::setw(2) << wind[wind_step] << " "
                           << wind_step << " Pos: " << pos;
                        stepstring = ss.str();
                    }

                    Rock::push(pos, rock_step, direction, chamber);
                    if (Rock::falling(pos, rock_step, chamber)) {
                        int highest = 0;
                        for (auto const &p : Rock::get_rock(pos, rock_step)) {
                            chamber[p] = Cell::Rock;
                            highest    = std::max(highest, p.y);
                        }
                        std::stringstream ss{};
                        ss << "height: " << highest;
                        total_height = ss.str();
                        std::stringstream s2{};
                        s2 << "nb rocks: " << ++rock_count;
                        nb_rocks = s2.str();

                        pos = {2, highest + 4};
                        rock_step += 1;
                        if (rock_step == 5)
                            rock_step = 0;

                        if (rock_count == 2023) {
                            cout << "height: " << highest + 1 << endl;
                        }
                    }
                }
            }

            window.ClearBackground(RAYWHITE);
            window.BeginDrawing();
            for (auto const &p : particles) {
                pixel.draw_rect(p.x, p.y, 1, 1, {200, 200, 200, 255});
            }
            pixel.draw_text(stepstring, 5, 5, 3, {100, 100, 200, 255});
            pixel.draw_text(nb_rocks, 5, 10, 3, {100, 100, 200, 255});
            pixel.draw_text(total_height, 5, 15, 3, {100, 100, 200, 255});

            for (Pos2D const &p : Rock::get_rock(pos, rock_step)) {
                pixel.draw_rect(p.x + screen_width / 2, screen_width - p.y - 2,
                                1, 1, {200, 50, 50, 255});
            }

            pixel.draw_rect(screen_width / 2 - 1, 0, 1, screen_width,
                            {0, 0, 0, 255});
            pixel.draw_rect(screen_width / 2 + 7, 0, 1, screen_width,
                            {0, 0, 0, 255});
            pixel.draw_rect(screen_width / 2, screen_width - 1, 8, 1,
                            {0, 0, 0, 255});

            for (const auto &[pos, cell] : chamber) {
                if (cell == Cell::Rock)
                    pixel.draw_rect(pos.x + screen_width / 2,
                                    screen_width - pos.y - 2, 1, 1,
                                    {160, 160, 160, 255});
            }

            window.EndDrawing();
        }
#endif

        return 0;
    }

    auto part2() const -> uint64_t { return 0; }
};

} // namespace day17

#endif
