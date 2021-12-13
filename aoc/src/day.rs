use crate::logger;

use super::AocError;
use std::{
    fmt::Display,
    time::{Duration, Instant},
};

const STAR_COLOR: &str = "\x1b[1m\x1b[38;2;255;255;100m";
const TITLE_COLOR: &str = "\x1b[1m\x1b[38;2;100;185;255m";
const LOG_COLOR: &str = "\x1b[1m\x1b[38;2;100;255;155m";
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
        let starttime = Instant::now();
        let mut aoc = Self::from_input()?;
        let elapsed_loading = Instant::now() - starttime;
        println!("+-------------------------------------------------------------+");
        println!(
            "| Running \x1b[1mDay{:2} {}{:<46}\x1b[0m|",
            Self::day(),
            TITLE_COLOR,
            Self::title(),
        );
        println!("|----------------------------------------+----------+---------|");

        let starttime = Instant::now();
        let part1_res = match aoc.part1() {
            Ok(res) => format!("{}{:<32}\x1b[0m", STAR_COLOR, res),
            Err(err) => format!("{}{:<32}\x1b[0m", ERROR_COLOR, err),
        };
        let elapsed = Instant::now() - starttime;
        println!(
            "| part1: {}: {} : {}|",
            part1_res,
            print_time(elapsed_loading),
            print_time(elapsed)
        );
        println!("|                                        :          :         |");
        let logs1 = logger::extract_logs();

        let mut aoc = Self::from_input()?;
        let starttime = Instant::now();
        let part2_res = match aoc.part2() {
            Ok(res) => format!("{}{:<32}\x1b[0m", STAR_COLOR, res),
            Err(err) => format!("{}{:<32}\x1b[0m", ERROR_COLOR, err),
        };
        let elapsed = Instant::now() - starttime;
        println!(
            "| part2: {}: {} : {}|",
            part2_res,
            print_time(elapsed_loading),
            print_time(elapsed)
        );
        let logs2 = logger::extract_logs();

        print_logs_part(&logs1, 1);
        print_logs_part(&logs2, 2);

        if logs2.len() + logs1.len() > 0 {
            println!("+-------------------------------------------------------------+");
        } else {
            println!("+----------------------------------------+----------+---------+");
        }
        println!("");

        Ok(())
    }
}

pub fn print_logs_part(logs: &[String], part: u8) {
    if logs.len() > 0 {
        println!("|.............................................................|");
        println!(
            "| {}LOG Part {}\x1b[0m                                                  |",
            LOG_COLOR, part
        );
        for log in logs.iter() {
            if log.len() > 60 {
                println!("| {} ...|", &log[..56]);
            } else {
                println!("| {:<60}|", log);
            }
        }
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

fn print_time(dt: Duration) -> String {
    let dt_s = dt.as_secs_f64();

    if dt_s > 1.0 {
        format!("{:6.1}\x1b[38;2;128;128;128ms \x1b[0m", dt_s)
    } else if dt_s > 1.0e-3 {
        format!("{:6.1}\x1b[38;2;128;128;128mms\x1b[0m", dt_s * 1.0e3)
    } else {
        format!("{:6.1}\x1b[38;2;128;128;128mμs\x1b[0m", dt_s * 1.0e6)
    }
}
