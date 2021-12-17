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

fn count(
    p1: u8,
    p2: u8,
    rules: &HashMap<Pair, u8>,
    memo: &mut HashMap<Pair, HashMap<u8, usize>>,
    iterations: usize,
) -> HashMap<u8, usize> {
    if iterations == 0 {
        let mut counts = HashMap::new();
        *counts.entry(p1).or_default() += 1;
        *counts.entry(p2).or_default() += 1;
        return counts;
    }

    if let Some(c) = memo.get(&Pair::new(p1, p2).with_iterations(iterations)) {
        return c.clone();
    }
    if let Some(c) = rules.get(&Pair::new(p1, p2)) {
        let mut counts = HashMap::new();
        for (c, count) in count(p1, *c, rules, memo, iterations - 1) {
            *counts.entry(c).or_default() += count;
        }
        for (c, count) in count(*c, p2, rules, memo, iterations - 1) {
            *counts.entry(c).or_default() += count;
        }
        *counts.entry(*c).or_default() -= 1;
        memo.insert(
            Pair::new(p1, p2).with_iterations(iterations),
            counts.clone(),
        );
        counts
    } else {
        let mut counts = HashMap::new();
        *counts.entry(p1).or_default() += 1;
        *counts.entry(p2).or_default() += 1;
        counts
    }
}

fn solve(iterations: usize, polymer: &[u8], rules: &HashMap<Pair, u8>) -> usize {
    let mut memo: HashMap<Pair, HashMap<u8, usize>> = HashMap::new();
    let mut counts: HashMap<u8, usize> = HashMap::new();

    let mut windows = polymer.windows(2);
    {
        let w = windows.next().unwrap();

        for (c, count) in count(w[0], w[1], rules, &mut memo, iterations) {
            *counts.entry(c).or_default() += count;
        }
    }
    for w in windows {
        for (c, count) in count(w[0], w[1], rules, &mut memo, iterations) {
            *counts.entry(c).or_default() += count;
        }
        *counts.entry(w[0]).or_default() -= 1;
    }

    counts.values().max().unwrap() - counts.values().min().unwrap()
}

impl Challenge<usize> for Day14 {
    fn part1(&mut self) -> Result<usize, aoc::AocError> {
        Ok(solve(10, &self.polymer, &self.rules))
    }

    fn part2(&mut self) -> Result<usize, aoc::AocError> {
        Ok(solve(40, &self.polymer, &self.rules))
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
