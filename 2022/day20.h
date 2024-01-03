#ifndef DAY20_H_
#define DAY20_H_

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include <AocDay.h>
#include <AocObject.h>

struct Number {
    int n;
    int pos;
};

using std::cout, std::endl;

std::ostream &operator<<(std::ostream &os, Number const &n) {
    os << n.n << "(" << n.pos << ")";

    return os;
}

class Day20 : public aoc::AocObject<Day20>,
              public aoc::AocDay<Day20, uint64_t> {
    std::vector<Number> m_numbers;

  public:
    static constexpr char FILENAME[] = "20";
    static constexpr char YEAR[]     = "2022";
    static constexpr char TITLE[]    = "Grove Positioning System";

    static auto get_object(std::istream &stream, Day20 &day) -> bool {

        std::string line{};
        int pos = 0;

        while (std::getline(stream, line)) {
            int n = std::stoi(line);
            day.m_numbers.push_back({n, n + pos});
            ++pos;
        }

        return true;
    }

    auto part1() const -> uint64_t {
        auto numbers = m_numbers;

        for (auto const &n : numbers)
            cout << n << ", ";
        cout << endl;

        auto comp = [](const Number &a, const Number &b) {
            return a.pos < b.pos;
        };

        std::sort(numbers.begin(), numbers.end(), comp);

        for (auto const &n : numbers)
            cout << n << ", ";
        cout << endl;

        return 0;
    }

    auto part2() const -> uint64_t {
        auto numbers = m_numbers;

        for (auto const &n : numbers)
            cout << n << ", ";
        cout << endl;

        auto comp = [](const Number &a, const Number &b) {
            return a.pos < b.pos;
        };

        std::sort(numbers.begin(), numbers.end(), comp);

        for (auto const &n : numbers)
            cout << n << ", ";
        cout << endl;

        return 0;
    }
};
#endif // DAY20_H_
