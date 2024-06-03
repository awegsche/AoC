//
// Created by andiw on 05/01/2024.
//

#include <gtest/gtest.h>

#include "day10.h"

#include <cmake-build-debug/_deps/googletest-src/googletest/include/gtest/gtest.h>

TEST(day10, part1ex1) {

    AsteroidField field{
        "......#.#.\n#..#.#....\n..#######.\n.#.#.###..\n.#..#.....\n..#....#.#"
        "\n#..#....#.\n.##.#..###\n##...#..#.\n.#....####\n"};

    field.get_visible_asteroids();

    auto best = field.best();

    ASSERT_EQ(best.number_of_visible_asteroids(), 33);

    const aoc::Pos2D expected{5, 8};

    ASSERT_EQ(best.position, expected);
}

TEST(day10, part1ex2) {

    AsteroidField field{".#..##.###...#######\n##.############..##.\n.#.######.########.#\n.###.#######.####.#.\n#####.##.#.##.###.##\n..#####..#.#########\n####################\n#.####....###.#.#.##\n##.#################\n#####.##.###..####..\n..######..##.#######\n####.##.####...##..#\n.#####..#.######.###\n##...#.##########...\n#.##########.#######\n.####.#.###.###.#.##\n....##.##.###..#####\n.#.#.###########.###\n#.#.#.#####.####.###\n###.##.####.##.#..##"};
    field.get_visible_asteroids();

    auto best = field.best();

    ASSERT_EQ(best.number_of_visible_asteroids(), 210);

    const aoc::Pos2D expected{11, 13};

    ASSERT_EQ(best.position, expected);
}
