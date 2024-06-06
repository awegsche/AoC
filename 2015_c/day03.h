#ifndef DAY3_H
#define DAY3_H

#include "common.h"
#include "log.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPENFILE 0
#define LOADFILE 1
#define PART1_MOVE 2
#define PART1_CALC 3
#define PART2_MOVE 4
#define PART2_CALC 5
#define ENDERROR 32
#define AFTEREND 64

void do_day3(LogManager *man) {
    char *line        = NULL;
    size_t line_len   = 0;
    int section       = OPENFILE;
    FILE *inputfile   = NULL;
    char *walking_msg = NULL;

    int WIDTH  = 1024;
    int HEIGHT = 1024;

    int wh    = WIDTH * HEIGHT * sizeof(int);
    int pos_x = 512;
    int pos_y = 512;
    int pos_x_robo = 512;
    int pos_y_robo = 512;
    int idx   = 0;

    int *map = malloc(wh);
    memset(map, 0, wh);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BACKGROUND);

        switch (section) {
        case OPENFILE:
            log_manager_appendf(man, "loading file");
            inputfile = fopen("2015/input/day03.txt", "r");
            if (!inputfile) {
                log_manager_appendf(man, "couldn't open input file");
                section = ENDERROR;
                break;
            }
            fseek(inputfile, 0, SEEK_END);
            line_len = ftell(inputfile);
            fseek(inputfile, 0, SEEK_SET);

            line = malloc(line_len);
            fread(line, 1, line_len, inputfile);

            log_manager_appendf(man, "path loaded");
            walking_msg =
                log_manager_appendf(man, "walking ------------------");
            section = PART1_MOVE;
            break;
        case PART1_MOVE: {
            char dir = ' ';
            for (int jj = 0; jj < 20; ++jj) {
                dir = line[idx];
                switch (dir) {
                case '>':
                    ++pos_x;
                    break;
                case '<':
                    --pos_x;
                    break;
                case 'v':
                    --pos_y;
                    break;
                case '^':
                    ++pos_y;
                    break;
                }
                ++idx;

                if (idx == line_len) {
                    section = PART1_CALC;
                    log_manager_appendf(man, "finished walking path");
                    break;
                }
                if (pos_x < 0 || pos_x >= WIDTH || pos_y < 0 ||
                    pos_y >= HEIGHT) {
                    log_manager_appendf(man, "out of bounds of map");
                    section = ENDERROR;
                    break;
                }
                ++map[WIDTH * pos_y + pos_x];
            }

            float perc = (float)idx / (float)line_len * 100.0f;

            sprintf(walking_msg,
                    "walking: move: %c; %.2f percent; pos: (%d, %d)", dir, perc,
                    pos_x, pos_y);

        } break;
        case PART1_CALC: {
            int houses = 1; // init with starting point
            for (int y = 0; y < HEIGHT; ++y) {
                for (int x = 0; x < WIDTH; ++x) {
                    if (map[WIDTH * y + x])
                        ++houses;
                }
            }
            log_manager_appendf(man, "houses: %d", houses);
            memset(map, 0, sizeof(int) * wh);
            section = PART2_MOVE;
        } break;
        case PART2_MOVE: {
            char dir = ' ';
            for (int jj = 0; jj < 20; ++jj) {
                dir = line[idx];
                switch (dir) {
                case '>':
                    ++pos_x;
                    break;
                case '<':
                    --pos_x;
                    break;
                case 'v':
                    --pos_y;
                    break;
                case '^':
                    ++pos_y;
                    break;
                }
                ++idx;

                if (idx == line_len) {
                    section = PART1_CALC;
                    log_manager_appendf(man, "finished walking path");
                    break;
                }
                if (pos_x < 0 || pos_x >= WIDTH || pos_y < 0 ||
                    pos_y >= HEIGHT) {
                    log_manager_appendf(man, "out of bounds of map");
                    section = ENDERROR;
                    break;
                }
                ++map[WIDTH * pos_y + pos_x];
            }

            float perc = (float)idx / (float)line_len * 100.0f;

            sprintf(walking_msg,
                    "walking: move: %c; %.2f percent; pos: (%d, %d)", dir, perc,
                    pos_x, pos_y);
        } break;
        case PART2_CALC:
            break;
        case ENDERROR:
            section = AFTEREND;
            break;
        case AFTEREND:
            break;
        }
        draw_messages(man);

        EndDrawing();
    }

    if (inputfile) {
        fclose(inputfile);
    }

    if (line) {
        free(line);
    }
    free(map);
}

#endif
