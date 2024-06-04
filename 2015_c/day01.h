#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

typedef struct {
    const char *line;
    char *pointer;
    char *value_line;
    int step;
    int floor;
    bool finished;
} Day1;

void day1_setup_data(Day1 *data, const char *line) {
    printf("setup, line = %s\n", line);
    data->line       = line;
    data->pointer    = (char *)malloc((strlen(line) + 1) * sizeof(char));
    data->value_line = (char *)malloc(64 * sizeof(char));
    data->step       = 0;
    data->floor      = 0;
    data->finished   = false;
    // printf("sizeof(char) = %llu", sizeof(char));
}

void day1_cleanup(Day1 *data) {
    printf("cleanup ... ");
    free(data->pointer);
    free(data->value_line);
    printf("cleanup done");
}

void advance_step(Day1 *data) {
    switch (data->line[data->step]) {
    case '(':
        ++data->floor;
        break;
    case ')':
        --data->floor;
        break;
    }
    for (int i = 0; i < data->step; ++i) {
        data->pointer[i] = ' ';
    }
    data->pointer[data->step]     = '^';
    data->pointer[data->step + 1] = 0;
    sprintf(data->value_line, "%03d", data->floor);
    ++data->step;
    data->finished = data->step >= strlen(data->line);
}

void solve_step(Day1 *data) {

    DrawText(data->line, 20, 45, 20, LIGHTGRAY);
    DrawText(data->pointer, 20, 70, 20, RED);
    DrawText(data->value_line, 20, 95, 20, GREEN);
}

void do_day01(int steptime, LogManager* man) {
    int ticker = 0;

    Day1 data;
    day1_setup_data(&data, "(()())");

    while (!WindowShouldClose()) {
        ++ticker;

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawText("Solving Advent of Code 2015 in C", 20, 20, 20, LIGHTGRAY);
        solve_step(&data);

        if (!data.finished && ticker == steptime) {
            ticker = 0;
            advance_step(&data);

            log_manager_append(man, "next");
        }

        draw_messages(man);
        EndDrawing();
    }


    day1_cleanup(&data);
}
