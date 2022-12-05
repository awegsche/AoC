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

    Day1::test(24000, 45000);
    Day2::test(15, 12);
    Day3::test(157, 70);
    Day4::test(2, 4);
    Day5::test("CMZ", "MCD");

    Day1::run();
    Day2::run();
    Day3::run();
    Day4::run();
    Day5::run();
    return 0;
}
