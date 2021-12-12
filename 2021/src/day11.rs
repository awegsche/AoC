use std::fmt::Display;

use aoc::day::{Challenge, Day};
use aoc_macros::Day;

#[derive(Day)]
#[day = 11]
#[year = "2021"]
#[part1 = 1656]
#[part2 = 195]
pub(crate) struct Day11 {
    field: Vec<i32>,
    width: usize,
    height: usize,
}

fn index(x: usize, y: usize, width: usize) -> usize {
    y * width + x
}

impl Display for Day11 {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        for y in 0..self.height {
            for x in 0..self.width {
                let v = self.field[index(x + 1, y + 1, self.width + 2)];
                if v == 0 {
                    write!(f, "\x1b[1m{}\x1b[0m", v)?;
                } else {
                    write!(f, "{}", v)?;
                }
            }
            writeln!(f, "")?;
        }
        Ok(())
    }
}

fn check(field: &[i32], width: usize, x: usize, y: usize) -> bool {
    field[index(x, y, width + 2)] > 9
}

impl Day11 {
    fn step(&mut self) -> usize {
        let mut new_field = self.field.clone();

        //println!("{}", self);
        for value in new_field.iter_mut() {
            *value += 1;
        }
        let mut cascade = true;
        let mut flash = 0;

        while cascade {
            //println!("stepping");
            cascade = false;
            for y in 0..self.height {
                for x in 0..self.width {
                    let x_ = x + 1;
                    let y_ = y + 1;

                    if check(&new_field, self.width, x_, y_) {
                        flash += 1;
                        cascade = true;
                        new_field[index(x_ - 1, y_ - 1, self.width + 2)] += 1;
                        new_field[index(x_ - 1, y_, self.width + 2)] += 1;
                        new_field[index(x_ - 1, y_ + 1, self.width + 2)] += 1;
                        new_field[index(x_, y_ - 1, self.width + 2)] += 1;
                        new_field[index(x_, y_ + 1, self.width + 2)] += 1;
                        new_field[index(x_ + 1, y_ - 1, self.width + 2)] += 1;
                        new_field[index(x_ + 1, y_, self.width + 2)] += 1;
                        new_field[index(x_ + 1, y_ + 1, self.width + 2)] += 1;
                        new_field[index(x_, y_, self.width + 2)] = -1024;
                    }
                }
            }
        }

        self.field = new_field.clone();

        for value in self.field.iter_mut() {
            if *value < 0 {
                *value = 0;
            }
        }

        flash
    }
}
impl Challenge<usize> for Day11 {
    fn part1(&mut self) -> Result<usize, aoc::AocError> {
        let mut flashes = 0;
        for i in 0..100 {
            //log!("generation: {}", i);
            flashes += self.step();
        }

        Ok(flashes)
    }

    fn part2(&mut self) -> Result<usize, aoc::AocError> {
        let mut steps = 1;
        let mut flashes = 0;
        loop {
            let fl = self.step();
            log::info!("flashes: {}", fl);
            flashes += fl;
            if fl == self.width * self.height {
                break;
            }
            steps += 1;
        }
        Ok(steps)
    }

    fn parse_input(filename: &str) -> Result<Self, aoc::AocError> {
        let mut lines = aoc::lines(filename)?;
        let first_line: Vec<i32> = lines
            .next()
            .unwrap()
            .chars()
            .map(|s| (s as i32) - ('0' as i32))
            .collect();

        let width = first_line.len();
        let mut height = 1;
        let mut field: Vec<i32> = Vec::with_capacity((width + 2) * (height * 2));
        field.extend(vec![0; width + 2]);
        field.push(0);
        field.extend(first_line);
        field.push(0);

        //println!("field: {:?}", field);

        while let Some(line) = lines.next() {
            field.push(0);
            field.extend(line.chars().map(|s| (s as i32) - ('0' as i32)));
            field.push(0);
            height += 1;
        }
        field.extend(vec![0; width + 2]);

        //println!("field: {:?}", field);

        Ok(Self {
            field,
            width,
            height,
        })
    }
}
