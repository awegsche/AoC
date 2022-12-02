#include <iostream>
#include <filesystem>

#include "day1.h"
#include "day2.h"

using std::cout, std::cerr, std::endl;

template<typename Day>
auto run_part1(Day& day) -> bool {
    return day.part1();
}

template<typename Day>
auto from_file(std::string const& filename) -> std::optional<Day> {
    std::ifstream file(filename);

    if (!file.is_open()) {

        cerr << "couldn't open file " << filename << endl;
        return {};
    }

    return Day::from_istream(file);
}

template<typename Day>
auto test_day(typename Day::value part1, typename Day::value part2) -> bool {

    auto day = from_file<Day>(std::string("../../2022/inputs/day") + Day::FILENAME + "_test_input.txt");

    if (!day) return false;

    auto ans1 = day->part1();

    cout << "Part 1 = " << ans1;
    if (ans1 == part1)
        cout << " [OK] " << endl;
    else
        cout << " [Xx] " << endl;

    auto ans2 = day->part2();

    cout << "Part 2 = " << ans2;
    if (ans2 == part2)
        cout << " [OK] " << endl;
    else
        cout << " [Xx] " << endl;

    return true;
}

template<typename Day>
auto run_day() -> bool {
    auto day = from_file<Day>(std::string("../../2022/inputs/day") + Day::FILENAME + "_input.txt");
    if (!day) return false;


    cout << "Part 1 = " << day->part1() << endl;
    cout << "Part 2 = " << day->part2() << endl;

    return true;
}

int main() {
    test_day<Day1>(24000, 45000);
    run_day<Day1>();

    test_day<Day2>(15, 12);
    run_day<Day2>();
    return 0;
}
