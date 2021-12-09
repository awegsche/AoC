use std::fmt::Display;

use aoc::day::Challenge;
use aoc::day::Day;
use aoc_macros::Day;
use log::info;
use log::trace;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Mark {
    LowPoint,
    Wall,
    Marked,
    Unmarked,
}

#[derive(Day)]
#[day = 9]
#[year = "2021"]
#[title = "Smoke Basin"]
#[part1 = 15]
#[part2 = 1134]
pub(crate) struct Day9 {
    width: i64,
    height: i64,
    field: Vec<u8>,
    marked: Vec<Mark>,
}

impl Day9 {
    pub fn index(&self, x: i64, y: i64) -> Option<usize> {
        if x < 0 || y < 0 || x >= self.width || y >= self.height {
            None
        } else {
            Some((y * self.width + x) as usize)
        }
    }
    pub fn get(&self, x: i64, y: i64) -> u8 {
        if let Some(i) = self.index(x, y) {
            unsafe { *self.field.get_unchecked(i) }
        } else {
            255
        }
    }
    pub fn find_low_points(&mut self) -> Vec<(i64, i64, i32)> {
        let mut marked = Vec::new();
        for y in 0..self.height {
            for x in 0..self.width {
                let value = self.get(x, y);
                let index = self.index(x, y).unwrap();
                if self.get(x - 1, y) > value
                    && self.get(x + 1, y) > value
                    && self.get(x, y - 1) > value
                    && self.get(x, y + 1) > value
                {
                    self.marked[index] = Mark::LowPoint;
                    marked.push((x, y, value as i32));
                }
                if value == 9 {
                    self.marked[index] = Mark::Wall;
                }
            }
        }
        info!("{:?}", marked);
        marked
    }

    fn mark_bassin(&mut self, x: i64, y: i64) -> i32 {
        if let Some(index) = self.index(x,y) {
        let mark = self.marked[index];
        if self.get(x, y) > 8 || mark == Mark::Wall || mark == Mark::Marked {
            return 0;
        }
        trace!("check ({}, {})", x, y);
        self.marked[index] = Mark::Marked;

        1
            + self.mark_bassin(x + 1, y)
            + self.mark_bassin(x - 1, y)
            + self.mark_bassin(x, y - 1)
            + self.mark_bassin(x, y + 1)
        }
        else {
            0
        }
    }
}

impl Challenge<i32> for Day9 {
    fn part1(&mut self) -> Result<i32, aoc::AocError> {
        Ok(self.find_low_points().iter().map(|(_, _, v)| *v + 1).sum())
    }

    fn part2(&mut self) -> Result<i32, aoc::AocError> {
        let low_points = self.find_low_points();

        let mut biggest = 0;
        let mut bassin_sizes: Vec<i32> = low_points.iter().map(|(x,y,v)| {self.mark_bassin(*x, *y) }).collect();
        bassin_sizes.sort();
        bassin_sizes.reverse();

        info!("{:?}", &bassin_sizes[..3]);

        Ok(bassin_sizes.iter().take(3).product())
    }

    fn parse_input(filename: &str) -> Result<Self, aoc::AocError> {
        let mut lines = aoc::lines(filename)?;
        let first_line = lines.next().unwrap();
        let mut field: Vec<u8> = first_line
            .chars()
            .map(|x| x.to_digit(10).unwrap() as u8)
            .collect();
        let width = field.len() as i64;
        let mut height = 1;

        for line in lines {
            field.extend(line.chars().map(|x| x.to_digit(10).unwrap() as u8));
            height += 1;
        }

        let marked = vec![Mark::Unmarked; field.len()];

        Ok(Self {
            width,
            height,
            field,
            marked,
        })
    }
}

pub(crate) fn play() {
    let mut day = Day9::from_input().unwrap();
    println!("{}", day.part1().unwrap());
    println!("{}", day);
}

// -------------------------------------------------------------------------------------------------
// --- utils ---------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
//
const CLRS: [&str; 10] = [
    "\x1b[38;2;50;50;50m",
    "\x1b[38;2;70;70;70m",
    "\x1b[38;2;90;90;90m",
    "\x1b[38;2;110;110;110m",
    "\x1b[38;2;130;130;130m",
    "\x1b[38;2;150;150;150m",
    "\x1b[38;2;170;170;170m",
    "\x1b[38;2;190;190;190m",
    "\x1b[38;2;210;210;210m",
    "\x1b[38;2;220;220;220m",
];

impl Display for Day9 {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        for y in 0..self.height {
            for x in 0..self.width {
                let value = self.get(x, y);
                let marked = self.marked[self.index(x, y).unwrap()];

                match marked {
                    Mark::LowPoint => {
                        write!(f, "\x1b[41m{}{}\x1b[0m", CLRS[value as usize], value)?
                    }
                    Mark::Wall => write!(f, "\x1b[42m{}{}\x1b[0m", CLRS[value as usize], value)?,
                    _ => write!(f, "{}{}\x1b[0m", CLRS[value as usize], value)?,
                };
            }
            writeln!(f, "")?;
        }
        Ok(())
    }
}
