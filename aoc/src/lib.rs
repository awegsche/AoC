use std::{error::Error, fmt::Display, fs::File, io::{BufRead, BufReader}, str::FromStr};

pub mod sdl;
pub mod day;

/// -------------------------------------------------------------------------------------------------
/// ---- input parsing ------------------------------------------------------------------------------
/// -------------------------------------------------------------------------------------------------
pub fn lines(filename: &str) -> Result<impl Iterator<Item = String>, std::io::Error> {
    Ok(BufReader::new(File::open(filename)?)
        .lines()
        .filter_map(Result::ok))
}

pub fn parse_lines<T>(filename: &str) -> std::io::Result<impl Iterator<Item = T>>
where
    T: FromStr,
{
    Ok(lines(filename)?
        .map(|s| s.parse::<T>())
        .filter_map(Result::ok))
}

/// -------------------------------------------------------------------------------------------------
/// ---- AoC Error ----------------------------------------------------------------------------------
/// -------------------------------------------------------------------------------------------------
#[derive(Debug)]
pub struct AocError {
    msg: String,
}

impl AocError {
    pub fn new<T: Into<String>>(msg: T) -> AocError {
        AocError { msg: msg.into() }
    }

    pub fn not_impl<T>() -> Result<T, AocError> {
        Err(AocError { msg: "not yet implemented".to_string() })
    }
}
impl Error for AocError {}

impl Display for AocError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        //format_args!("AocError: {}", self.msg).fmt(f)
        let display = format!("AocError: {}", self.msg);
        display.fmt(f)
    }
}

impl From<std::io::Error> for AocError {
    fn from(err: std::io::Error) -> Self {
        AocError::new(format!("{}", err))
    }
}
