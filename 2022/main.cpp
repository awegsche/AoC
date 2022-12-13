#include <iostream>
#include <filesystem>
#include <bitset>

//#include "day1.h"
#include "day2.h"
#include "day3.h"
#include "day4.h"
#include "day5.h"
#include "day6.h"
#include "day7.h"
#include "day8.h"
#include "day9.h"
#include "day10.h"
#include "day11.h"
#include "day12.h"

//#define ONLY_TESTS

using std::cout, std::cerr, std::endl;

void tests() {
 //   Day1::test(24000, 45000);

    Day2::test(15, 12);

    Day3::test(157, 70);

    Day4::test(2, 4);

    Day5::test("CMZ", "MCD");

    Day6::test(5, 23);
    Day6::manual_test("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg", 10, 29);
    Day6::manual_test("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw", 11, 26);

    Day7::test(95437, 24933642);

    Day8::test(21, 8);

    Day9::test(13, 1);
    Day9::manual_test("R 5\nU 8\nL 8\nD 3\nR 17\nD 10\nL 25\nU 20", 0, 36);

    Day10::test(13140, 0);

    Day11::test(10605, 2713310158);

    Day12::test(10, 0);
}

void runs() {
  //  Day1::run();
    Day2::run();
    Day3::run();
    Day4::run();
    Day5::run();
    Day6::run();
    Day7::run();
    Day8::run();
    Day9::run();
    Day10::run();
    Day11::run();
    Day12::run();
}

int main() {
    
    cout << "hello" << endl;

    /*
    auto day10 = Day10::from_input();

    if (day10) {
        day10->show_window();
    }
    return 0;
    */

    tests();

#ifndef ONLY_TESTS
    runs();
#endif

    return 0;
}
