use aoc::day::{Challenge, Day};
use aoc_macros::Day;

const DAYS: usize = 80;
const DAYS2: usize = 256;

#[derive(Day)]
#[day=6]
#[year="2021"]
#[part1=5934]
#[part2=26984457539]
pub(crate) struct Day6 {
    fish: Vec<u8>,
}

impl Day6 {
    fn calc_number(&self, days: usize) -> usize {
        let mut fish = [0; 10];

        for i in 0..9 {
            fish[i] = self.fish.iter().filter(|x| **x == i as u8).count();
        }

        for d in 0..days {
            let parents = fish[0];
            let children = fish[8];
            fish[8] = fish[0];
            fish[0] = fish[1];
            fish[1] = fish[2];
            fish[2] = fish[3];
            fish[3] = fish[4];
            fish[4] = fish[5];
            fish[5] = fish[6];
            fish[6] = fish[7] + parents;
            fish[7] = children;
        }
        fish.iter().sum()
    }
}

impl Challenge<usize> for Day6 {
    fn part1(&mut self) -> Result<usize, aoc::AocError> {
        Ok(self.calc_number(DAYS))
    }

    fn part2(&mut self) -> Result<usize, aoc::AocError> {
        Ok(self.calc_number(DAYS2))
    }

    fn parse_input(filename: &str) -> Result<Self, aoc::AocError> {
        let line = aoc::lines(filename)?.next().unwrap();

        Ok(Self {
            fish: line.split(',').map(|s| s.parse().unwrap()).collect(),
        })
    }
}
