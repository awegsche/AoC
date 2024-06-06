#ifndef DAY2_H
#define DAY2_H

#include "common.h"
#include "log.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#define OPENFILE 0
#define LOADFILE 1
#define CALCTOTAL 2
#define CALCRIBBON 3
#define ENDERROR 16
#define AFTER_END 32

typedef struct {
    int sides[3];
} Wrapping;

Wrapping wrapping_from_string(const char *line) {
    Wrapping w;

    sscanf(line, "%dx%dx%d", &w.sides[0], &w.sides[1], &w.sides[2]);

    // sort sides
    if (w.sides[0] < w.sides[1]) {
        int t      = w.sides[0];
        w.sides[0] = w.sides[1];
        w.sides[1] = t;
    }

    if (w.sides[1] < w.sides[2]) {
        int t      = w.sides[1];
        w.sides[1] = w.sides[2];
        w.sides[2] = t;
    }
    if (w.sides[0] < w.sides[1]) {
        int t      = w.sides[0];
        w.sides[0] = w.sides[1];
        w.sides[1] = t;
    }

    return w;
}

int calc_total_paper(Wrapping *wrappings, size_t count) {
    int total = 0;
    for (size_t i = 0; i < count; ++i) {
        Wrapping *w = &wrappings[i];

        total += 2 * (w->sides[0] * w->sides[1] + w->sides[0] * w->sides[2]) +
                 3 * (w->sides[2] * w->sides[1]);
    }
    return total;
}

int calc_total_ribbon(Wrapping *wrappings, size_t count) {
    int total = 0;
    for (size_t i = 0; i < count; ++i) {
        const Wrapping *w = &wrappings[i];

        total += 2 * (w->sides[1] + w->sides[2]) +
                 w->sides[0] * w->sides[1] * w->sides[2];
    }
    return total;
}

void do_day2(LogManager *man) {

    int step    = 0;
    int frame   = 0;
    int section = 0;

    size_t linelen       = 0;
    char *line           = (char *)malloc(1024);
    FILE *inputfile      = 0;
    char *loaded_message = 0;
    char *last_w_msg     = 0;

    Wrapping wrappings[1024];
    size_t wrapping_count = 0;

    while (!WindowShouldClose()) {

        ++frame;

        if (frame > 30) {
            frame = 0;
            ++step;
        }

        BeginDrawing();
        ClearBackground(BACKGROUND);

        switch (section) {
        case OPENFILE:
            log_manager_appendf(man, "loading file %d", 42);
            inputfile = fopen("2015/input/day02.txt", "r");
            if (!inputfile) {
                log_manager_appendf(man, "couldn't open input file");
                section = ENDERROR;
            } else {
                loaded_message = log_manager_appendf(
                    man, "loading wrappings. %zu wrappings loaded",
                    wrapping_count);
                last_w_msg = log_manager_appendf(
                    man, "last wrapping: ----------------- ");
                ++section;
            }
            break;
        case LOADFILE:
            for (int jj = 0; jj < 10; ++jj) {
                if (fgets(line, 1024, inputfile)) {
                    wrappings[wrapping_count] = wrapping_from_string(line);
                    ++wrapping_count;

                    sprintf(loaded_message,
                            "loading wrappings. %zu wrappings loaded",
                            wrapping_count);

                    if (wrapping_count == 1024) {
                        log_manager_appendf(man,
                                            "end of wrapping buffer reached, "
                                            "increase buffer size");
                        section = ENDERROR;
                    }
                } else {
                    ++section;
                    break;
                }
            }
            Wrapping *w = &wrappings[wrapping_count - 1];
            sprintf(last_w_msg, "last wrapping: %d x %d x %d", w->sides[0], w->sides[1],
                    w->sides[2]);
            break;
        case CALCTOTAL:
            int total = calc_total_paper(wrappings, wrapping_count);
            log_manager_appendf(man, "total paper: %d", total);
            section = CALCRIBBON;
            break;
        case CALCRIBBON:
            int ribbon = calc_total_ribbon(wrappings, wrapping_count);
            log_manager_appendf(man, "total ribbon: %d", ribbon);
            section = AFTER_END;
            break;
        case ENDERROR:
            log_manager_appendf(man, "an error has occured");
            section = AFTER_END;
            break;
        case AFTER_END:
            break;
        }

        draw_messages(man);

        EndDrawing();
    }
    fclose(inputfile);
    free(line);
}

#endif
