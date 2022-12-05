//
// Created by andiw on 05/12/2022.
//

#ifndef AOC_AOCDAY_H
#define AOC_AOCDAY_H

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

using std::cout, std::cerr, std::endl;

template<typename Day, typename Tvalue>
class AocDay {
public:

    using value = Tvalue;

    /// Loads the `AocDay`'s content from a file
    ///
    /// This requires the static function `get_object(std::istream& stream, Day& day) -> bool`
    /// implemented for the deriving `Day`.
    /// \param filename
    /// \return
    static auto from_file(std::filesystem::path const& filename) -> std::optional<Day> {
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
    static auto test(Tvalue part1, Tvalue part2) -> bool {

        auto day = from_file(std::filesystem::path("../..")
                             / Day::YEAR
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

    /// Actually solves this `Day`'s puzzle. Reading the input from `inputs/day##_input.txt` and
    /// printing out the solution.
    /// \return
    static auto run() -> bool {
        auto day = from_file(std::filesystem::path("../..")
                             / Day::YEAR
                             / (std::string("inputs/day") + Day::FILENAME + "_input.txt"));
        if (!day) return false;

        cout << "Part 1 = " << day->part1() << endl;
        cout << "Part 2 = " << day->part2() << endl;

        return true;
    }

    virtual auto part1() -> Tvalue = 0;

    virtual auto part2() -> Tvalue = 0;

};


#endif //AOC_AOCDAY_H
