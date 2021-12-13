use std::collections::HashMap;

use aoc::day::{Challenge, Day};
use aoc_macros::Day;
use log::info;

#[derive(Day)]
#[day = 12]
#[year = "2021"]
#[title="Passage Pathing"]
#[part1 = 10]
#[part2 = 36]
pub(crate) struct Day12 {
    cave_system: HashMap<String, Vec<String>>,
}

fn is_small(cave: &str) -> bool {
    cave.chars().next().unwrap().is_lowercase()
}

fn already_one_small_twice(visited: &HashMap<&String, usize>) -> bool {
    for (key, value) in visited.iter() {
        if is_small(key) && *value > 1 {
            return true;
        }
    }
    false
}

fn explore_p1<'a>(
    cave_system: &'a HashMap<String, Vec<String>>,
    mut path: Vec<&'a String>,
    from: &'a String,
    mut visited: HashMap<&'a String, usize>,
) -> usize {
    path.push(&from);
    if from == "end" {
        //println!("{:?}", path);
        return 1;
    }

    let mut count = 0;
    for to in cave_system.get(from).unwrap() {
        let mut visited = visited.clone();
        if is_small(to) && visited[&to] > 0 {
            continue;
        }
        *visited.get_mut(&to).unwrap() += 1;

        count += explore_p1(cave_system, path.clone(), &to, visited);
    }
    count
}

fn explore_p2<'a>(
    cave_system: &'a HashMap<String, Vec<String>>,
    mut path: Vec<&'a String>,
    from: &'a String,
    mut visited: HashMap<&'a String, usize>,
) -> usize {
    path.push(&from);
    if from == "end" {
        //println!("{:?}", path);
        return 1;
    }

    let mut count = 0;
    for to in cave_system.get(from).unwrap() {
        let mut visited = visited.clone();
        if to == "start" {
            continue;
        }
        if is_small(to) {
            if already_one_small_twice(&visited) {
                if visited[&to] > 0 {
                    continue;
                }
            } else {
                if visited[&to] > 1 {
                    continue;
                }
            }
        }
        *visited.get_mut(&to).unwrap() += 1;

        count += explore_p2(cave_system, path.clone(), &to, visited);
    }
    count
}

impl Challenge<usize> for Day12 {
    fn part1(&mut self) -> Result<usize, aoc::AocError> {
        let start = "start".to_string();
        let mut visited = HashMap::new();

        for key in self.cave_system.keys() {
            visited.insert(key, 0);
        }
        *visited.get_mut(&start).unwrap() = 1;

        Ok(explore_p1(&self.cave_system, vec![], &start, visited))
    }

    fn part2(&mut self) -> Result<usize, aoc::AocError> {
        let start = "start".to_string();
        let mut visited = HashMap::new();

        for key in self.cave_system.keys() {
            visited.insert(key, 0);
        }
        *visited.get_mut(&start).unwrap() = 1;

        Ok(explore_p2(&self.cave_system, vec![], &start, visited))
    }

    fn parse_input(filename: &str) -> Result<Self, aoc::AocError> {
        let lines = aoc::lines(filename)?;
        let mut cave_system: HashMap<String, Vec<String>> = HashMap::new();

        for line in lines {
            let (from, to) = line.split_once('-').unwrap();
            cave_system
                .entry(from.to_string())
                .or_default()
                .push(to.to_string());
            cave_system
                .entry(to.to_string())
                .or_default()
                .push(from.to_string());
        }

        //println!("{:?}", cave_system);

        Ok(Self { cave_system })
    }
}
