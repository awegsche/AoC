#include <iostream>
#include <filesystem>
#include <bitset>

#include "day1.h"
#include "day2.h"
#include "day3.h"
#include "day4.h"


using std::cout, std::cerr, std::endl;

int main() {
    cout << "Day 1" << endl;
    test_day<Day1>("2022", 24000, 45000);
    run_day<Day1>("2022");
    cout << endl;

    cout << "Day 2" << endl;
    test_day<Day2>("2022", 15, 12);
    run_day<Day2>("2022");
    cout << endl;

    cout << "Day 3" << endl;
    test_day<Day3>("2022", 157, 70);
    run_day<Day3>("2022");
    cout << endl;

    cout << "Day 4" << endl;
    test_day<Day4>("2022", 2, 4);
    run_day<Day4>("2022");
    cout << endl;
    return 0;
}
