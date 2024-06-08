#ifndef DAY6_H
#define DAY6_H

#include "common.h"
#include "log.h"
#include "raylib.h"
#include <stdbool.h>

typedef enum {
    DAY6_INIT,
    DAY6_TEST_1,
    DAY6_TEST_2,
    DAY6_END_TESTS,
    DAY6_LOAD_CMDS,
    DAY6_OPEN_FILE,
    DAY6_END_LOADING,
    DAY6_SOLVE1,
    DAY6_SOLVE2,
    DAY6_END_ERROR,
    DAY6_END_SUCCESS,
    DAY6_ATEND
} Day6_Section;

#define LIGHTSTEP 12

#define COMMAND_BUFFER_SIZE 1024
#define LINE_BUFFER_SIZE 1024

// ---- structs ------------------------------------------------------------------------------------

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Color3;

typedef struct {
    int x;
    int y;
} Vector2i;

#define TURN_ON 1
#define TURN_OFF -1
#define TOGGLE 0

typedef struct {
    char mode;
    Vector2i p0;
    Vector2i p1;
} Command;

// ---- part1 commands -----------------------------------------------------------------------------

void toggle(Color3 *data, Vector2i p0, Vector2i p1) {
    for (int y = p0.y; y <= p1.y; ++y) {
        for (int x = p0.x; x <= p1.x; ++x) {
            if (data[y * 1000 + x].r == 255) {
                data[y * 1000 + x] = (Color3){0, 0, 0};
            } else {
                data[y * 1000 + x] = (Color3){255, 255, 255};
            }
        }
    }
}

void turn_on(Color3 *data, Vector2i p0, Vector2i p1) {
    for (int y = p0.y; y <= p1.y; ++y) {
        for (int x = p0.x; x <= p1.x; ++x) {
            data[y * 1000 + x] = (Color3){255, 255, 255};
        }
    }
}

void turn_off(Color3 *data, Vector2i p0, Vector2i p1) {
    for (int y = p0.y; y <= p1.y; ++y) {
        for (int x = p0.x; x <= p1.x; ++x) {
            data[y * 1000 + x] = (Color3){0, 0, 0};
        }
    }
}

void exec_command(Command cmd, Color3 *pixels) {
    switch (cmd.mode) {
    case TURN_ON:
        turn_on(pixels, cmd.p0, cmd.p1);
        break;
    case TURN_OFF:
        turn_off(pixels, cmd.p0, cmd.p1);
        break;
    case TOGGLE:
        toggle(pixels, cmd.p0, cmd.p1);
        break;
    }
}

size_t count_lights(const Color3 *pixels) {
    size_t count = 0;
    for (size_t i = 0; i < 1000000; ++i) {
        if (pixels[i].r == 255)
            ++count;
    }
    return count;
}

// ---- part2 commands -----------------------------------------------------------------------------

size_t value(const Color3 *col) { return col->r + col->g + col->b; }

void increase(Color3 *col) {
    if (col->g == 255) {
        ++col->b;
    } else if (col->r == 255) {
        ++col->g;
    } else {
        ++col->r;
    }
}

void decrease(Color3 *col) {
    if (col->b > 0) {
        --col->b;
    } else if (col->g > 0) {
        --col->g;
    } else if (col->r > 0) {
        --col->r;
    }
}

void toggle2(Color3 *data, Vector2i p0, Vector2i p1) {
    for (int y = p0.y; y <= p1.y; ++y) {
        for (int x = p0.x; x <= p1.x; ++x) {
            Color3 *pixel = data + y * 1000 + x;
            for (int i = 0; i < LIGHTSTEP * 2; ++i) {
                increase(pixel);
            }
        }
    }
}

void turn_on2(Color3 *data, Vector2i p0, Vector2i p1) {
    for (int y = p0.y; y <= p1.y; ++y) {
        for (int x = p0.x; x <= p1.x; ++x) {
            Color3 *pixel = data + y * 1000 + x;
            for (int i = 0; i < LIGHTSTEP; ++i) {
                increase(pixel);
            }
        }
    }
}

void turn_off2(Color3 *data, Vector2i p0, Vector2i p1) {
    for (int y = p0.y; y <= p1.y; ++y) {
        for (int x = p0.x; x <= p1.x; ++x) {
            for (int i = 0; i < LIGHTSTEP; ++i) {
                decrease(data + y * 1000 + x);
            }
        }
    }
}

void exec_command2(Command cmd, Color3 *pixels) {
    switch (cmd.mode) {
    case TURN_ON:
        turn_on2(pixels, cmd.p0, cmd.p1);
        break;
    case TURN_OFF:
        turn_off2(pixels, cmd.p0, cmd.p1);
        break;
    case TOGGLE:
        toggle2(pixels, cmd.p0, cmd.p1);
        break;
    }
}

size_t count_lumi(const Color3 *pixels) {
    size_t count = 0;
    for (size_t i = 0; i < 1000000; ++i) {
        count += value(pixels + i) / LIGHTSTEP;
    }
    return count;
}

Command read_from_str(const char *str) {
    Command cmd = {0};
    if (sscanf_s(str, "turn on %d,%d through %d,%d", &cmd.p0.x, &cmd.p0.y, &cmd.p1.x, &cmd.p1.y))
        cmd.mode = TURN_ON;
    else if (sscanf_s(str, "turn off %d,%d through %d,%d", &cmd.p0.x, &cmd.p0.y, &cmd.p1.x,
                      &cmd.p1.y))
        cmd.mode = TURN_OFF;
    else if (sscanf_s(str, "toggle %d,%d through %d,%d", &cmd.p0.x, &cmd.p0.y, &cmd.p1.x, &cmd.p1.y))
        cmd.mode = TOGGLE;
    return cmd;
}

// ---- main ---------------------------------------------------------------------------------------

void do_day6(LogManager *man) {

    log_manager_appendf(man, "---- Day 06 ----");

    // setup lights texture?
    Image im = {.format  = PIXELFORMAT_UNCOMPRESSED_R8G8B8,
                .data    = malloc(24 * 1000 * 1000),
                .width   = 1000,
                .height  = 1000,
                .mipmaps = 1};

    unsigned char *data = (unsigned char *)im.data;
    for (int y = 0; y < 1000; ++y) {
        for (int x = 0; x < 1000; ++x) {
            size_t pos    = (y * 1000 + x) * 3;
            data[pos]     = (char)((float)y / 1000.0f * 128.0f);
            data[pos + 1] = (char)((float)x / 1000.0f * 128.0f);
            data[pos + 2] = 255;
        }
    }

    Texture texture = LoadTextureFromImage(im);

    int i = 0;

    int section = 0;

    // state
    FILE *inputfile = 0;
    Command cmds[COMMAND_BUFFER_SIZE];
    size_t command_count = 0;
    char line[LINE_BUFFER_SIZE];
    char *opened_cmds_log = 0;

    size_t current_cmd = 0;
    char *part1_log1   = 0;
    char *part1_log2   = 0;

    char *part2_log1 = 0;
    char *part2_log2 = 0;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BACKGROUND);

        switch (section) {
        case DAY6_INIT:
            UpdateTexture(texture, data);
            DrawTexture(texture, 0, 0, RAYWHITE);
            section = DAY6_TEST_1;
            break;
        case DAY6_OPEN_FILE:
            log_manager_appendf(man, "loading inputfile");
            int err = fopen_s(&inputfile, "2015/input/day06.txt", "r");

            if (err) {
                log_manager_appendf(man, "couldn't open inputfile, errno = %d", err);
                section = DAY6_END_ERROR;
                break;
            }
            log_manager_appendf(man, "opened inputfile");
            opened_cmds_log = log_manager_appendf(man, "loaded %4d commads", 0);
            section         = DAY6_LOAD_CMDS;
            break;
        case DAY6_LOAD_CMDS:
            if (!fgets(line, LINE_BUFFER_SIZE, inputfile)) {
                printf("%s\n", line);
                section = DAY6_END_LOADING;
                break;
            }
            cmds[command_count] = read_from_str(line);
            ++command_count;
            sprintf(opened_cmds_log, "loaded %llu commands", command_count);
            if (command_count == COMMAND_BUFFER_SIZE) {
                log_manager_appendf(man, "exceeded command buffer size");
                section = DAY6_END_ERROR;
            }
            break;
        case DAY6_END_LOADING:
            fclose(inputfile);
            log_manager_appendf(man, "loaded %d commands", command_count);
            log_manager_appendf(man, "--- Part 1 ---");
            section = DAY6_SOLVE1;

            memset(data, 0, 1000 * 1000 * sizeof(Color3));

            part1_log1 = log_manager_appendf(man, "executed %4d commands", 0);
            part1_log2 = log_manager_appendf(man, "lights on: %4d", 0);

            break;
        case DAY6_SOLVE1:

            exec_command(cmds[current_cmd], (Color3 *)data);
            ++current_cmd;
            UpdateTexture(texture, data);

            size_t lights = count_lights((const Color3 *)data);
            sprintf(part1_log1, "executed %4llu commands", current_cmd);
            sprintf(part1_log2, "lights on: %llu", lights);

            if (current_cmd == command_count) {
                current_cmd = 0;
                memset(data, 0, 1000 * 1000 * sizeof(Color3));
                log_manager_appendf(man, "--- Part 2 ---");
                part2_log1 = log_manager_appendf(man, "executed %4d commands", 0);
                part2_log2 = log_manager_appendf(man, "luminosity: %10d", 0);
                section    = DAY6_SOLVE2;
            }

            break;
        case DAY6_SOLVE2:
            exec_command2(cmds[current_cmd], (Color3 *)data);
            ++current_cmd;
            UpdateTexture(texture, data);

            lights = count_lumi((const Color3 *)data);
            sprintf(part2_log1, "executed %4llu commands", current_cmd);
            sprintf(part2_log2, "lights on: %llu", lights);

            if (current_cmd == command_count) {
                section = DAY6_END_SUCCESS;
            }
            break;

        case DAY6_TEST_1:
            // do the second example (the first one seems trivial)
            turn_on((Color3 *)data, (Vector2i){0, 0}, (Vector2i){999, 0});
            UpdateTexture(texture, data);
            DrawTexture(texture, 0, 0, RAYWHITE);

            // just count the number of lights
            if (count_lights((const Color3 *)data) == 1000) {
                log_manager_appendf(man, "test1 [  OK  ]");
                section = DAY6_TEST_2;
            } else {
                log_manager_appendf(man, "test1 [FAILED]");
                section = DAY6_END_ERROR;
            }

            break;
        case DAY6_TEST_2:
            // do the second example (the first one seems trivial)
            turn_on((Color3 *)data, (Vector2i){499, 499}, (Vector2i){500, 500});
            UpdateTexture(texture, data);

            // just count the number of lights
            if (count_lights((const Color3 *)data) == 1004) {
                log_manager_appendf(man, "test2 [  OK  ]");
                section = DAY6_END_TESTS;
            } else {
                log_manager_appendf(man, "test2 [FAILED]");
                section = DAY6_END_ERROR;
            }
            break;
        case DAY6_END_TESTS:
            log_manager_appendf(man, "all tests passed, continue solving puzzle");
            section = DAY6_OPEN_FILE;
            break;
        case DAY6_END_ERROR:
            log_manager_appendf(man, "errors occured quit");
            log_manager_appendf(man, "--- END ---");
            section = DAY6_ATEND;
            break;
        case DAY6_END_SUCCESS:
            log_manager_appendf(man, "success");
            log_manager_appendf(man, "--- END ---");
            section = DAY6_ATEND;
            break;
        case DAY6_ATEND:
            break;
        }

        DrawTexture(texture, 0, 0, RAYWHITE);
        draw_messages(man);
        EndDrawing();
    }

    CloseWindow();
    UnloadTexture(texture);
    free(data);
}

#endif
