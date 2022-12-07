//
// Created by andiw on 05/12/2022.
//

#ifndef AOC_AOCDAY_H
#define AOC_AOCDAY_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

#include "pretty.h"
#include "timing.h"

using std::cout, std::cerr, std::endl, std::setw, std::setfill, std::left, std::right;

namespace aoc {


    template<typename Tvalue>
    struct AocResult {
        Tvalue part1;
        Tvalue part2;
    };

    template<typename Tvalue>
    struct AocCheck {
        Tvalue part1;
        Tvalue part2;
        Tvalue correct1;
        Tvalue correct2;

        explicit operator bool() const { return part1 && part2; }
    };

    template<typename Tvalue>
    std::ostream &operator<<(std::ostream &os, AocCheck<Tvalue> const &ch) {
        return os
                << "Part 1: "
                << (ch.part1 == ch.correct1 ? OK : FAIL)
                << "   " << ch.part1 << " / " << ch.correct1
                << endl
                << "Part 2: "
                << (ch.part2 == ch.correct2 ? OK : FAIL)
                << "   " << ch.part2 << " / " << ch.correct2 << ""
                << endl;
    }


    template<typename Day, typename Tvalue>
    class AocDay {
    public:

        // ---- public interface -----------------------------------------------------------------------

        virtual auto part1() const -> Tvalue = 0;

        virtual auto part2() const -> Tvalue = 0;

        using value = Tvalue;

        // ---- public utilities -----------------------------------------------------------------------

        /// Loads the `AocDay`'s content from a file
        ///
        /// This requires the static function `get_object(std::istream& stream, Day& day) -> bool`
        /// implemented for the deriving `Day`.
        /// \param filename
        /// \return
        static auto from_file(std::filesystem::path const &filename) -> std::optional<Day> {
            std::ifstream file(filename);

            if (!file.is_open()) {

                cerr << "couldn't open file " << filename << endl;
                return {};
            }

            return Day::from_istream(file);
        }

        /// Launches the test as given by the example in aoc.
        /// \param part1
        /// \param part2
        /// \return
        static auto test(Tvalue correct1, Tvalue correct2) -> bool {

            auto day = from_file(std::filesystem::path(SOURCE_DIR)
                                 / Day::YEAR
                                 / (std::string("inputs/day") + Day::FILENAME + "_test_input.txt"));

            cout << ":: Test Day " << Day::FILENAME << endl;

            if (!day) {
                cerr << " couldn't load test input" << endl;
                return false;
            }

            cout << day->check(correct1, correct2) << endl;
            return true;
        }

        /// Actually solves this `Day`'s puzzle. Reading the input from `inputs/day##_input.txt` and
        /// printing out the solution.
        /// \return
        static auto run() -> bool {
            using std::chrono::steady_clock;

            Timer timer{};
            auto day = from_file(std::filesystem::path(SOURCE_DIR)
                                 / Day::YEAR
                                 / (std::string("inputs/day") + Day::FILENAME + "_input.txt"));
            auto time_setup = timer.elapsed();
            if (!day) return false;

            timer.restart();
            auto answer1 = day->part1();
            auto time1 = timer.elapsed();

            timer.restart();
            auto answer2 = day->part2();
            auto time2 = timer.elapsed();

            cout << "+" << setfill('-') << setw(COL1 + COL2 + COL3 + 1) << "-" << "+" << endl;

            cout << "| DAY " << setfill(' ')
                 << "\33[1m"
                 << setw(2) << left << Day::FILENAME << " "
                 << TITLE_COLOR
                 << setw(COL1 + COL2 + COL3 - 7) << left << Day::TITLE
                 << RESET
                 << "|" << endl;

            cout << "+" << setfill('-') << setw(COL1 + COL2 + COL3 + 1) << "-" << "+" << endl;

            cout << "|" << setfill(' ')
                 << setw(COL1) << left << " Setup : "
                 << STAR_COLOR
                 << setw(COL2) << " "
                 << RESET
                 << ":";
            print_time(time_setup);
            cout << "|" << endl;

            cout << "|" << setfill('.') << setw(COL1 + COL2 + COL3 + 1) << "." << "|" << endl;
            cout << "|" << setfill(' ') << setw(COL1 + COL2 + COL3 + 1) << " " << "|" << endl;

            cout << "|" << setfill(' ')
                 << setw(COL1) << left << " Part 1: "
                 << STAR_COLOR
                 << setw(COL2) << left << answer1
                 << RESET
                 << ":";
            print_time(time1);
            cout << "|" << endl;

            cout << "|" << setfill(' ')
                 << setw(COL1) << left << " Part 2: "
                 << STAR_COLOR
                 << setw(COL2) << left << answer2
                 << RESET
                 << ":";
            print_time(time2);
            cout << "|" << endl;

            cout << "+" << setfill('-') << setw(COL1 + COL2 + COL3 + 1) << "-" << "+" << endl;
            cout << endl;
            return true;
        }

        static auto manual_test(std::string const &input, Tvalue correct1, Tvalue correct2) {
            cout << "Day " << Day::FILENAME << "\33[1m manual test\33[0m" << endl;
            std::stringstream stream{input};

            auto day = Day::from_istream(stream);

            if (day) {
                cout << day->check(correct1, correct2) << endl;
            } else {
                cerr << "couldn't create Day from string \"" << input << "\"" << endl;
            }
        }

    private:
        auto both_parts() const -> AocResult<Tvalue> {
            return {part1(), part2()};
        }

        auto check(Tvalue correct1, Tvalue correct2) -> AocCheck<Tvalue> {
            auto result = both_parts();
            return {result.part1, result.part2, correct1, correct2};
        }
    };

} // namespace aoc

#endif //AOC_AOCDAY_H
