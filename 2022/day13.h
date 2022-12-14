
#ifndef AOC_DAY13_H
#define AOC_DAY13_H

#include <vector>
#include <variant>
#include <iostream>
#include <string>
#include <compare>

#include <AocObject.h>
#include <AocDay.h>

// [1,1,1, [1,2]]
using std::cout, std::cerr, std::endl;

struct Package: public aoc::AocObject<Package> {
   std::variant<int, std::vector<Package>> items;

   static auto get_object(std::istream& stream, Package& package) -> bool {
      package.items = 0;

      char c = 0;

      while(std::isspace(c = stream.get())) {};

      if (c == 0) {
         return false;
      }
      else if (c == '[') {
         package.items = std::vector<Package>{};
         std::vector<Package>& items = std::get<1>(package.items);

         while(true) {
            while(std::isspace(c = stream.get())) {};
            if (c == ']') {
               break;
            }
            if (c != ',')
               stream.unget();
            auto& new_package = items.emplace_back();
            get_object(stream, new_package);

         }
      }
      else if (std::isdigit(c)) {
         stream.unget();
         int i = 0;
         stream >> i;
         package.items = i;
      }
      else {
         cerr << "unexpected token '" << c << "'" << endl;
         return false;
      }
      return true;
   }

   auto operator==(Package const& other) const -> bool {
      if (items.index() != other.items.index()) return false;

      switch(items.index()) {
         case 0: return std::get<0>(items) == std::get<0>(other.items);
         case 1: {
                    std::vector<Package> const& my_items = std::get<1>(items);
                    std::vector<Package> const& other_items = std::get<1>(other.items);
                    if (my_items.size() != other_items.size()) return false;
                    for (int i = 0; i < my_items.size(); ++i) {
                       if (my_items[i] != other_items[i]) return false;
                    }
                    return true;
                 }
      }

      return false;
   }

   auto operator<(Package const& other) const -> bool {
      if (items.index() == 0 && other.items.index() == 0)
         return std::get<0>(items) < std::get<0>(other.items);

      if (items.index() == 1 && other.items.index() == 1) {
         std::vector<Package> const& my_items = std::get<1>(items);
         std::vector<Package> const& other_items = std::get<1>(other.items);

         if (my_items.size() < other_items.size()) {
            for (int i = 0; i < my_items.size(); ++i) {
               if (my_items[i] < other_items[i]) return true;
               if (other_items[i] < my_items[i]) return false;
            }
            return true;
         }
         else {
            for (int i = 0; i < other_items.size(); ++i) {
               if (my_items[i] < other_items[i]) return true;
               if (other_items[i] < my_items[i]) return false;
            }
            return false;
         }
      }

      if (items.index() == 0 && other.items.index() == 1) {
         std::vector<Package> const& other_items = std::get<1>(other.items);
         if (other_items.size() == 0) return false;
         if (other_items[0] < *this) return false;
         return true;
      }
      if (items.index() == 1 && other.items.index() == 0) {
         std::vector<Package> const& my_items = std::get<1>(items);
         if (my_items.size() == 0) return true;
         return my_items[0] < other;
      }
      return true;
   }

};

std::ostream& operator<<(std::ostream& os, Package const& package) {
   switch (package.items.index()) {
      case 0: return os << std::get<0>(package.items);
      case 1: {
                 os << "[";
                 for (Package const& p: std::get<1>(package.items)) {
                    os << p << ",";
                 }
                 os << "]";
              }
   }
   return os;
}

class Day13 : public aoc::AocObject<Day13>, public aoc::AocDay<Day13, int> {
   std::vector<Package> m_packages;

   public:
   static constexpr char FILENAME[] = "13";
   static constexpr char YEAR[] = "2022";
   static constexpr char TITLE[] = "Distress Signal";

   static auto get_object(std::istream& stream, Day13& day) -> bool {

      while( Package::get_object(stream, day.m_packages.emplace_back())) { }

      day.m_packages.pop_back();

      //if (day.m_packages.size() % 2 != 0) return false;
      return true;
   }

   auto part1() const -> int override {

      int sum = 0;

      for (int i = 0; i < m_packages.size()/2; ++i) {
         auto const& a = m_packages[2*i];
         auto const& b = m_packages[2*i+1];


         if (a<b){
            sum += i + 1;
         }
      }

      return sum;
   }

   auto part2() const -> int override {
      auto packages = m_packages;

      auto p1 = *Package::from_string("[[2]]");
      auto p2= *Package::from_string("[[6]]");
      packages.push_back(p1);
      packages.push_back(p2);

      std::sort(packages.begin(), packages.end(), std::less<>());

      int decoder = 1;
      int index = 1;
      for (auto const& p: packages) {
         if (p == p1 || p == p2) decoder *= index;
         ++index;
      }

      return decoder;
   }
};
#endif
