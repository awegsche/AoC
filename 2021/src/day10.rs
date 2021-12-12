use aoc::day::{Challenge, Day};
use aoc_macros::Day;
use log::info;

#[derive(Day)]
#[day = 10]
#[year = "2021"]
#[part1 = 26397]
#[part2 = 288957]
pub(crate) struct Day10 {
    lines: Vec<String>,
}

enum LineOption {
    Incomplete(String),
    Corrupt(i64),
}

impl LineOption {
    pub fn is_incomplete(&self) -> Option<i64> {
        match self {
            LineOption::Incomplete(string) => {
                let mut result = 0;

                for ch in string.chars().rev() {
                    result *= 5;
                    result += match ch {
                        ')' => 1,
                        ']' => 2,
                        '}' => 3,
                        '>' => 4,
                        _ => 0,
                    }
                }
                Some(result)
            }
            _ => None,
        }
    }

    pub fn is_corrupt(&self) -> Option<i64> {
        match self {
            LineOption::Corrupt(score) => Some(*score),
            _ => None,
        }
    }
}

fn scan_line(line: &str) -> LineOption {
    let mut next_close = String::new();

    for token in line.chars() {
        match token {
            '(' => next_close.push(')'),
            '[' => next_close.push(']'),
            '{' => next_close.push('}'),
            '<' => next_close.push('>'),
            ')' => {
                if next_close.pop().unwrap() != ')' {
                    return LineOption::Corrupt(3);
                }
            }
            ']' => {
                if next_close.pop().unwrap() != ']' {
                    return LineOption::Corrupt(57);
                }
            }
            '}' => {
                if next_close.pop().unwrap() != '}' {
                    return LineOption::Corrupt(1197);
                }
            }
            '>' => {
                if next_close.pop().unwrap() != '>' {
                    return LineOption::Corrupt(25137);
                }
            }
            _ => {}
        };
    }
    LineOption::Incomplete(next_close)
}

impl Challenge<i64> for Day10 {
    fn part1(&mut self) -> Result<i64, aoc::AocError> {
        Ok(self
            .lines
            .iter()
            .map(|line| scan_line(line))
            .filter_map(|line| line.is_corrupt())
            .sum())
    }

    fn part2(&mut self) -> Result<i64, aoc::AocError> {
        let mut scores: Vec<i64> = self
            .lines
            .iter()
            .map(|line| scan_line(line))
            .filter_map(|line| line.is_incomplete())
            .collect();
        scores.sort();
        //for score in scores.iter() {
        //    println!("{:?}", score);
        //}
        Ok(scores[scores.len() / 2])
    }

    fn parse_input(filename: &str) -> Result<Self, aoc::AocError> {
        Ok(Self {
            lines: aoc::lines(filename)?.collect(),
        })
    }
}
