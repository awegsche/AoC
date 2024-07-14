#ifndef DAY7_H
#define DAY7_H

#include "common.h"
#include "log.h"
#include <raylib.h>
#include <stdint.h>
#include <stdio.h>

// ---- sections -----------------------------------------------------------------------------------
typedef enum {
    DAY7_INIT,
    DAY7_TEST1,
    DAY7_LOAD_GATES,
    DAY7_PART1,
    DAY7_CALC1,
    DAY7_END_SUCCESS,
    DAY7_END_ERROR,
    DAY7_AT_END,
} Day7_Section;

// ---- structs and constants ----------------------------------------------------------------------

#define SCAN_BUFSIZE 512

typedef struct {
    uint16_t wires[26][27];
    char mask[26][27];
} Wires;

uint16_t *get_wire(Wires *tree, char name[2]) { return &tree->wires[name[0] - 'a'][name[1] - 'a']; }

void set_wire(Wires *tree, char name[2], uint16_t value) {
    *get_wire(tree, name) = value;
    tree->mask[name[0] - 'a'][name[1]-'a'] = 1;
}
void read_from_wire(Wires *tree, char name[2], uint16_t value) {
    tree->mask[name[0] - 'a'][name[1]-'a'] = 0;
}

typedef enum { GATE_NOOP, GATE_SET, GATE_AND, GATE_OR, GATE_LSHIFT, GATE_RSHIFT, GATE_NOT } GateType;

typedef struct {
    GateType type;
    int value;
    char op1[2];
    char op2[2];
    char target[2];
} Gate;

static LogManager *day7_log = 0;

// ---- gate related functions ---------------------------------------------------------------------

void exec_gate(Gate *gate, Wires *wires) {
    uint16_t op1 = *get_wire(wires, gate->op1);
    uint16_t op2 = *get_wire(wires, gate->op2);
    switch (gate->type) {
    case GATE_NOOP:
        break;
    case GATE_SET:
        if (gate->op1[0])
            *get_wire(wires, gate->target) = *get_wire(wires, gate->op1);
        else
            *get_wire(wires, gate->target) = gate->value;
        break;
    case GATE_AND:
        if (op1 || op2)
            *get_wire(wires, gate->target) = op1 & op2;
        break;
    case GATE_OR:
        if (op1 || op2)
            *get_wire(wires, gate->target) = op1 | op2;
        break;
    case GATE_LSHIFT:
        if (op1)
            *get_wire(wires, gate->target) = op1 << gate->value;
        break;
    case GATE_RSHIFT:
        if (op1)
            *get_wire(wires, gate->target) = op1 >> gate->value;
        break;
    case GATE_NOT:
        if (op1)
            *get_wire(wires, gate->target) = ~op1;
        break;
    }
}

// assigns wire name `str` to `op`, replacing an end of string `'\0'` by `'z' + 1` in order to
// make it fit into the 27th slot for wires
void assign_op(char *op, const char *str) {
    op[0] = str[0];
    op[1] = (str[1] == 0) ? 'z' + 1 : str[1];
}

Gate read_gate(const char *str) {
    static char buf[SCAN_BUFSIZE];
    Gate gate = {0};
    gate.type = GATE_NOOP;

    char op1[16];
    char op2[16];
    int iop2;
    char target[16];
    if (sscanf_s(str, "%s -> %s", op1, 16, target, 16) == 2) {
        if (op1[0] >= 'a' && op1[1] <= 'z')
            assign_op(gate.op1, op1);
        else {
            gate.value = atoi(op1);
        }
        gate.type = GATE_SET;
        assign_op(gate.target, target);

        if (day7_log) {
            log_manager_appendf(day7_log, "parsed SET gate: %d -> %s", gate.value, gate.target);
        }
        return gate;
    }

    if (sscanf_s(str, "%s AND %s -> %s", op1, 16, op2, 16, target, 16) == 3) {
        assign_op(gate.op1, op1);
        assign_op(gate.op2, op2);
        assign_op(gate.target, target);
        gate.type = GATE_AND;
        return gate;
    }

    if (sscanf_s(str, "%s OR %s -> %s", op1, 16, op2, 16, target, 16) == 3) {
        assign_op(gate.op1, op1);
        assign_op(gate.op2, op2);
        assign_op(gate.target, target);
        gate.type = GATE_OR;
        return gate;
    }

    // printf("trying to read LSHIFT: %s\n", str);
    if (sscanf_s(str, "%s LSHIFT %d -> %s", op1, 16, &iop2, target, 16) == 3) {
        assign_op(gate.op1, op1);
        gate.value = iop2;
        assign_op(gate.target, target);
        gate.type = GATE_LSHIFT;
        return gate;
    }

    // printf("trying to read RSHIFT: '%s'\n", str);
    if (sscanf_s(str, "%s RSHIFT %d -> %s", op1, 16, &iop2, target, 16) == 3) {
        assign_op(gate.op1, op1);
        gate.value = iop2;
        assign_op(gate.target, target);
        gate.type = GATE_RSHIFT;
        return gate;
    }

    if (sscanf_s(str, "NOT %s -> %s", op1, 16, target, 16) == 2) {
        printf("have a NOT\n");
        assign_op(gate.op1, op1);
        assign_op(gate.target, target);
        gate.type = GATE_NOT;
        return gate;
    }
    return gate;
}

// ---- main ---------------------------------------------------------------------------------------

#define test(wire, value)                                                                           \
    if (*get_wire(&test_wires, wire) == value) {                                                    \
        log_manager_appendf(man, "%6s [  OK  ]", wire);                                             \
    } else {                                                                                        \
        log_manager_appendf(man, "%6s [FAILED], is %d and should be %d", wire,                      \
                            *get_wire(&test_wires, wire), value);                                   \
        section = DAY7_END_ERROR;                                                                   \
        break;                                                                                      \
    }

void do_day7(LogManager *man) {

#define MAX_GATES 1024

    Wires wires = {0};
    Gate gates[MAX_GATES];
    size_t gate_count    = 0;
    Day7_Section section = DAY7_INIT;

    char *part1_log_line1 = 0;
    char *part1_log_line2 = 0;
    size_t current_gate   = 0;

    while (!WindowShouldClose()) {
        AOC_BEGIN_FRAME();

        switch (section) {
        case DAY7_INIT:
            log_manager_appendf(man, "==== Day 7 ====");
            day7_log = man;
            section  = DAY7_TEST1;
            break;
        case DAY7_TEST1:

            Wires test_wires = {0};
            Gate test_gates[8];

            test_gates[0] = read_gate("123 -> x");
            test_gates[1] = read_gate("456 -> y");
            test_gates[2] = read_gate("x AND y -> d");
            test_gates[3] = read_gate("x OR y -> e");
            test_gates[4] = read_gate("x LSHIFT 2 -> f");
            test_gates[5] = read_gate("y RSHIFT 2 -> g");
            test_gates[6] = read_gate("NOT x -> h");
            test_gates[7] = read_gate("NOT y -> i");
            test_gates[8] = read_gate("NOT a -> j");

            for (int i = 0; i < 9; ++i) {
                exec_gate(&test_gates[i], &test_wires);
            }

            char test_wire_address[2];
            assign_op(test_wire_address, "x");
            test(test_wire_address, 123);
            assign_op(test_wire_address, "y");
            test(test_wire_address, 456);
            assign_op(test_wire_address, "d");
            test(test_wire_address, 72);
            assign_op(test_wire_address, "e");
            test(test_wire_address, 507);
            assign_op(test_wire_address, "f");
            test(test_wire_address, 492);
            assign_op(test_wire_address, "g");
            test(test_wire_address, 114);
            assign_op(test_wire_address, "h");
            test(test_wire_address, 65412);
            assign_op(test_wire_address, "i");
            test(test_wire_address, 65079);
            // assign_op(test_wire_address, "j");
            // test(test_wire_address, 65079);

            log_manager_appendf(man, "finished tests, continue solving puzzle");
            section = DAY7_LOAD_GATES;
            break;
        case DAY7_END_SUCCESS:
            log_manager_appendf(man, "--- success ---");
            section = DAY7_AT_END;
            break;
        case DAY7_END_ERROR:
            log_manager_appendf(man, "errors occured, quit");
            section = DAY7_AT_END;
            break;
        case DAY7_AT_END:
            break;
        case DAY7_LOAD_GATES:
            FILE *inputfile = 0;
            int err         = fopen_s(&inputfile, "2015/input/day07.txt", "r");
            if (err) {
                log_manager_appendf(man, "couldn't open inputfile, errno = %d", err);
                section = DAY7_END_ERROR;
                break;
            }
            char buf[1024];

            while (fgets(buf, 1024, inputfile)) {
                gates[gate_count] = read_gate(buf);
                printf("gate %llu: %d\n", gate_count, gates[gate_count].type);
                ++gate_count;

                if (gate_count == MAX_GATES) {
                    log_manager_appendf(man, "MAX_GATES exceeded");
                    section = DAY7_END_ERROR;
                    break;
                }
            }
            section = DAY7_PART1;
            fclose(inputfile);
            break;
        case DAY7_PART1: {
            log_manager_appendf(man, "%d gates loaded", gate_count);
            part1_log_line1 = log_manager_appendf(man, "executing gate %d", current_gate);
            char wire[2];
            assign_op(wire, "a");
            part1_log_line2 =
                log_manager_appendf(man, "value of wire 'a': %d", *get_wire(&wires, wire));
            section = DAY7_CALC1;
        }

        break;
        case DAY7_CALC1: {
            for (size_t igate = 0; igate < gate_count; ++igate)
                exec_gate(gates + igate, &wires);

            char wire[2];
            assign_op(wire, "a");
            uint16_t value_of_a = *get_wire(&wires, wire);
            sprintf(part1_log_line2, "value of wire 'a': %d", value_of_a);

            sprintf(part1_log_line1, "executing cycle %4llu", current_gate);
            ++current_gate;
            if (value_of_a) {
                section = DAY7_END_SUCCESS;
            }
        } break;
        }

        AOC_END_FRAME(man);
    }
}

#endif
