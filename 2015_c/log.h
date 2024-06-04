#ifndef LOG_H
#define LOG_H

#include <raylib.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define LOG_MANAGER_CAPACITY 1024

typedef struct {
    char **messages;
    size_t len;
    size_t cap;
} LogManager;

LogManager log_manager_init() {
    LogManager man;
    man.messages = (char **)malloc(LOG_MANAGER_CAPACITY*sizeof(char*));
    man.len      = 0;
    man.cap      = LOG_MANAGER_CAPACITY;

    return man;
}

void log_manager_append(LogManager *man, const char *message) {
    if (man->len == man->cap) {
        man->cap = man->cap * 2;
        char **new_data = (char **)malloc(man->cap*sizeof(char*));
        memcpy(new_data, man->messages, man->len*sizeof(char*));
        free(man->messages);
        man->messages = new_data;
    }
    char* mess_copy = (char*)malloc(strlen(message) + 1);
    memcpy(mess_copy, message, strlen(message) + 1);
    man->messages[man->len] = mess_copy;
    ++man->len;
}

void log_manager_cleanup(LogManager *man) {
    for (size_t i = 0; i < man->len; ++i) {
        free((void *)man->messages[i]);
    }
    free(man->messages);
}

void draw_messages(const LogManager *man) {
    int max_lines     = 480 / 20;
    int first_line = man->len > max_lines ? man->len - max_lines : 0;

    for (int i = first_line; i < man->len; ++i) {
        DrawText(man->messages[i], 20, 480 + 20 * (i-first_line), 20, GRAY);
    }
}

#endif
