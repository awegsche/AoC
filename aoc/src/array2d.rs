use std::{
    ops::{Index, IndexMut},
    str::FromStr,
};

pub struct Array2D<T> {
    data: Vec<T>,
    width: usize,
    height: usize,
}

impl<T> Array2D<T> {
    pub fn with_capacity(n: usize) -> Self {
        Self {
            data: Vec::with_capacity(n),
            width: 0,
            height: 0,
        }
    }

    pub fn from_vector_and_shape(data: Vec<T>, width: usize, height: usize) -> Self {
        Self {
            data,
            width,
            height,
        }
    }

    pub fn width(&self) -> usize {
        self.width
    }

    pub fn height(&self) -> usize {
        self.height
    }

    pub fn data(&self) -> &Vec<T> {
        &self.data
    }

    pub fn data_mut(&mut self) -> &mut Vec<T> {
        &mut self.data
    }
}

impl<T> Array2D<T>
where
    T: FromStr,
{
    pub fn from_lines<It, F>(lines: &mut It, line_predicate: F) -> Self
    where
        It: Iterator<Item = String>,
        F: Fn(&mut Vec<T>, String) -> bool,
    {
        let first_line = lines.next().unwrap();

        let mut data: Vec<T> = Vec::new();
        line_predicate(&mut data, first_line);
        let width = data.len();
        let mut height = 1;
        while let Some(line) = lines.next() {
            if !line_predicate(&mut data, line) {
                break;
            }
            height += 1;
        }

        Self {
            data,
            width,
            height,
        }
    }
}

impl<T> Index<(usize, usize)> for Array2D<T> {
    type Output = T;

    fn index(&self, index: (usize, usize)) -> &Self::Output {
        &self.data[get_index(index.0, index.1, self.width)]
    }
}

impl<T> IndexMut<(usize, usize)> for Array2D<T> {
    fn index_mut(&mut self, index: (usize, usize)) -> &mut Self::Output {
        &mut self.data[get_index(index.0, index.1, self.width)]
    }
}

fn get_index(x: usize, y: usize, width: usize) -> usize {
    x + width * y
}
