use std::time::Instant;
use std::{fmt::Display, str::FromStr};

use aoc::array2d::Array2D;
use aoc::day::{Challenge, Day};
use aoc_macros::Day;
use lazy_regex::regex_captures;
use log::{info, logger};

#[derive(Day)]
#[day = 13]
#[year = "2021"]
#[part1 = 17]
#[part2 = 100]
#[title = "Transparent Origami"]
pub(crate) struct Day13 {
    //sheet: Array2D<bool>,
    points: Vec<(usize, usize)>,
    folds: Vec<Fold>,
    width: usize,
    height: usize,
    sheet: Option<Array2D<bool>>,
}

#[derive(Clone, Copy, Debug)]
enum Fold {
    X(usize),
    Y(usize),
}

impl FromStr for Fold {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let (_, dim, pos) = regex_captures!(r#"fold along ([xy])=(\d+)"#, s).unwrap();

        match dim {
            "x" => Ok(Fold::X(pos.parse().unwrap())),
            "y" => Ok(Fold::Y(pos.parse().unwrap())),
            _ => Err("fold must be along `x` or `y`".to_string()),
        }
    }
}

impl Challenge<usize> for Day13 {
    fn part1(&mut self) -> Result<usize, aoc::AocError> {
        let folds = self.folds.clone();

        let fold = folds[0];

        match fold {
            Fold::X(pos) => {
                self.fold_x(pos);
                self.width = pos;
            }
            Fold::Y(pos) => {
                self.fold_y(pos);
                self.height = pos
            }
        }

        let mut indices: Vec<usize> = self
            .points
            .iter()
            .map(|(x, y)| x + y * self.width)
            .collect();
        indices.sort();
        indices.dedup();

        Ok(indices.len())
    }

    fn part2(&mut self) -> Result<usize, aoc::AocError> {
        //println!("{}", self);

        let folds = self.folds.clone();

        for fold in folds {
            match fold {
                Fold::X(pos) => {
                    self.fold_x(pos);
                    self.width = pos;
                }
                Fold::Y(pos) => {
                    self.fold_y(pos);
                    self.height = pos
                }
            }

            //println!("{}", self);
        }

        info!("size after folding: {} {}", self.width, self.height);
        self.build_sheet();

        info!("message:");
        let sheet = self.sheet.as_ref().unwrap();
        let height = self.height;
        let width = self.width;
        for y in 0..height {
            let mut outstr = String::new();
            for x in 0..width {
                if sheet[(x, y)] {
                    //outstr += "\x1b[1m#\x1b[0m";
                    outstr.push('#');
                } else {
                    //outstr += "\x1b[38;2;128;128;128m.\x1b[0m";
                    outstr.push(' ');
                }
            }
            //println!("{}", outstr);
            info!("{}", outstr);
        }

        let mut indices: Vec<usize> = self
            .points
            .iter()
            .map(|(x, y)| x + y * self.width)
            .collect();
        indices.sort();
        indices.dedup();

        Ok(indices.len())
    }

    fn parse_input(filename: &str) -> Result<Self, aoc::AocError> {
        let mut lines = aoc::lines(filename)?;

        let mut points: Vec<(usize, usize)> = Vec::new();

        let mut max_x = 0;
        let mut max_y = 0;
        while let Some(line) = lines.next() {
            if line.is_empty() {
                break;
            }

            let (_, x, y) = regex_captures!(r#"(\d+),(\d+)"#, &line).unwrap();
            let x = x.parse().unwrap();
            let y = y.parse().unwrap();

            if x > max_x {
                max_x = x;
            }
            if y > max_y {
                max_y = y;
            }

            points.push((x, y));
        }

        info!("max_x = {}, max_y = {}", max_x, max_y);
        info!("field would have {} bools", max_x * max_y);
        let width = max_x + 1;
        let height = max_y + 1;

        let folds = lines.map(|s| s.parse().unwrap()).collect();

        Ok(Self {
            points,
            folds,
            width,
            height,
            sheet: None,
        })
    }
}

impl Day13 {
    pub fn fold_x(&mut self, position: usize) {
        info!("folding x: {}", position);
        let fold_border = 2 * position;
        for (px, _) in self.points.iter_mut() {
            if *px > position {
                *px = fold_border - *px;
            }
        }
    }
    pub fn fold_y(&mut self, position: usize) {
        info!("folding y: {}", position);
        let fold_border = 2 * position;
        for (_, py) in self.points.iter_mut() {
            if *py > position {
                *py = fold_border - *py;
            }
        }
    }

    fn build_sheet(&mut self) {
        let mut sheet = Array2D::from_vector_and_shape(
            vec![false; self.width * self.height],
            self.width,
            self.height,
        );

        for coord in self.points.iter() {
            sheet[*coord] = true;
        }

        self.sheet = Some(sheet);
    }
}

pub fn reddit() {
    println!("attempting to read");
    let loading_start = Instant::now();
    let mut day = Day13::parse_input("2021/inputs/day13_reddit.txt");
    let loading_elapsed = Instant::now() - loading_start;
    println!("loaded, elapsed: {}ms", loading_elapsed.as_secs_f64() * 1.0e3);

    match &mut day {
        Err(err) => println!("err: {}", err),
        Ok(day) => {
            let starttime = Instant::now();
            day.part2().unwrap();
            let elapsed = Instant::now() - starttime;
            let messages = aoc::logger::extract_logs();
            for m in messages.iter() {
                println!("|{:<200}|", &m[..200.min(m.len())]);
            }
            println!("elapsed: {}ms", elapsed.as_secs_f64() * 1.0e3);
        }
    }
}
