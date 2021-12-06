use std::fmt::{Display, write};

use aoc::day::Day;

const NAME: &str = "day4";

pub struct Day4 {
    numbers: Vec<i32>,
    boards: Vec<Board>,
}

#[derive(Default)]
pub struct Board {
    w: i32,
    numbers: Vec<i32>,
    marked: Vec<bool>,
}

impl Board {
    pub fn push_line(&mut self, line: &str) {
        let numbers = line.split_whitespace().map(|x| x.parse::<i32>().unwrap());

        self.numbers.extend(numbers);
        self.w += 1;
    }

    pub fn finish(&mut self) {
        self.marked = vec![false; self.numbers.len()];
    }

    pub fn mark(&mut self, number: i32) {
        for (n, mark) in self.numbers.iter().zip(self.marked.iter_mut()) {
            if *n == number {
                *mark = true;
                return;
            }
        }
    }

    pub fn is_finished(&self) -> bool {
        for row in 0..self.w {
            let begin = (row * self.w) as usize;
            let end = ((row + 1) * self.w) as usize;
            if self.marked[begin..end].iter().all(|b| *b) {
                return true;
            }
        }

        for column in 0..self.w {
            let mut all = true;
            for cell in 0..self.w {
                if !self.marked[(column + cell * self.w) as usize] {
                    all = false;
                }
            }
            if all {
                return true;
            }
        }

        false
    }

    pub fn score(&self) -> i32 {
        self.numbers
            .iter()
            .zip(self.marked.iter())
            .filter(|(n, m)| !**m)
            .map(|(n, m)| *n)
            .sum()
    }
}

impl Display for Board {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        for row in 0..self.w {
            for cell in 0..self.w {
                let index = (row*self.w + cell) as usize;
                if self.marked[index] {
                    write!(f, "\x1b[1m{:2}\x1b[0m ", self.numbers[index])?;
                }
                else {
                    write!(f, "{:2} ", self.numbers[index])?;
                }
            }
            write!(f, "\n")?;
        }
        Ok(())
    }
}

impl Day4 {
    pub fn from_file(filename: &str) -> Self {
        let mut lines = aoc::lines(filename).unwrap();

        let numbers = lines
            .next()
            .unwrap()
            .split(',')
            .map(|s| s.parse().unwrap())
            .collect();

        let mut boards = Vec::new();

        'outer: loop {
            let mut board = Board::default();

            loop {
                if let Some(line) = lines.next() {
                    if line.is_empty() {
                        break;
                    }

                    println!("parsing line: {:?}", line);
                    board.push_line(&line);
                }
                else {
            board.finish();
            boards.push(board);
                    break 'outer;}
            }

            board.finish();
            boards.push(board);
        }

        Self { numbers, boards }
    }

}

impl Day<i32> for Day4 {
    fn part1(&mut self) -> Result<i32, aoc::AocError> {
        for number in self.numbers.iter() {
            for board in self.boards.iter_mut() {
                board.mark(*number);
                if board.is_finished() {
                    return Ok(number * board.score());
                }
            }
        }

        Ok(0)
    }

    fn part2(&mut self) -> Result<i32, aoc::AocError> {
        let mut last = 0;
        for number in self.numbers.iter() {
            for board in self.boards.iter_mut() {
                board.mark(*number);
                if board.is_finished() {
                    last = board.score() * number;
                    println!("new last: {}", last);
                }
            }
            self.boards.retain(|b| !b.is_finished());
            if self.boards.len() == 0 { break; }

        }

        Ok(last)
    }

    fn name() -> &'static str {
        NAME
    }

    fn parse_input(filename: &str) -> Result<Self, aoc::AocError> {
        Ok(Self::from_file(filename))
    }
}

fn main() {
    Day4::run().unwrap();
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn part1() {
        assert_eq!(Day4::from_test().unwrap().part1().unwrap(), 4512)
    }
    #[test]
    fn part2() {
        assert_eq!(Day4::from_test().unwrap().part2().unwrap(), 1924)
    }
}
