#include <iostream>
#include <filesystem>
#include <bitset>

#include "day1.h"
#include "day2.h"
#include "day3.h"
#include "day4.h"
#include "day5.h"
#include "day6.h"

using std::cout, std::cerr, std::endl;

void tests() {
    Day1::test(24000, 45000);

    Day2::test(15, 12);

    Day3::test(157, 70);

    Day4::test(2, 4);

    Day5::test("CMZ", "MCD");

    Day6::test(5, 23);
    Day6::manual_test("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", 10, 29);
    Day6::manual_test("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", 11, 26);
}

void runs() {
    Day1::run();
    Day2::run();
    Day3::run();
    Day4::run();
    Day5::run();
    Day6::run();
}

int main() {
    tests();

    runs();

    return 0;
}
