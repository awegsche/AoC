#ifndef DAY5_H
#define DAY5_H

#include "log.h"
#define DO_TEST1 1
#define DO_TEST2 2
#define DO_TEST3 3
#define DO_TEST4 4
#define DO_TEST5 5 // first part2 test
#define DO_TEST6 6
#define DO_TEST7 7
#define DO_TEST8 8
#define DO_TEST9 9
#define END_TESTS 10
#define PART1 11
#define PART2 12
#define CALC_PART1 13
#define CALC_PART2 14
#define END_PART1 15
#define END_PART2 16
#define END_ERROR 32
#define END_SUCCESS 64
#define AT_END 65

#include "raylib.h"
#include <stdbool.h>

bool is_nice(const char *message) {
    int vowels      = 0;
    bool has_double = false;
    char last       = ' ';

    const char *c = message;
    while (*c != 0) {
        switch (*c) {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            ++vowels;
            break;
        case 'b':
            if (*(c - 1) == 'a')
                return false;
            break;
        case 'd':
            if (*(c - 1) == 'c')
                return false;
            break;
        case 'q':
            if (*(c - 1) == 'p')
                return false;
            break;
        case 'y':
            if (*(c - 1) == 'x')
                return false;
            break;
        default:
            break;
        }
        if (*c == last) {
            has_double = true;
        }
        last = *c;
        ++c;
    }
    return (vowels > 2) && has_double;
}

bool is_nice2(const char *message) {

    int mask[26];
    memset(&mask, 0, 26 * sizeof(int));

    int mask_old[26];
    memset(&mask_old, 0, 26 * sizeof(int));

    const char *c  = message;
    bool has_pairs = false;
    bool has_xyx   = false;

    while (*(c + 1) != 0) {
        char _c    = *c - 'a';
        char _next = *(c + 1) - 'a';

        if ((mask_old[_c] >> _next) & 1) {
            has_pairs = true;
            break;
        }
        memcpy(mask_old, mask, 26 * sizeof(int));
        mask[_c] |= (1 << _next);
        ++c;
    }

    c = message;
    while (*(c + 2) != 0) {
        if (*c == *(c + 2)) {
            has_xyx = true;
            break;
        }
        ++c;
    }
    return has_pairs && has_xyx;
}

bool do_test(LogManager *man, const char *message, bool nice) {
    char *test_log = log_manager_appendf(
        man, "running test with nice string %s      ", message);

    if (is_nice(message) == nice) {
        sprintf(test_log, "running test with string %s OK", message);
        return true;
    } else {
        sprintf(test_log, "running test with string %s FAILED", message);
        return false;
    }
}

bool do_test2(LogManager *man, const char *message, bool nice) {
    char *test_log = log_manager_appendf(
        man, "running test of part 2 with nice string %s      ", message);

    if (is_nice2(message) == nice) {
        sprintf(test_log, "running test with string %s OK", message);
        return true;
    } else {
        sprintf(test_log, "running test with string %s FAILED", message);
        return false;
    }
}

void do_day5(LogManager *man) {
    int section = DO_TEST1;

    char *messages[1024];
    size_t message_count   = 0;
    size_t current_message = 0;

    char *part1_log       = 0;
    char *part1_log_line2 = 0;
    int part1_count       = 0;

    char *part2_log       = 0;
    char *part2_log_line2 = 0;
    int part2_count       = 0;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch (section) {
        case DO_TEST1:
            section = do_test(man, "aaa\0", true) ? DO_TEST2 : END_ERROR;
            break;
        case DO_TEST2:
            section =
                do_test(man, "ugknbfddgicrmopn\0", true) ? DO_TEST3 : END_ERROR;
            break;
        case DO_TEST3:
            section = do_test(man, "jchzalrnumimnmhp\0", false) ? DO_TEST4
                                                                : END_ERROR;
            break;
        case DO_TEST4:
            section = do_test(man, "haegwjzuvuyypxyu\0", false) ? DO_TEST5
                                                                : END_ERROR;
            log_manager_appendf(
                man, "finished part1 tests, continue with part2 ones");
            break;
        case DO_TEST5:
            section = do_test2(man, "qjhvhtzxzqqjkmpb\0", true) ? DO_TEST6
                                                                : END_ERROR;
            break;
        case DO_TEST6:
            section = do_test2(man, "ieodomkazucvgmuy\0", false) ? DO_TEST7
                                                                 : END_ERROR;
            break;
        case DO_TEST7:
            section = do_test2(man, "xxyxx\0", true) ? DO_TEST8 : END_ERROR;
            break;
        case DO_TEST8:
            section = do_test2(man, "uurcxstgmygtbstg\0", false) ? DO_TEST9
                                                                 : END_ERROR;
            break;
        case DO_TEST9:
            section = do_test2(man, "aaa\0", false) ? END_TESTS : END_ERROR;
            break;
        case END_TESTS:
            log_manager_appendf(
                man, "--- all tests passed successfully, solve puzzle");
            section = PART1;
            break;
        case PART1:
            FILE *inputfile = fopen("2015/input/day05.txt", "r");
            if (!inputfile) {
                log_manager_appendf(man, "couldn't open inputfile");
                section = END_ERROR;
                break;
            }

            char line[64];
            while (fgets(line, 64, inputfile)) {
                // replace the newline character
                size_t ii = 0;
                for (; ii < 64; ++ii) {
                    if (line[ii] == '\n') {
                        line[ii] = 0;
                        break;
                    }
                }
                char *mess_copy = malloc(ii + 1);
                memcpy(mess_copy, line, ii + 1);

                messages[message_count] = mess_copy;
                ++message_count;

                if (message_count > 1024) {
                    log_manager_appendf(
                        man,
                        "exceeded message buffer size, make a bigger one!");
                    section = END_ERROR;
                    break;
                }
            }
            log_manager_appendf(man, "loaded %d messages to check",
                                message_count);
            fclose(inputfile);

            log_manager_appendf(man, "checking messages -- part1");
            part1_log = log_manager_appendf(
                man, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx : [OK / FAILED]");
            part1_log_line2 = log_manager_appendf(man, "nice messages: 0");

            section = CALC_PART1;
            break;
        case CALC_PART1:

            if (is_nice(messages[current_message])) {
                ++part1_count;
                sprintf(part1_log, "%7.2f | %32s: [  OK  ]",
                        (float)current_message / (float)message_count * 100.0f,
                        messages[current_message]);
                sprintf(part1_log_line2, "nice messages: %d", part1_count);
            } else {
                sprintf(part1_log, "%7.2f | %32s: [FAILED]",
                        (float)current_message / (float)message_count * 100.0f,
                        messages[current_message]);
            }
            ++current_message;

            if (current_message == message_count) {
                section = END_PART1;

                sprintf(part1_log, "100.00 | %32s: [  OK  ]",
                        messages[current_message]);
                break;
            }

            break;
        case PART2:
            log_manager_appendf(man, "checking messages -- part2");
            part2_log = log_manager_appendf(
                man, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx : [OK / FAILED]");
            part2_log_line2 = log_manager_appendf(man, "nice messages: 0");

            section = CALC_PART2;

            break;
        case CALC_PART2:
            if (is_nice2(messages[current_message])) {
                ++part2_count;
                sprintf(part2_log, "%7.2f | %32s: [  OK  ]",
                        (float)current_message / (float)message_count * 100.0f,
                        messages[current_message]);
                sprintf(part2_log_line2, "nice messages: %d", part2_count);
            } else {
                sprintf(part2_log, "%7.2f | %32s: [FAILED]",
                        (float)current_message / (float)message_count * 100.0f,
                        messages[current_message]);
            }
            ++current_message;

            if (current_message == message_count) {
                section = END_PART2;

                sprintf(part2_log, "100.00 | %32s: [  OK  ]",
                        messages[current_message]);
                break;
            }

            break;
            section = END_SUCCESS;
            break;
        case END_PART1:
            log_manager_appendf(man, "=== part1 nice messages: %d",
                                part1_count);
            current_message = 0;
            section         = PART2;
            break;
        case END_PART2:
            log_manager_appendf(man, "=== part2 nice messages: %d",
                                part2_count);
            current_message = 0;
            section         = END_SUCCESS;
            break;
        case END_ERROR:
            log_manager_appendf(man, "errors occured, program couldn't finish");
            section = AT_END;
            // actually omitting break here does the right thing (repeateing the
            // `--- END ---` message)
        case END_SUCCESS:
            log_manager_appendf(man, "--- END ---");

            // delete all the messages from memory
            for (size_t m = 0; m < message_count; ++m) {
                free(messages[m]);
            }
            section = AT_END;
            break;
        case AT_END:
            break;
        }
        draw_messages(man);
        EndDrawing();
    }
}

#endif
