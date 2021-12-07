use std::time::Instant;

use sdl2::{
    render::Canvas,
    video::{SwapInterval, Window},
};

// reexport sdl
pub use sdl2;

const MOVE_SPEED: f32 = 500.0;

pub struct SdlWindow {
    pub event_pump: sdl2::EventPump,
    //video_subsystem: VideoSubsystem,
    pub canvas: Canvas<Window>,
    redraw: bool,
    //ctx: GLContext,
    last_fps_time: Instant,
    frames_since_last: u32,
    total_frames: u32,
    pub offset: (f32, f32),
}

impl SdlWindow {
    pub fn new(title: &str, width: u32, height: u32) -> Result<Self, String> {
        let sdl_context = sdl2::init()?;
        let video_subsystem = sdl_context.video()?;
        let gl_attr = video_subsystem.gl_attr();
        gl_attr.set_context_profile(sdl2::video::GLProfile::Core);

        gl_attr.set_double_buffer(true);
        gl_attr.set_multisample_samples(4);
        gl_attr.set_context_version(3, 3);

        let window = video_subsystem
            .window(title, width, height)
            .opengl()
            .resizable()
            .build()
            .or_else(|s| Err(format!("error building SDL window: {}", s)))?;

        let _ctx = window.gl_create_context().unwrap();
        video_subsystem
            .gl_set_swap_interval(SwapInterval::VSync)
            .unwrap();

        let canvas = window
            .into_canvas()
            .accelerated()
            .present_vsync()
            .build()
            .or_else(|s| Err(format!("error building canvas: {}", s)))?;

        let event_pump: sdl2::EventPump = sdl_context.event_pump()?;

        Ok(Self {
            event_pump,
            //video_subsystem,
            canvas,
            //ctx,
            redraw: true,
            frames_since_last: 0,
            total_frames: 0,
            last_fps_time: Instant::now(),
            offset: (0.0, 0.0),
        })
    }

    ///
    pub fn draw<F>(&mut self, content: F)
    where
        F: Fn(&mut Canvas<Window>, (f32, f32)),
    {
        content(&mut self.canvas, self.offset);
    }

    pub fn movement(&mut self, dt: f32) {
        if self
            .event_pump
            .keyboard_state()
            .is_scancode_pressed(sdl2::keyboard::Scancode::W)
        {
            self.offset.1 += MOVE_SPEED * dt;
        }
        if self
            .event_pump
            .keyboard_state()
            .is_scancode_pressed(sdl2::keyboard::Scancode::S)
        {
            self.offset.1 -= MOVE_SPEED * dt;
        }
        if self
            .event_pump
            .keyboard_state()
            .is_scancode_pressed(sdl2::keyboard::Scancode::A)
        {
            self.offset.0 += MOVE_SPEED * dt;
        }
        if self
            .event_pump
            .keyboard_state()
            .is_scancode_pressed(sdl2::keyboard::Scancode::D)
        {
            self.offset.0 -= MOVE_SPEED * dt;
        }
    }

    /// swaps the buffers (makes all render commands visible).
    /// This function also keepd track of fps
    pub fn present(&mut self) {
        if !self.redraw {
            return;
        }
        self.canvas.present();
        self.total_frames += 1;
        self.frames_since_last += 1;
        let dt = self.last_fps_time.elapsed().as_secs_f32();
        if dt > 0.1 {
            let fps = self.frames_since_last as f32 / dt;
            //println!("fps: {:.2}", fps);
            self.last_fps_time = Instant::now();
            self.frames_since_last = 0;
        }
    }
}
