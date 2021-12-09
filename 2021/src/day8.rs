use std::{iter::Map, str::FromStr};

use aoc::{day::Challenge, sdl::sdl2::{self, pixels::Color}};
use aoc::day::Day;
use aoc_macros::Day;

const ONE_N_DIGITS: usize = 2;
const FOUR_N_DIGITS: usize = 4;
const SEVEN_N_DIGITS: usize = 3;
const EIGHT_N_DIGITS: usize = 7;

const ONE_SEGMENTS: &str    = "cf";
const TWO_SEGMENTS: &str    = "acdeg";
const THREE_SEGMENTS: &str  = "acdfg";
const FOUR_SEGMENTS: &str   = "bcdf";
const FIVE_SEGMENTS: &str   = "abdfg";
const SIX_SEGMENTS: &str    = "abdefg";
const SEVEN_SEGMENTS: &str  = "acf";
const EIGHT_SEGMENTS: &str  = "abcdefg";
const NINE_SEGMENTS: &str   = "abcdfg";
const ZERO_SEGMENTS: &str   = "abcefg";

#[derive(Day)]
#[day=8]
#[year="2021"]
#[title="Seven Segment Search"]
#[part1=26]
#[part2=0]
pub(crate) struct Day8 {
    outputs: Vec<Output>,
}

fn is_unique(digit: &&String) -> bool {
    digit.len() == ONE_N_DIGITS ||
        digit.len() == FOUR_N_DIGITS ||
        digit.len() == SEVEN_N_DIGITS ||
        digit.len() == EIGHT_N_DIGITS
}

impl Challenge<usize> for Day8 {
    fn part1(&mut self) -> Result<usize, aoc::AocError> {
        Ok(self.outputs.iter().map(|output| {
           output.output.iter().filter(is_unique).count()
        }).sum())
    }

    fn part2(&mut self) -> Result<usize, aoc::AocError> {
        aoc::AocError::not_impl()
    }

    fn parse_input(filename: &str) -> Result<Self, aoc::AocError> {
        Ok(Self{
            outputs: aoc::parse_lines(filename)?.collect()
        })
    }
}

struct Output {
    pub digits: Vec<String>,
    pub output: Vec<String>,
    //pub map: Map<String, u32>,
}

impl FromStr for Output {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut split = s.split('|');
        let digits = split.next().unwrap().trim()
            .split_whitespace().map(|s| s.to_string()).collect();
        let output = split.next().unwrap().trim()
            .split_whitespace().map(|s| s.to_string()).collect();

        Ok(
            Self{
                digits,
                output
            }
        )
    }
}

impl Output {
    pub fn check_digits(&mut self) {
        // get 1:
        let one = self.digits.iter().find(|d| d.len() == 2).unwrap();
    }
}
