//
// Created by andiw on 01/12/2022.
//

#ifndef AOC_AOCLINES_H
#define AOC_AOCLINES_H

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


template<typename Object>
class AocLines {
public:
    explicit AocLines(std::istream& stream) noexcept {

        while(true) {
            std::optional<Object> obj = Object::from_istream(stream);
            if (!obj) break;
            m_objects.push_back(*obj);
        }
    }

    static auto from_file(std::string const& filename) noexcept -> std::optional<AocLines>{
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "failed to open file \"" << filename << "\"" << std::endl;
            return {};
        }

        return AocLines(file);
    }

    [[nodiscard]] auto begin() noexcept {
        return m_objects.begin();
    }

    [[nodiscard]] auto end() noexcept {
        return m_objects.end();
    }

private:
    std::vector<Object> m_objects;
};

template<typename Day>
auto run_part1(Day& day) -> bool {
    return day.part1();
}

template<typename Day>
auto from_file(std::filesystem::path const& filename) -> std::optional<Day> {
    std::ifstream file(filename);

    if (!file.is_open()) {

        cerr << "couldn't open file " << filename << endl;
        return {};
    }

    return Day::from_istream(file);
}

template<typename Day>
auto test_day(std::string const& year, typename Day::value part1, typename Day::value part2) -> bool {

    auto day = from_file<Day>(std::filesystem::path("../..")
                              / year
                              / (std::string("inputs/day") + Day::FILENAME + "_test_input.txt"));

    if (!day) return false;

    auto ans1 = day->part1();

    cout << "Part 1 = " << ans1;
    if (ans1 == part1)
        cout << " [OK] " << endl;
    else
        cout << " [X ] " << endl;

    auto ans2 = day->part2();

    cout << "Part 2 = " << ans2;
    if (ans2 == part2)
        cout << " [OK] " << endl;
    else
        cout << " [X ] " << endl;

    return true;
}

template<typename Day>
auto run_day(std::string const& year) -> bool {
    auto day = from_file<Day>(std::filesystem::path("../..")
                              / year
                              / (std::string("inputs/day") + Day::FILENAME + "_input.txt"));
    if (!day) return false;


    cout << "Part 1 = " << day->part1() << endl;
    cout << "Part 2 = " << day->part2() << endl;

    return true;
}

#endif //AOC_AOCLINES_H
