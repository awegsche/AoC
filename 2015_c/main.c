#include <stdio.h>
#include <raylib.h>

#include "day01.h"
#include "log.h"

const int STEPTIME = 30;


int main(int argc, char** argv) {
    printf("hello %s %d", "42", 69);

    const int width = 1280;
    const int height = 960;

    LogManager logs = log_manager_init();

    InitWindow(width, height, "AoC 2015 -- in C");

    SetTargetFPS(60);

    do_day01(STEPTIME, &logs);

    CloseWindow();

    log_manager_cleanup(&logs);
    return 0;
}
