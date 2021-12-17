use std::collections::HashMap;
use std::str::FromStr;

use aoc::day::{Challenge, Day};
use aoc_macros::Day;
use log::info;

#[derive(Day)]
#[day = 14]
#[year = "2021"]
#[title = "Extended Polymerization"]
#[part1 = 1588]
#[part2 = 2188189693529]
pub(crate) struct Day14 {
    polymer: Vec<u8>,
    rules: HashMap<Pair, u8>,
}

#[derive(PartialEq, Eq, Hash)]
struct Pair {
    pair: [u8; 2],
    iterations: usize,
}

impl FromStr for Pair {
    fn from_str(s: &str) -> Result<Self, <Self as FromStr>::Err> {
        let s = s.as_bytes();
        Ok(Self {
            pair: [s[0], s[1]],
            iterations: 0,
        })
    }
    type Err = String;
}

impl Pair {
    pub fn new(p1: u8, p2: u8) -> Self {
        Self {
            pair: [p1, p2],
            iterations: 0,
        }
    }
    pub fn with_iterations(mut self, it: usize) -> Self {
        self.iterations = it;
        self
    }
}

fn solve(iterations: usize, polymer: &Vec<u8>) -> usize {
    // actually we might be able to solve this like the lantern fish
    // by keeping track of how many instances of each pair are there
    //
    let mut pairs: HashMap<[u8;2], usize> = polymer.windows(2).map(|w| ([w[0], w[1]], 1)).collect();

    for _ in 0..iterations {
        for (pair, count) in pairs.iter() {

         // TODO
        }
    }
    0

}

impl Challenge<usize> for Day14 {
    fn part1(&mut self) -> Result<usize, aoc::AocError> {
        //Ok(solve(10, &self.polymer, &self.rules))
        aoc::AocError::not_impl()
    }

    fn part2(&mut self) -> Result<usize, aoc::AocError> {
        //Ok(solve(40, &self.polymer, &self.rules))
        aoc::AocError::not_impl()
    }

    fn parse_input(filename: &str) -> Result<Self, aoc::AocError> {
        let mut lines = aoc::lines(filename)?;

        let polymer = lines.next().unwrap().as_bytes().to_vec();
        lines.next();

        let mut rules: HashMap<Pair, u8> = HashMap::new();
        for line in lines {
            let (key, value) = line.split_once(" -> ").unwrap();

            rules.insert(key.parse().unwrap(), value.as_bytes()[0]);
        }

        Ok(Self { polymer, rules })
    }
}
