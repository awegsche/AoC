//
// Created by andiw on 07/12/2022.
//

#include "timing.h"

void aoc::print_time(long long int nanoseconds) {

    cout << setw(COL3 - 4) << std::setprecision(3) << right;

    if (nanoseconds < 1000000) {
        cout << 1.0e-3f * nanoseconds << UNIT_COLOR << " us ";
    } else if (nanoseconds < 1000000000) {
        cout << 1.0e-6f * nanoseconds << UNIT_COLOR << " ms ";
    } else {
        cout << 1.0e-9f * nanoseconds << UNIT_COLOR << "  s ";
    }
    cout << RESET;
}
