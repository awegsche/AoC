use aoc::{
    day::{Challenge, Day},
    AocError,
};
use aoc_macros::Day;
use lazy_regex::regex_captures;
use log::info;
use std::{fmt::Display, str::FromStr};

#[derive(Day)]
#[day = 5]
#[year = "2021"]
#[part1 = 5]
#[part2 = 12]
struct Day5 {
    vents: Vec<Vent>,
}
impl Challenge<i32> for Day5 {
    fn part1(&mut self) -> Result<i32, aoc::AocError> {
        // get min and max to initialize the field
        let max_x = self.vents.iter().map(|v| v.x0.max(v.x1)).max().unwrap() + 2;
        let max_y = self.vents.iter().map(|v| v.y0.max(v.y1)).max().unwrap() + 2;

        let n = (max_x * max_y) as usize;
        info!("going to allocate {} bytes of memory", n);
        if n > (2 << 30) {
            return Err(AocError::new(
                "field size exceeds would 1GiB of data. we don't do that",
            ));
        }

        let mut field = Vec::with_capacity(n);
        for _ in 0..n {
            field.push(0);
        }

        for vent in self.vents.iter() {
            // horizontal
            let Vent { x0, y0, x1, y1 } = *vent;
            if vent.y0 == vent.y1 {
                for x in x0.min(x1)..=x0.max(x1) {
                    field[index(x, y0, max_x)] += 1;
                }
            }
            // vertical
            else if vent.x0 == vent.x1 {
                for y in y0.min(y1)..=y0.max(y1) {
                    field[index(vent.x0, y, max_x)] += 1;
                }
            }
        }

        info!("vents: {}", self.vents.len());
        let count = field.iter().filter(|x| **x > 1).count() as i32;
        Ok(count)
    }

    fn part2(&mut self) -> Result<i32, aoc::AocError> {
        // get min and max to initialize the field
        let max_x = self.vents.iter().map(|v| v.x0.max(v.x1)).max().unwrap() + 2;
        let max_y = self.vents.iter().map(|v| v.y0.max(v.y1)).max().unwrap() + 2;

        let n = (max_x * max_y) as usize;
        info!("going to allocate {} bytes of memory", n);
        if n > (2 << 30) {
            return Err(AocError::new(
                "field size exceeds would 1GiB of data. we don't do that",
            ));
        }

        let mut field = Vec::with_capacity(n);
        for _ in 0..n {
            field.push(0);
        }

        for vent in self.vents.iter() {
            // horizontal
            let Vent { x0, y0, x1, y1 } = *vent;
            if vent.y0 == vent.y1 {
                //println!("add horizontal vent: {}", vent);
                for x in x0.min(x1)..=x0.max(x1) {
                    field[index(x, y0, max_x)] += 1;
                }
            }
            // vertical
            else if vent.x0 == vent.x1 {
                //println!("add vertical vent: {}", vent);
                for y in y0.min(y1)..=y0.max(y1) {
                    field[index(vent.x0, y, max_x)] += 1;
                }
            }
            // diagonal
            else if (x0 - x1).abs() == (y1 - y0).abs() {
                let x_step = if x1 > x0 { 1 } else { -1 };
                let y_step = if y1 > y0 { 1 } else { -1 };
                for (x, y) in num::range_step(x0, x1 + x_step, x_step).zip(num::range_step(
                    y0,
                    y1 + y_step,
                    y_step,
                )) {
                    field[index(x, y, max_x)] += 1;
                }
            }
        }

        info!("vents: {}", self.vents.len());
        let count = field.iter().filter(|x| **x > 1).count() as i32;
        Ok(count)
    }

    fn parse_input(filename: &str) -> Result<Self, aoc::AocError> {
        Ok(Self {
            vents: aoc::parse_lines(filename)?.collect(),
        })
    }
}

// -------------------------------------------------------------------------------------------------
// --- Vent ----------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------

#[derive(Clone, Copy)]
struct Vent {
    pub x0: i32,
    pub y0: i32,
    pub x1: i32,
    pub y1: i32,
}

impl FromStr for Vent {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let (_, x0, y0, x1, y1) = regex_captures!(r#"(\d+),(\d+)\s+->\s+(\d+),(\d+)"#, s).unwrap();

        Ok(Self {
            x0: x0.parse().unwrap(),
            y0: y0.parse().unwrap(),
            x1: x1.parse().unwrap(),
            y1: y1.parse().unwrap(),
        })
    }
}

fn index(x: i32, y: i32, width: i32) -> usize {
    (x + y * width) as usize
}

fn print_field(field: &[u8], w: i32, h: i32) {
    for y in 0..h {
        for x in 0..w {
            print!("{} ", field[index(x, y, w)]);
        }
        println!("");
    }
}

impl Display for Vent {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "({},{}) -> ({},{})", self.x0, self.y0, self.x1, self.y1)
    }
}

fn main() {
    Day5::run().unwrap();
}
