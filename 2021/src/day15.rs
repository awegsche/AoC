use std::cmp::Reverse;
use std::collections::{BinaryHeap, HashMap};
use std::time::Instant;

use aoc::array2d::{Array2D, Index2Du32};
use aoc::day::{Challenge, Day};
use aoc_macros::Day;
use log::info;

#[derive(Day)]
#[day = 15]
#[year = "2021"]
#[part1 = 40]
#[part2 = 40]
#[title = "Chitons"]
pub(crate) struct Day15 {
    field: Array2D<u8>,
}

struct Path {
    pub positions: Vec<Index2Du32>,
}

#[derive(Debug, Clone)]
struct Vertex {
    pub position: Index2Du32,
    pub neighbours: Vec<Index2Du32>,
}

impl Vertex {
    pub fn new(position: Index2Du32, neighbours: Vec<Index2Du32>) -> Self {
        Self {
            position,
            neighbours,
        }
    }
}
fn dijkstra_binearyheap(field: &Array2D<u8>) -> u32 {
    info!("start Dijkstra with BinaryHeap");

    info!("setting up height hashmap and best hashmap");
    let mut starttime = Instant::now();
    let mut map: HashMap<(i32, i32), u8> = HashMap::new();
    let mut best: HashMap<(i32, i32), u32> = HashMap::new();

    for x in 0..field.width() {
        for y in 0..field.height() {
            map.insert((x as i32, y as i32), field[(x, y)]);
            best.insert((x as i32, y as i32), u32::max_value());
        }
    }

    // add borders
    for x in 0..field.width() {
        map.insert((x as i32, -1), 255);
        map.insert((x as i32, field.height() as i32), 255);
    }
    for y in 0..field.height() {
        map.insert((-1, y as i32), 255);
        map.insert((field.width() as i32, y as i32), 255);
    }
    info!(
        "time to prepare: {:.3}ms",
        starttime.elapsed().as_secs_f64() * 1.0e3
    );
    starttime = Instant::now();

    let mut visited = BinaryHeap::new();

    visited.push((Reverse(0), (0, 0)));

    while let Some((Reverse(cost), (x, y))) = visited.pop() {
        if cost < best[&(x, y)] {
            best.insert((x, y), cost);
            for (dx, dy) in [(-1, 0), (1, 0), (0, -1), (0, 1)] {
                let _x = x + dx;
                let _y = y + dy;
                if _x >= 0 && _y >= 0 && _x < field.width() as i32 && _y < field.height() as i32 {
                    visited.push((Reverse(cost + map[&(_x, _y)] as u32), (_x, _y)));
                }
            }
        }
    }
    info!(
        "time for performing the search: {:.3}ms",
        starttime.elapsed().as_secs_f64() * 1.0e3
    );

    best[&(field.width() as i32 - 1, field.height() as i32 - 1)]
}

fn dijkstra(field: &Array2D<u8>) -> (Array2D<usize>, Array2D<usize>) {
    info!("start Dijkstra");
    let mut starttime = Instant::now();
    let n = field.width() * field.height();
    let w = field.width();
    let h = field.height();

    let mut q_data = Vec::with_capacity(n);

    for y in 0..h as u32 {
        for x in 0..w as u32 {
            let mut neighbours = Vec::new();
            if x > 0 {
                neighbours.push(Index2Du32(x - 1, y))
            }
            if x + 1 < w as u32 {
                neighbours.push(Index2Du32(x + 1, y))
            }
            if y > 0 {
                neighbours.push(Index2Du32(x, y - 1))
            }
            if y + 1 < h as u32 {
                neighbours.push(Index2Du32(x, y + 1))
            }
            q_data.push(Vertex::new(Index2Du32(x as u32, y as u32), neighbours));
        }
    }
    let vertex_list = Array2D::from_vector_and_shape(q_data, w, h);
    let mut visited = Array2D::from_vector_and_shape(vec![false; n], w, h);
    let mut dist = Array2D::from_vector_and_shape(vec![usize::max_value(); n], w, h);
    let mut prev = Array2D::from_vector_and_shape(vec![0usize; n], w, h);

    info!(
        "time for setting up the data structures: {:.3} ms",
        starttime.elapsed().as_secs_f64() * 1.0e3
    );

    starttime = Instant::now();

    dist[(0, 0)] = 0;

    while let Some((u, &u_dist)) = dist
        .iter()
        .enumerate()
        .filter(|(i, _)| !visited[*i])
        .min_by(|(_, a), (_, b)| a.cmp(b))
    {
        visited[u] = true;

        //if u == n-1 { break; }

        for v_ind in vertex_list[u].neighbours.iter() {
            if visited[*v_ind] {
                continue;
            };
            let v = &vertex_list[*v_ind];
            let new_dist = u_dist + field[v.position] as usize;
            if new_dist < dist[v.position] {
                dist[v.position] = new_dist;
                prev[v.position] = u;
            }
        }
    }
    info!(
        "time for performing the search: {:.3} ms",
        starttime.elapsed().as_secs_f64() * 1.0e3
    );
    (dist, prev)
}

impl Challenge<usize> for Day15 {
    fn part1(&mut self) -> Result<usize, aoc::AocError> {
        let (dist, prev) = dijkstra(&self.field);
        let n = self.field.width() * self.field.height();

        Ok(dist[n - 1])
    }

    fn part2(&mut self) -> Result<usize, aoc::AocError> {
        let mut starttime = Instant::now();
        let w = self.field.width();
        let h = self.field.height();
        let n = w * h;
        let mut big_field = Vec::with_capacity(n * 25);

        for yf in 0..5 {
            for y in 0..h {
                for xf in 0..5 {
                    for x in 0..w {
                        let mut v = self.field[(x, y)] + yf + xf;
                        if v > 9 {
                            v = v % 10 + 1;
                        }
                        big_field.push(v);
                    }
                }
            }
        }

        let big_field = Array2D::from_vector_and_shape(big_field, w * 5, h * 5);
        info!(
            "setup big_field: {}",
            starttime.elapsed().as_secs_f64() * 1.0e3
        );

        //info!("first try with array2d");
        //info!("----------------------");
        //let (dist, prev) = dijkstra(&big_field);

        info!("second try with binaryheap");
        info!("--------------------------");
        let result = dijkstra_binearyheap(&big_field);
        Ok(result as usize)
    }

    fn parse_input(filename: &str) -> Result<Self, aoc::AocError> {
        let mut lines = aoc::lines(filename)?;

        Ok(Self {
            field: Array2D::from_lines(&mut lines, |board, line| {
                board.extend(line.chars().map(|c| c as u8 - '0' as u8));
                true
            }),
        })
    }
}
