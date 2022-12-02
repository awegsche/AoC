# <img align="center" src="media/aoc.png#gh-dark-mode-only" height=32><img align="center" src="media/aoc_inv.png#gh-light-mode-only" height=32> AoC
my solutions for [Advent of Code](https://adventofcode.com/)

## Events

- [2021](2021) [25<img src="media/aoc.png#gh-dark-mode-only" height=14><img src="media/aoc_inv.png#gh-light-mode-only" height=14>]

## Usage

### Rust

The subprojects `aoc` and `aoc_macros` define two traits `Day` and `Challenge<T>` together with a derive macro for `Day`.
The derive macro automatically creates tests and some basic implementation for the struct.

```rust

// first, define the struct and auto-derive `Day`:
#[derive(Day)]
#[day=1]        // required, defines which day we are on
#[year="2021"]  // required, defines the event (and thus the location of the input files)
#[part1=10]     // required, defines the expected output value for part1's test case
#[part2=10]     // required, defines the expected output value for part2's test case
#[title="Some title"] // optional, defines the title of the challenge
pub(crate) struct Day1 {
  // internal data (= parsed input file)
}

// then, impl `Challenge<T>` for the struct
// this takes care of input loading and contains the actual part1 and part2 algorithms
// `T` is the return type (will often be a more or less large integer)
impl Challenge<i32> for Day1 {
  fn part1(&mut self) -> Result<i32, aoc::AocError> {
    // part 1 logic
    Ok(10) // returns the expected value wrapped in a Result
  }
  
  fn parse(filename: &str) -> Result<Self, aoc::AocError> {
    // parses the input file, `filename` will be "inputs/day##_test_input.txt" for test and "<year>/inputs/day##_input.txt" for the actual run
    // here, aoc's input convenience functions may be used:
    let mut lines = aoc::lines(filename)?;
    // ..
    Ok(Self{...})
  }
}
```

### C++

