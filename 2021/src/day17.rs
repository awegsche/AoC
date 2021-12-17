use std::{str::FromStr, time::Instant};

use aoc::{
    day::{Challenge, Day},
    sdl::sdl2::{
        self,
        rect::{Point, Rect},
    },
    AocError,
};
use aoc_macros::Day;
use lazy_regex::regex_captures;
use log::info;

const VER_ZOOM: f32 = 10.0;
const HOR_ZOOM: f32 = 10.0;

#[derive(Day)]
#[day = 17]
#[year = "2021"]
#[part1 = 45]
#[part2 = 112]
#[title = "Trick Shot"]
pub(crate) struct Day17 {
    x0: i32,
    x1: i32,
    y0: i32,
    y1: i32,
}

fn check_int(x: i32, t: i32) -> Option<i32> {
    let num = 2 * x + t * t - t;
    let den = 2 * t;
    if num % den == 0 {
        Some(num/den)
    }
    else {
        None
    }
}

impl Challenge<i32> for Day17 {
    fn part1(&mut self) -> Result<i32, aoc::AocError> {
        let t = -self.y0 * 2;
        let vy = -self.y0 - 1;
        let tmax = vy + 1;
        let ymax = vy * tmax - tmax * (tmax - 1) / 2;
        info!("vy = {}", vy);
        info!("t = {}", t);

        Ok(ymax)
    }

    fn part2(&mut self) -> Result<i32, aoc::AocError> {
        let mut vys = Vec::new();
        let mut vxs = Vec::new();
        for y in self.y0..=self.y1 {
            for t in 1..=(-y) {
                if let Some(vy) = check_int(y, t) {
                    vys.push((vy,t));
                }
            }
            let vy = -y-1;
            let t = -y*2;
            vys.push((vy,t));

        }
        for x in self.x0..=self.x1 {
            for t in 1..=x {
                if let Some(vx) = check_int(x, t) {
                    vxs.push((vx,t));
                }
            }
            for vx in -x..=x {
                if vx*vx/2+vx == x {
                    vxs.push((vx, vx+1));
                }
            }
        }
        //vxs.sort_by(|(a,_),(b,_)| a.cmp(b));
        //vys.sort_by(|(a,_),(b,_)| a.cmp(b));
        //vxs.dedup_by(|(a,_),(b,_)| a==b);
        //vys.dedup_by(|(a,_),(b,_)| a==b);
        vxs.sort();
        vys.sort();
        vxs.dedup();
        vys.dedup();
        //println!("{:?}", vys);
        //println!("{:?}", vxs);
        let mut vs = Vec::new();
        for (vx,tx) in vxs.iter() {
            for (vy,ty) in vys.iter() {
                if tx<=ty {
                    vs.push((vx,vy));
                }
            }
        }
        vs.sort();
        vs.dedup();
        //println!("{:?}", vs);
        Ok(vs.len() as i32)
    }

    fn parse_input(filename: &str) -> Result<Self, aoc::AocError> {
        aoc::lines(filename)?.next().unwrap().parse()
    }
}

impl FromStr for Day17 {
    type Err = AocError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let (_, x0, x1, y0, y1) =
            regex_captures!("x=(-?\\d+)..(-?\\d+), y=(-?\\d+)..(-?\\d+)", s).unwrap();

        Ok(Self {
            x0: x0.parse().unwrap(),
            x1: x1.parse().unwrap(),
            y0: y0.parse().unwrap(),
            y1: y1.parse().unwrap(),
        })
    }
}

impl Day17 {
    fn at_time(&self, t_end: i32, vx: i32, vy: i32) -> (i32, i32) {
        let mut pos = (0, 0);
        let mut vx = vx;
        let mut vy = vy;
        for t in 0..t_end {
            pos.0 += vx;
            pos.1 += vy;
            if vx > 0 {
                vx -= 1;
            }
            vy -= 1;
        }
        pos
    }
}

fn print_eq(y0: i32, y1: i32) {
    for y in y0..=y1 {
        for t in 1..=2 * y.abs() {
            let num = 2 * y + t * t - t;
            let den = 2 * t;
            if num % den == 0 {
                println!("y={}, t={}, {}", y, t, num / den);
            }
        }
    }
}
