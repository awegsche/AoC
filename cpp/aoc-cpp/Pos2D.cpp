#include "Pos2D.h"
std::ostream &operator<<(std::ostream &os, aoc::Pos2D const &p) {
  os << "(" << p.x << ", " << p.y << ")";
  return os;
}
