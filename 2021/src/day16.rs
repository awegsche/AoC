use std::{fmt::Display, str::FromStr};
use std::fmt::{Debug, Write};

use aoc::{AocError, day::{Challenge, Day}};
use aoc_macros::Day;

#[derive(Day)]
#[day=16]
#[year="2021"]
#[part1=16]
#[part2=16]
#[title="Packet Decoder"]
pub(crate) struct Day16{
    data: Vec<u8>
}

impl Challenge<u64> for Day16 {
    fn part1(&mut self) -> Result<u64, aoc::AocError> {
        let chunk: Chunk = BitsReader::new(&self.data).parse_next()?;
        Ok(chunk.get_version() as u64)
    }

    fn part2(&mut self) -> Result<u64, aoc::AocError> {
        let chunk: Chunk = BitsReader::new(&self.data).parse_next()?;
        //println!("{}", chunk);
        Ok(chunk.eval())
    }

    fn parse_input(filename: &str) -> Result<Self, aoc::AocError> {
        aoc::lines(filename)?.next().unwrap().parse()
    }
}

struct BitsReader<'a> {
    data: &'a Vec<u8>,
    byte_pos: usize,
    bit_pos: usize,
}

trait FromBits: Sized + std::fmt::Debug {
    fn from_bits<'a, 'b>(reader: &'b mut BitsReader<'a>) -> Result<Self, AocError>;
    fn with_version(self, version: u8) -> Self;
    fn get_version(&self) -> u32;
    fn eval(&self) -> u64;
}

impl<'a> BitsReader<'a> {
    pub fn new(data: &'a Vec<u8>) -> Self {
        Self{
            data,
            byte_pos: 0,
            bit_pos: 0
        }
    }
    pub fn get_position(&self) -> usize {
        self.byte_pos * 8 + self.bit_pos
    }
    pub fn read_chunk(&mut self, size: u8) -> u32 {
        let mut value = 0;

        let mut byte = self.data[self.byte_pos];
        for i in 0..size {
            let bit = (byte >> (7-self.bit_pos)) & 1;
            value = (value << 1) | bit as u32;
            self.bit_pos += 1;
            if self.bit_pos == 8 {
                self.bit_pos = 0;
                self.byte_pos += 1;
                byte = self.data[self.byte_pos];
            }
        }

        value
    }
    pub fn peek_chunk(&mut self, size: u8) -> u32 {
        let byp = self.byte_pos;
        let bip = self.bit_pos;

        let ch = self.read_chunk(size);

        self.byte_pos = byp;
        self.bit_pos = bip;

        ch
    }

    pub fn parse_next<T: FromBits>(&mut self) -> Result<T, AocError> {
        T::from_bits(self)
    }
}

#[derive(Debug)]
enum Chunk {
    Literal(Literal),
    Operator(Operator)

}

impl FromBits for Chunk {
    fn from_bits<'a, 'b>(reader: &'b mut BitsReader<'a>) -> Result<Self, AocError> {
        let version = reader.read_chunk(3) as u8;
        let typ = reader.peek_chunk(3);

        match typ {
            4 => Ok(Chunk::Literal(Literal::from_bits(reader)?.with_version(version))),
            _ => Ok(Chunk::Operator(Operator::from_bits(reader)?.with_version(version))),
        }
    }

    fn get_version(&self) -> u32 {
        match self {
            Chunk::Literal(lit) => lit.get_version(),
            Chunk::Operator(op) => op.get_version(),
        }
    }

    fn with_version(self, version: u8) -> Self {
        self
    }

    fn eval(&self) -> u64 {
        match self {
            Chunk::Literal(Literal(v, _)) => *v,
            Chunk::Operator(op) => op.eval(),
        }
    }
}

#[derive(Debug)]
struct Operator{
    typ: u8,
    version: u8,
    sub_packets: Vec<Chunk>
}

impl FromBits for Operator{
    fn from_bits<'a, 'b>(reader: &'b mut BitsReader<'a>) -> Result<Self, AocError> {
        let typ = reader.read_chunk(3) as u8;
        let length = reader.read_chunk(1);
        let mut sub_packets = Vec::new();

        if length == 1 {
            let count = reader.read_chunk(11);
            for _ in 0..count {
                sub_packets.push(reader.parse_next()?);

            }
        }
        else if length == 0 {
            let nbits = reader.read_chunk(15);
            let end_pos = reader.get_position() + nbits as usize;

            while reader.get_position() < end_pos {
                sub_packets.push(reader.parse_next()?);
            }
        }
        else {
            return Err(AocError::new("operator type must be 0 or 1"));
        }

        Ok(Self{
            version: 0,
            typ,
            sub_packets
        })


    }

    fn get_version(&self) -> u32 {
        self.sub_packets.iter().map(|p| p.get_version()).sum::<u32>() + self.version as u32
    }

    fn with_version(mut self, version: u8) -> Self {
        self.version = version;
        self
    }

    fn eval(&self) -> u64 {
        match self.typ {
            0 => self.sub_packets.iter().map(|p| p.eval()).sum(),
            1 => self.sub_packets.iter().map(|p| p.eval()).product(),
            2 => self.sub_packets.iter().map(|p| p.eval()).min().unwrap(),
            3 => self.sub_packets.iter().map(|p| p.eval()).max().unwrap(),
            5 => if self.sub_packets[0].eval() > self.sub_packets[1].eval() { 1 } else {0},
            6 => if self.sub_packets[0].eval() < self.sub_packets[1].eval() { 1 } else {0},
            7 => if self.sub_packets[0].eval() == self.sub_packets[1].eval() { 1 } else {0},
            _ => 0
        }
    }

}

#[derive(Debug)]
struct Literal(u64, u8);

impl FromBits for Literal {
    fn from_bits<'a, 'b>(reader: &'b mut BitsReader<'a>) -> Result<Self, AocError> {
        let mut value = 0;
        reader.read_chunk(3); // skip over the type
        for _ in 0..16 {
            let last = reader.read_chunk(1);
            let v = reader.read_chunk(4) as u64;
            value = (value << 4) | v;
            if last == 0 {
                return Ok(Self(value, 0));
            }
        }

        Err(AocError::new("sorry, didn't expect this to be longer than 32 bit"))
    }

    fn get_version(&self) -> u32 {
        self.1 as u32
    }

    fn with_version(mut self, version: u8) -> Self {
        self.1 = version;
        self
    }

    fn eval(&self) -> u64 {
        self.0
    }
}




impl Day16 {
    pub fn info(&self) -> String {
        let mut info = String::new();

        let mut reader = BitsReader::new(&self.data);

        let chunk: Chunk = reader.parse_next().unwrap();
        println!("chunk: {:?}", chunk);
        println!("sum: {}", chunk.get_version());

        info

    }
}

impl FromStr for Day16 {
    type Err = aoc::AocError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let bytes: Vec<u8> = s.chars().map(|b| b.to_digit(16).unwrap() as u8).collect();
        Ok( Self{
            data: bytes.chunks(2).map(|ch| ch[0] << 4 | ch[1]).collect()
        } )
    }
}

const MESSAGE1: &str = "D2FE28";
const MESSAGE2: &str = "8A004A801A8002F478";

pub(crate) fn examples() {
    let message: Day16 = MESSAGE2.parse().unwrap();

    println!("{}", message.info());
}


impl Display for Chunk {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Chunk::Literal(Literal(v, _)) => write!(f, "{}", v),
            Chunk::Operator(op) => write!(f, "{}", op),
        }
    }
}

struct DebugDisplay<'a, T> {
    display: &'a T
}

impl<'a, T> Debug for DebugDisplay<'a, T>
where T: Display {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.display)
    }
}

impl<'a, T> DebugDisplay<'a, T> {
    pub fn new(display: &'a T) -> Self {
        Self{
            display
        }
    }
}

impl Display for Operator {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self.typ {
            0 => {
                write!(f, "Sum")?;
                f.debug_list().entries(self.sub_packets.iter().map(|p| DebugDisplay::new(p))).finish()?;
            },
            1 => {
                write!(f, "Product")?;
                f.debug_list().entries(self.sub_packets.iter().map(|p| DebugDisplay::new(p))).finish()?;
            },
            2 => {
                write!(f, "Min")?;
                f.debug_list().entries(self.sub_packets.iter().map(|p| DebugDisplay::new(p))).finish()?;
            },
            3 => {
                write!(f, "Max")?;
                f.debug_list().entries(self.sub_packets.iter().map(|p| DebugDisplay::new(p))).finish()?;
            },
            5 => {
                write!(f, "( {} > {} ) ", self.sub_packets[0], self.sub_packets[1])?;
            },
            6 => {
                write!(f, "( {} < {} ) ", self.sub_packets[0], self.sub_packets[1])?;
            },
            7 => {
                write!(f, "( {} == {} ) ", self.sub_packets[0], self.sub_packets[1])?;
            },
            _ => write!(f,"")?,
        };
        Ok(())
    }
}
