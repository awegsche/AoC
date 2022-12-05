#include <iostream>
#include <filesystem>
#include <bitset>

#include "day1.h"
#include "day2.h"
#include "day3.h"
#include "day4.h"
#include "day5.h"


using std::cout, std::cerr, std::endl;

int main() {
    cout << "Day 1" << endl;
    Day1::test(24000, 45000);
    Day1::run();
    cout << endl;

    cout << "Day 2" << endl;
    Day2::test(15, 12);
    Day2::run();
    cout << endl;

    cout << "Day 3" << endl;
    Day3::test(157, 70);
    Day3::run();
    cout << endl;

    cout << "Day 4" << endl;
    Day4::test(2,4);
    Day4::run();
    cout << endl;
    return 0;
}
