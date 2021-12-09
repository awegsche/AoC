use aoc::day::{Challenge, Day};
use aoc_macros::Day;

#[derive(Day)]
#[day = 7]
#[year = "2021"]
#[title = "The Treachery of Whales"]
#[part1 = 37]
#[part2 = 168]
pub(crate) struct Day7 {
    positions: Vec<i32>,
}

fn calc_fuel(positions: &[i32], x0: i32) -> i32 {
    positions.iter().map(|pos| (pos - x0).abs()).sum()
}

fn add_up(x: i32) -> i32 {
    x*(x+1)/2
}

fn calc_fuel_nl(positions: &[i32], x0: i32) -> i32 {
    positions.iter().map(|pos| add_up((pos - x0).abs())).sum()
}

impl Challenge<i32> for Day7 {
    fn part1(&mut self) -> Result<i32, aoc::AocError> {
        self.positions.sort();
        let median = self.positions[self.positions.len() / 2];

        Ok(calc_fuel(&self.positions, median))
    }

    fn part2(&mut self) -> Result<i32, aoc::AocError> {
        let max = *self.positions.iter().max().unwrap();

        let (fuel, _) = (0..max)
            .map(|x0| (calc_fuel_nl(&self.positions, x0), x0))
            .min_by(|(fa, _), (fb, _)| fa.cmp(fb))
            .unwrap();

        Ok(fuel)
    }

    fn parse_input(filename: &str) -> Result<Self, aoc::AocError> {
        Ok(Self {
            positions: aoc::lines(filename)?
                .next()
                .unwrap()
                .split(',')
                .map(|s| s.parse().unwrap())
                .collect(),
        })
    }
}
