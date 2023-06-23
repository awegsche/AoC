//
// Created by andiw on 07/12/2022.
//

#ifndef AOC_TIMING_H
#define AOC_TIMING_H

#include <chrono>
#include <iostream>
#include <iomanip>
#include <ratio>

#include "pretty.h"

using std::cout, std::cerr, std::endl, std::setw, std::setfill, std::left, std::right;

using std::chrono::steady_clock, std::chrono::duration_cast, std::chrono::nanoseconds;
using timepoint = std::chrono::time_point<steady_clock, std::chrono::nanoseconds>;

namespace aoc {
    constexpr char UNIT_COLOR[] = "\33[1m\33[38;2;128;128;128m";

    class Timer {
        timepoint m_last;
        timepoint m_stop;

    public:
        Timer()
        : m_last(steady_clock::now())
        , m_stop(steady_clock::now())
        {}

        void restart() {
            m_last = steady_clock::now();
        }

        /// elapsed time in nanoseconds
        /// \return
        auto elapsed() const -> long long {
            return duration_cast<nanoseconds>(steady_clock::now() - m_last).count();
        }

        auto stop() {
            m_stop = steady_clock::now();
        }

        auto get_lap() const -> long long {
            return duration_cast<nanoseconds>(m_stop - m_last).count();
        }

    };

    void print_time(long long nanoseconds);

} // namespace aoc

#endif //AOC_TIMING_H
