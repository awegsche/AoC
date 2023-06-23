use std::{fmt::Display, ops::{Index, IndexMut}, slice::{Iter, IterMut}, str::FromStr};

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

    pub fn get_index(&self, x: usize, y: usize) -> usize {
        x + y * self.width
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

    pub fn iter(&self) -> Iter<T> {
        self.data.iter()
    }

    pub fn iter_mut(&mut self) -> IterMut<T> {
        self.data.iter_mut()
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

impl<T> Index<usize> for Array2D<T> {
    type Output = T;

    fn index(&self, index: usize) -> &Self::Output {
        &self.data[index]
    }
}

impl<T> IndexMut<usize> for Array2D<T> {
    fn index_mut(&mut self, index: usize) -> &mut Self::Output {
        &mut self.data[index]
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

#[derive(PartialEq, Eq, Clone, Copy, Hash, Debug, Default)]
pub struct Index2Du32(pub u32, pub u32);

impl<T> Index<Index2Du32> for Array2D<T> {
    type Output = T;

    fn index(&self, index: Index2Du32) -> &Self::Output {
        &self.data[get_index(index.0 as usize, index.1 as usize, self.width)]
    }
}

impl<T> IndexMut<Index2Du32> for Array2D<T> {
    fn index_mut(&mut self, index: Index2Du32) -> &mut Self::Output {
        &mut self.data[get_index(index.0 as usize, index.1 as usize, self.width)]
    }
}

fn get_index(x: usize, y: usize, width: usize) -> usize {
    x + width * y
}

impl<T: Display> Display for Array2D<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        for y in 0..self.height {
            for x in 0..self.width {
                write!(f, "{}|", self[(x,y)])?;
            }
            writeln!(f, "")?;
        }
        Ok(())
    }
}
