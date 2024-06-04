#include "log.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---- status codes
// -------------------------------------------------------------------------------
#define SUCCESS 0
#define FILENOTFOUND 1
#define BAD_ALLOC 2

#define JUMP 10

// ---- Day data struct and handling
// ---------------------------------------------------------------
typedef struct {
    char *line;
    char *pointer;
    char *value_line;
    int step;
    int floor;
    bool finished;
    bool underground_reached;
    int offset;
} Day1;

void day1_setup_data(Day1 *data, char *line) {
    printf("setup, line = %s\n", line);
    data->line          = line;
    data->pointer       = (char *)malloc((strlen(line) + 1) * sizeof(char));
    data->pointer[0]    = 0;
    data->value_line    = (char *)malloc(64 * sizeof(char));
    data->value_line[0] = 0;
    data->step          = 0;
    data->floor         = 0;
    data->offset        = 0;
    data->finished      = false;
    data->underground_reached = false;
    // printf("sizeof(char) = %llu", sizeof(char));
}

int day1_load_from_file(Day1 *data, const char *filename) {
    FILE *file = fopen(filename, "rb");

    if (!file)
        return FILENOTFOUND;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *line = malloc(length + 1);

    if (!line)
        return BAD_ALLOC;

    printf("length: %ld\n", length);
    fread(line, 1, length, file);
    line[length] = 0;
    printf("line: %s\n", line);

    day1_setup_data(data, line);

    fclose(file);

    return SUCCESS;
}

void day1_from_const(Day1 *data, const char *line) {
    char *newline = malloc(strlen(line));
    day1_setup_data(data, newline);
}

void day1_cleanup(Day1 *data) {
    printf("cleanup ...");
    free(data->line);
    printf(" line ");
    free(data->pointer);
    printf(" pointer ");
    free(data->value_line);
    printf(" value_line ");
    printf("... cleanup done\n");
}

// ---- Solving day 1
// ------------------------------------------------------------------------------
void advance_step(Day1 *data) {
    if (data->finished) return;
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
    sprintf(data->value_line, "step: %04d; floor: %4d", data->step, data->floor);
    ++data->step;
    data->finished = data->step >= strlen(data->line);
    if (data->step > 20) {
        data->offset = data->step - 20;
    }
}

void solve_step(Day1 *data) {

    DrawText(data->line + data->offset, 20, 45, 20, LIGHTGRAY);
    DrawText(data->pointer + data->offset, 20, 70, 20, RED);
    DrawText(data->value_line, 20, 95, 20, GREEN);
}

void do_day01(int steptime, LogManager *man) {
    int ticker = 0;

    Day1 data;
    char *logmessage = (char *)malloc(512);

    sprintf(logmessage, "load input file");
    log_manager_append(man, logmessage);

    WindowShouldClose();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    draw_messages(man);
    EndDrawing();

    day1_load_from_file(&data, "2015/input/day01.txt");

    sprintf(logmessage, "loading finished");
    log_manager_append(man, logmessage);

    WindowShouldClose();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    draw_messages(man);
    EndDrawing();

    while (!WindowShouldClose()) {
        ++ticker;

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawText("Solving Advent of Code 2015 in C", 20, 20, 20, LIGHTGRAY);
        solve_step(&data);

        if (!data.finished && ticker == steptime) {
            ticker = 0;
            for(int ii = 0; ii < JUMP; ++ii) {
            advance_step(&data);

            if (data.finished) {
                sprintf(logmessage, "finished, floor = %d", data.floor);
                log_manager_append(man, logmessage);
            }

            if (!data.underground_reached && data.floor < 0) {

                sprintf(logmessage, "reached floor -1 at %d", data.step);
                log_manager_append(man, logmessage);
                data.underground_reached = true;
            }
            }
        }

        draw_messages(man);
        EndDrawing();
    }

    free(logmessage);
    day1_cleanup(&data);
}
