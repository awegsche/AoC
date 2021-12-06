use super::AocError;
use std::{
    fmt::Display,
    time::Instant,
};

const STAR_COLOR: &str = "\x1b[1m\x1b[38;2;255;255;100m";
const ERROR_COLOR: &str = "\x1b[1m\x1b[38;2;255;100;100m";

/// -------------------------------------------------------------------------------------------------
/// ---- AoC trait ----------------------------------------------------------------------------------
/// -------------------------------------------------------------------------------------------------
pub trait Day<T: Display>: Sized {
    fn part1(&mut self) -> Result<T, AocError>;
    fn part2(&mut self) -> Result<T, AocError>;

    fn parse_input(filename: &str) -> Result<Self, AocError>;

    fn name() -> &'static str;

    fn filename_input() -> String {
        format!("inputs/{}_input.txt", Self::name())
    }
    fn filename_test() -> String {
        format!("inputs/{}_test_input.txt", Self::name())
    }

    fn filename_from_test() -> Result<Box<dyn Iterator<Item = String>>, std::io::Error> {
        Ok(Box::new(super::lines(&format!(
            "inputs/{}_test_input.txt",
            Self::name()
        ))?))
    }

    fn from_input() -> Result<Self, AocError> {
        Self::parse_input(&Self::filename_input())
    }

    fn from_test() -> Result<Self, AocError> {
        Self::parse_input(&Self::filename_test())
    }

    fn run() -> Result<(), AocError> {
        let mut aoc = Self::from_input()?;
        println!("+-------------------------------------------------+");
        println!(
            "| Running \x1b[1m{:6} \x1b[0m                                 |",
            Self::name()
        );
        println!("|----------------------------------------+--------|");

        let starttime = Instant::now();
        let part1_res = match aoc.part1() {
            Ok(res) => format!("{}{:<32}\x1b[0m", STAR_COLOR, res),
            Err(err) => format!("{}{:<32}\x1b[0m", ERROR_COLOR, err),
        };
        let elapsed = Instant::now() - starttime;
        println!("| part1: {}. \x1b[1m{:5.1}\x1b[0mms|", part1_res, elapsed.as_secs_f32() * 1.0e3);
        println!("|                                        .        |");

        let starttime = Instant::now();
        let part2_res = match aoc.part2() {
            Ok(res) => format!("{}{:<32}\x1b[0m", STAR_COLOR, res),
            Err(err) => format!("{}{:<32}\x1b[0m", ERROR_COLOR, err),
        };
        let elapsed = Instant::now() - starttime;
        println!("| part2: {}. \x1b[1m{:5.1}\x1b[0mms|", part2_res, elapsed.as_secs_f32() * 1.0e3);
        println!("+----------------------------------------+--------+");
        Ok(())
    }
}
