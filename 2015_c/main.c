#include <stdio.h>
#include <raylib.h>

#include "day01.h"
#include "day02.h"
#include "log.h"

const int STEPTIME = 1;


int main(int argc, char** argv) {
    printf("AoC 2015 -- in C\n");


    // check if we are in the right dir
    FILE* file = fopen("2015/input/day01.txt", "r");
    if (!file) {
        fprintf(stderr, "Couldn't load input file, are we in the right directory?\n");
        return 1;
    }
    fclose(file);

    const int width = 1280;
    const int height = 960;

    LogManager logs = log_manager_init();

    InitWindow(width, height, "AoC 2015 -- in C");

    SetTargetFPS(60);

    do_day2(&logs);

    CloseWindow();

    log_manager_cleanup(&logs);
    return 0;
}
