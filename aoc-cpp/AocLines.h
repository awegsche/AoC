//
// Created by andiw on 01/12/2022.
//

#ifndef AOC_AOCLINES_H
#define AOC_AOCLINES_H

#include <istream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <optional>
#include <filesystem>

//template<typename T>
//concept AocObjectable = requires(T obj, std::istream& file) {
//    {T::from_istream(file)} -> std::same_as<std::optional<T>>;
//};

using std::cout, std::cerr, std::endl;

namespace aoc {

std::istream &getline(std::istream &stream, std::string &line, char delimiter);

std::istream &getline(std::istream &stream, std::string &line);

template <typename Object> class AocLines {
public:
  explicit AocLines(std::istream &stream) noexcept {

    while (true) {
      std::optional<Object> obj = Object::from_istream(stream);
      if (!obj)
        break;
      m_objects.push_back(*obj);
    }
  }

  static auto from_file(std::string const &filename) noexcept
      -> std::optional<AocLines> {
    std::ifstream file(filename);

    if (!file.is_open()) {
      std::cerr << "failed to open file \"" << filename << "\"" << std::endl;
      return {};
    }

    return AocLines(file);
  }

  [[nodiscard]] auto begin() noexcept { return m_objects.begin(); }

  [[nodiscard]] auto end() noexcept { return m_objects.end(); }

private:
  std::vector<Object> m_objects;
    };

    template<typename Day>
    auto run_part1(Day &day) -> bool {
        return day.part1();
    }

} // namespace aoc

#endif //AOC_AOCLINES_H
