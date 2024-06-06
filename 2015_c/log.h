#ifndef LOG_H
#define LOG_H

#include <raylib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_MANAGER_CAPACITY 1024
#define LOG_MANAGER_MESS_LENGTH 1024

static Font log_font     = {0};
static int log_font_size = 24;

typedef struct {
    char **messages;
    size_t len;
    size_t cap;
} LogManager;

LogManager log_manager_init() {
    LogManager man;
    man.messages = (char **)malloc(LOG_MANAGER_CAPACITY * sizeof(char *));
    man.len      = 0;
    man.cap      = LOG_MANAGER_CAPACITY;

    log_font = GetFontDefault();

    return man;
}

void log_manager_set_font(Font font) { log_font = font; }

void log_manager_append_owned(LogManager *man, char *message) {
    if (man->len == man->cap) {
        man->cap        = man->cap * 2;
        char **new_data = (char **)malloc(man->cap * sizeof(char *));
        memcpy(new_data, man->messages, man->len * sizeof(char *));
        free(man->messages);
        man->messages = new_data;
    }
    man->messages[man->len] = message;
    ++man->len;
}

char *log_manager_appendf(LogManager *man, const char *fmt, ...) {
    static char buf[10 * LOG_MANAGER_MESS_LENGTH];

    va_list arglist;
    va_start(arglist, fmt);
    int len = vsprintf(buf, fmt, arglist);
    va_end(arglist);

    char *message = (char *)malloc(
        len + 128); // leave a bit of extra space for modifications
    memcpy(message, buf, len);
    message[len] = 0;
    log_manager_append_owned(man, message);
    return message;
}

/**
  append `message` to the log.

  returns a handle to the newly appended message, for later modification
  */
char *log_manager_append(LogManager *man, const char *message) {
    char *mess_copy = (char *)malloc(strlen(message) + 1);
    memcpy(mess_copy, message, strlen(message) + 1);

    log_manager_append_owned(man, mess_copy);

    return mess_copy;
}

void log_manager_cleanup(LogManager *man) {
    for (size_t i = 0; i < man->len; ++i) {
        free(man->messages[i]);
    }
    free(man->messages);
}

void draw_messages(const LogManager *man) {
    int max_lines  = 480 / 20;
    int first_line = man->len > max_lines ? man->len - max_lines : 0;

    for (int i = first_line; i < man->len; ++i) {
        DrawTextEx(log_font, man->messages[i],
                   (Vector2){20, 480 + log_font_size * (i - first_line)},
                   log_font_size, 0, GRAY);
    }
}

#endif
