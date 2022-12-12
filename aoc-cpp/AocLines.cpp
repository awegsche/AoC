#include "AocLines.h"

std::istream &aoc::getline(std::istream &stream, std::string &line) {
  std::getline(stream, line);
  if (line.back() == '\r')
    line.pop_back();
  return stream;
}

std::istream &aoc::getline(std::istream &stream, std::string &line, char delimiter) {
  std::getline(stream, line, delimiter);
  if (line.back() == '\r')
    line.pop_back();
  return stream;
}
