use aoc::{day::{Challenge, Day}, sdl::sdl2::rect::Point};
use aoc_macros::Day;
use log::info;
use std::{fmt::Display, iter::Sum, ops::Add, str::FromStr};
use nalgebra::Vector2;

#[derive(Day)]
#[day=2]
#[year="2021"]
#[part1=150]
#[part2=900]
pub struct Day2 {
    pub moves: Vec<Movement>,
}

#[derive(Debug, Clone, Copy)]
pub enum Movement {
    Forward(i32),
    Down(i32),
    Up(i32),
}

impl FromStr for Movement {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut words = s.split(' ');

        let direction = words.next().ok_or("couldn't parse movement")?;
        let distance: i32 = words.next().unwrap().parse().or(Err(
            "couldn't parse distance part of movement (has to be i32)",
        ))?;

        match direction {
            "forward" => Ok(Movement::Forward(distance)),
            "up" => Ok(Movement::Up(distance)),
            "down" => Ok(Movement::Down(distance)),
            _ => Err("movement has to be one of 'forward', 'up' or 'down".to_string()),
        }
    }
}

impl Display for Movement {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{:?}", self)
    }
}

impl Challenge<i32> for Day2 {
    fn part1(&mut self) -> Result<i32, aoc::AocError> {
        Ok(self
            .moves
            .iter()
            .map(|m| match m {
                Movement::Forward(x) => point::new(*x, 0),
                Movement::Down(y) => point::new(0, *y),
                Movement::Up(y) => point::new(0, -y),
            })
            .sum::<point>().answer())
    }

    fn part2(&mut self) -> Result<i32, aoc::AocError> {
        let mut position = point::new(0,0);
        let mut aim = 0;

        for m in self.moves.iter() {
            match m {
                Movement::Down(y) => { aim += *y},
                Movement::Up(y) => { aim -= *y },
                Movement::Forward(x) => {
                    position.x += *x;
                    position.y += *x * aim;
                }
            }
        }

        info!("position: {}", position);

        Ok(position.answer())
    }

    fn parse_input(filename: &str) -> Result<Self, aoc::AocError> {
        Ok(Self {
            moves: aoc::parse_lines(filename)?.collect(),
        })
    }

}

#[cfg(test)]
mod test {
    use super::*;
    use aoc::day::Day;

    #[test]
    fn test_part1() {
        assert_eq!(
            Day2 {
                moves: [
                    "forward 5",
                    "down 5",
                    "forward 8",
                    "up 3",
                    "down 8",
                    "forward 2"
                ]
                .iter()
                .map(|s| s.parse().unwrap())
                .collect()
            }
            .part1()
            .unwrap(),
            150
        );
    }

    #[test]
    fn test_part2() {
        assert_eq!(
            Day2 {
                moves: [
                    "forward 5",
                    "down 5",
                    "forward 8",
                    "up 3",
                    "down 8",
                    "forward 2"
                ]
                .iter()
                .map(|s| s.parse().unwrap())
                .collect()
            }
            .part2()
            .unwrap(),
            900
        );
    }
}

#[derive(Debug, Default, Clone, Copy)]
struct point {
    x: i32,
    y: i32,
}

impl point {
    pub fn new(x: i32, y: i32) -> Self {
        Self{ x,y }
    }
    pub fn answer(&self) -> i32 {
        self.x * self.y
    }
}

impl Add<point> for point {
    type Output = point;

    fn add(self, rhs: point) -> Self::Output {
        Self {
            x: self.x + rhs.x,
            y: self.y + rhs.y,
        }
    }
}

impl Sum for point {
    fn sum<I: Iterator<Item = Self>>(iter: I) -> Self {
        let mut sum = Self::new(0,0);
        for p in iter {
            sum.x += p.x;
            sum.y += p.y;
        }
        sum
    }
}

impl Display for point {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "({}, {})", self.x, self.y)
    }
}
