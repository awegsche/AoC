
#ifndef AOC_DAY14_H
#define AOC_DAY14_H

#include <limits>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>

#include <re2.h>

#ifndef NGUI
#include <raylib-cpp.hpp>
#include <raylib.h>
#endif

#include <AocDay.h>
#include <AocObject.h>
#include <Pos2D.h>

enum Cell { Rock, Sand, Empty };

enum SandStatus { Falling, AtRest, InAbyss };

using aoc::Pos2D;
using std::cout, std::endl;

using SandMap = std::unordered_map<Pos2D, Cell>;

#ifndef NGUI
static const float RECTSIDE = 5.0f;
static const float WIDTH = 1024.0f;
constexpr int VIS_STEP = 5;

#endif

/// Updates the sand block's position
auto follow_sand(Pos2D &sand, SandMap &cells) -> SandStatus {
  Pos2D new_position = {sand.x, sand.y + 1};
  // . o .   o = sand
  // . ? .   ? = new_position

  // first test, if new_position is empty, o moves there
  // . o .   o = sand
  // . . .
  //   |
  //   V
  // . . .   o = sand
  // . o .
  {
    auto [cell_at_new, inserted] = cells.insert({new_position, Cell::Empty});

    if (cell_at_new->second == Cell::Empty) {
      // cells[new_position] = Cell::Sand;
      cell_at_new->second = Cell::Sand;
      cells[sand] = Cell::Empty;
      sand = new_position;
      return SandStatus::Falling;
    }
  }

  // first test, if new_position is occupied, try moving to the left
  // . o .   o = sand
  // . # .
  //   |
  //   V
  // . . .   o = sand
  // o # .
  --new_position.x;
  {
    auto [cell_at_new, inserted] = cells.try_emplace(new_position, Cell::Empty);
    if (cell_at_new->second == Cell::Empty) {
      // cells[new_position] = Cell::Sand;
      cell_at_new->second = Cell::Sand;
      cells[sand] = Cell::Empty;
      sand = new_position;
      return SandStatus::Falling;
    }
  }

  // first test, if new_position is occupied, try moving to the left
  // . o .   o = sand
  // # # .
  //   |
  //   V
  // . . .   o = sand
  // # # o
  new_position.x += 2;
  {
    auto [cell_at_new, inserted] = cells.try_emplace(new_position, Cell::Empty);
    if (cell_at_new->second == Cell::Empty) {
      // cells[new_position] = Cell::Sand;
      cell_at_new->second = Cell::Sand;
      cells[sand] = Cell::Empty;
      sand = new_position;
      return SandStatus::Falling;
    }
  }

  // if we haven't moved until now, stay and report at_rest
  return SandStatus::AtRest;
}

class Day14 : public aoc::AocObject<Day14>, public aoc::AocDay<Day14, int> {

  std::unordered_map<Pos2D, Cell> m_cells;
  int min_x;
  int min_y;
  int max_x;
  int max_y;
  Pos2D m_producer;

  void print_cells(SandMap const &cells) const {
    cout << "y: " << min_y << ", " << max_y << endl;
    cout << "x: " << min_x << ", " << max_x << endl;

    for (int y = min_y; y <= max_y; ++y) {
      for (int x = min_x; x <= max_x; ++x) {
        auto found = cells.find(Pos2D{x, y});
        if (found == cells.end())
          cout << '.';
        else {
          switch (found->second) {
          case Rock:
            cout << '#';
            break;
          case Sand:
            cout << 'o';
            break;
          case Empty:
            cout << '.';
            break;
            break;
          }
        }
      }
      cout << endl;
    }
  }

public:
  static constexpr char FILENAME[] = "14";
  static constexpr char YEAR[] = "2022";
  static constexpr char TITLE[] = "Regolith Reservoir";

  static auto get_object(std::istream &stream, Day14 &day) -> bool {
    std::string line{};
    const static re2::RE2 RE_POS{R"((\d+),(\d+)(\s*->\s*)?)"};

    day.m_producer = {500, 0};
    // include the producer
    day.max_x = 500;
    day.min_x = 500;
    day.max_y = 0;
    day.min_y = 0;

    while (std::getline(stream, line)) {
      re2::StringPiece sp_line{line};
      int from_x;
      int from_y;
      int to_x;
      int to_y;

      RE2::Consume(&sp_line, RE_POS, &from_x, &from_y);

      //            day.max_x = std::max(day.max_x, from_x);
      //            day.min_x = std::min(day.min_x, from_x);
      //            day.max_y = std::max(day.max_y, from_y);
      //            day.min_y = std::min(day.min_y, from_y);

      while (RE2::Consume(&sp_line, RE_POS, &to_x, &to_y)) {
        day.max_x = std::max(day.max_x, to_x);
        day.min_x = std::min(day.min_x, to_x);
        day.max_y = std::max(day.max_y, to_y);
        day.min_y = std::min(day.min_y, to_y);

        if (from_x == to_x) {
          for (int y = std::min(from_y, to_y); y <= std::max(from_y, to_y);
               ++y) {
            day.m_cells[Pos2D{from_x, y}] = Cell::Rock;
          }
        } else {
          for (int x = std::min(from_x, to_x); x <= std::max(from_x, to_x);
               ++x) {
            day.m_cells[Pos2D{x, from_y}] = Cell::Rock;
          }
        }
        from_x = to_x;
        from_y = to_y;
      }
    }
    return true;
  }

#ifndef NGUI
  auto draw_frame(SandMap const &cells, raylib::Window &window,
                  std::string const &title) const {

    window.ClearBackground(RAYWHITE);
    window.BeginDrawing();

    int sand = 0;

    for (const auto &[pos, cell] : cells) {
      raylib::Rectangle rect{5.0f * (pos.x - 500) + WIDTH * 0.5f,
                             5.0f * (pos.y), 5.0f, 5.0f};

      switch (cell) {
      case Rock:
        rect.Draw({128, 128, 128, 255});
        break;
      case Sand:
        rect.Draw({200, 150, 64, 255});
        ++sand;
        break;
      case Empty:
        break;
      }
    }
    std::stringstream ss{};
    ss << "Sand: " << sand;
    raylib::DrawText(ss.str(), 10, 30, 20, {0, 0, 0, 255});
    raylib::DrawText(title, 10, 10, 20, {0, 0, 0, 255});

    window.EndDrawing();
  }
#endif

  auto part1() const -> int {
    auto cells = m_cells;

#ifndef NGUI
    auto window = raylib::Window(static_cast<int>(WIDTH), 1024, TITLE);
    window.SetTargetFPS(60);
#endif

    while (true) {
      Pos2D sand = m_producer;

      SandStatus s;
      while ((s = follow_sand(sand, cells)) == SandStatus::Falling) {
        if (sand.y >= max_y) {
          // delete the sand block (it will fall down into the abyss)
          cells[sand] = Cell::Empty;
          s = SandStatus::InAbyss;
          break;
        }
      };
#ifndef NGUI
      draw_frame(cells, window, "Part 1");
#endif
      if (s == SandStatus::InAbyss)
        break;
    }
    cout << "show " << endl;

#ifndef NGUI
    while (!window.ShouldClose()) {
      draw_frame(cells, window, "Part 1");
    }
#endif

    int sum = 0;
    for (const auto &[key, cell] : cells) {
      if (cell == Cell::Sand)
        ++sum;
    }
    return sum;
  }

  auto part2() const -> int {
    auto cells = m_cells;

#ifndef NGUI
    auto window = raylib::Window(static_cast<int>(WIDTH), 1024, TITLE);
    window.SetTargetFPS(60);
#endif

    int print_int = 0;
    while (true) {
      Pos2D sand = m_producer;

      SandStatus s;
      while ((s = follow_sand(sand, cells)) == SandStatus::Falling) {
        if (sand.y >= max_y + 1) {
          cells[sand] = Cell::Sand;
          break;
        }
      };
      ++print_int;

#ifndef NGUI
      if (print_int == VIS_STEP) {
        draw_frame(cells, window, "Part 2");
        print_int = 0;
      }
#endif

      if (sand == m_producer)
        break;
    }

    cells[m_producer] = Cell::Sand;
#ifndef NGUI
    while (!window.ShouldClose()) {
      draw_frame(cells, window, "Part 2");
    }
#endif

    int sum = 0;
    for (const auto &[key, cell] : cells) {
      if (cell == Cell::Sand)
        ++sum;
    }
    return sum;
  }
};

#endif
