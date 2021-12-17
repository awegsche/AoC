use std::time::Instant;

use aoc;
use aoc::day::Challenge;
use aoc::day::Day;
use aoc::sdl::sdl2;
use aoc::sdl::sdl2::pixels::Color;
use aoc::sdl::sdl2::rect::Point;
use aoc_macros::Day;
use log::debug;

const VER_ZOOM: f32 = 0.1;

#[derive(Day)]
#[day = 1]
#[year = "2021"]
#[title="Sonar Sweep"]
#[part1 = 7]
#[part2 = 5]
pub(crate) struct Day1 {
    pub depths: Vec<i32>,
}

impl Challenge<i32> for Day1 {
    fn part1(&mut self) -> Result<i32, aoc::AocError> {
        Ok(self.depths.windows(2).filter(|win| win[0] < win[1]).count() as i32)
    }
    fn part2(&mut self) -> Result<i32, aoc::AocError> {
        Ok(self
            .depths
            .windows(3)
            .map(|win| win.iter().sum())
            .collect::<Vec<i32>>()
            .windows(2)
            .filter(|win| win[0] < win[1])
            .count() as i32)
    }

    fn parse_input(filename: &str) -> Result<Self, aoc::AocError> {
        let new = Self {
            depths: aoc::parse_lines(filename)?.collect(),
        };
        debug!("loaded {} elements", new.depths.len());
        Ok(new)
    }
}

pub(crate) fn play() {
    let day1 = Day1::from_input().unwrap();
    let points = day1
        .depths
        .iter()
        .enumerate()
        .map(|(i, d)| Point::new(i as i32, *d))
        .collect::<Vec<_>>();

    let mut sdl_window = aoc::sdl::SdlWindow::new("day1", 800, 600).unwrap();

    let mut starttime = Instant::now();
    'running: loop {
        let dt = starttime.elapsed().as_secs_f32();
        starttime = Instant::now();

        // exhaust the event pump:
        if let Some(event) = sdl_window.event_pump.wait_event_timeout(10) {
            match event {
                sdl2::event::Event::Quit { .. } => break 'running,
                _ => {}
            };
        }

        sdl_window.movement(dt);

        sdl_window.draw(|canvas, offset| {
            canvas.set_draw_color(Color::BLACK);
            canvas.clear();
            canvas.set_draw_color(Color::RGB(255, 255, 255));
            canvas
                .draw_lines(
                    &*points
                        .iter()
                        .map(|p| {
                            Point::new(
                                p.x + offset.0 as i32,
                                (p.y as f32 * VER_ZOOM + offset.1) as i32,
                            )
                        })
                        .collect::<Vec<Point>>(),
                )
                .unwrap();
            canvas.set_draw_color(Color::RGB(100, 100, 255));
            canvas
                .draw_line(
                    Point::new(offset.0 as i32, offset.1 as i32),
                    Point::new(points.len() as i32 + offset.0 as i32, offset.1 as i32),
                )
                .unwrap();
        });

        sdl_window.present();
    }
    println!("end");
}
