use aoc::day::Day;

const NAME: &str = "day6";
const DAYS: usize = 80;
const DAYS2: usize = 256;

struct Day6 {
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

impl Day<usize> for Day6 {
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

    fn name() -> &'static str {
        NAME
    }
}

fn main() {
    Day6::run().unwrap();
}

#[cfg(test)]
mod day6 {
    use std::time::Instant;

    use super::*;
    #[test]
    fn part1() {
        assert_eq!(Day6::from_test().unwrap().part1().unwrap(), 5934);
    }
    #[test]
    fn part2() {
        let starttime = Instant::now();
        assert_eq!(Day6::from_test().unwrap().part2().unwrap(), 26984457539);
        println!(
            "time for test: {}s",
            (Instant::now() - starttime).as_secs_f32()
        );
    }
}
