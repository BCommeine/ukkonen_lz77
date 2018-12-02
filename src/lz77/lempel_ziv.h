//
// Created by bert on 22/11/18.
//

#ifndef UKKONEN_LEMPEL_ZIV_H
#define UKKONEN_LEMPEL_ZIV_H

#include <stdint.h>
#include "../ukkonen/ukkonen.h"

struct Triple {
    uint32_t start;
    uint32_t length;
    uint8_t character;
};

struct Position {
    Edge* edge;
    int active_char;
    int length;
};

void codeer_lz ();

struct Position* create_position(struct Tree* tree);

void search_match(struct Tree* tree);

bool search_char(struct Tree* tree, struct Position* position);

void output(uint32_t start, uint32_t length, uint8_t character);

struct Triple* read_triple();

void decodeer_lz ();

#endif //UKKONEN_LEMPEL_ZIV_H