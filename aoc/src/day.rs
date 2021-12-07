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
pub trait Challenge<T: Display>: Sized + Day {
    fn part1(&mut self) -> Result<T, AocError>;
    fn part2(&mut self) -> Result<T, AocError>;

    fn parse_input(filename: &str) -> Result<Self, AocError>;

    fn from_input() -> Result<Self, AocError> {
        Self::parse_input(&Self::filename_input())
    }

    fn from_test() -> Result<Self, AocError> {
        eprintln!("'{}'", Self::filename_test());
        Self::parse_input(&Self::filename_test())
    }

    fn run() -> Result<(), AocError> {
        let mut aoc = Self::from_input()?;
        println!("+-------------------------------------------------+");
        println!(
            "| Running \x1b[1mDay{:2} \x1b[0m                                  |",
            Self::day()
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

/// -------------------------------------------------------------------------------------------------
/// ---- trait for macro impl -----------------------------------------------------------------------
/// -------------------------------------------------------------------------------------------------
///
pub trait Day {

    /// Returns the day
    fn day() -> u8;

    /// Returns the year of the event
    fn year() -> &'static str;

    /// Returns the title of the challenge
    fn title() -> &'static str;

    /// Returns the filename of the test file
    /// e.g. `2021/inputs/day1_input.txt`
    fn filename_input() -> String {
        format!("{}/inputs/day{:02}_input.txt", Self::year(), Self::day())
    }

    /// Returns the filename of the test file
    /// e.g. `inputs/day1_test_input.txt`
    /// WARNING: apparently cargo test switches the working dir to the targets dir
    fn filename_test() -> String {
        format!("inputs/day{:02}_test_input.txt", Self::day())
    }
}
