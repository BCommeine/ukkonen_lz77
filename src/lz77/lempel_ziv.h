//
// Created by bert on 22/11/18.
//

#ifndef UKKONEN_LEMPEL_ZIV_H
#define UKKONEN_LEMPEL_ZIV_H

#include <stdint.h>
#include "../ukkonen/ukkonen.h"

#define MEMORY_LIMIT 1024000

struct Triple {
    uint32_t start;
    uint32_t length;
    uint8_t character;
};

struct Position {
    Edge* edge;
    unsigned int active_char;
    unsigned int length;
};

struct Decoder {
    unsigned int n;
    unsigned int STRING_SIZE;
    char* begin;
};

void codeer_lz (bool optimize);

struct Position* create_position(struct Tree* tree);

void search_match(struct Tree* tree, bool optimize);

void next_char(struct Tree* tree, bool optimize);

bool search_char(struct Tree* tree, struct Position* position);

void output(uint32_t start, uint32_t length, uint8_t character);

struct Triple* read_triple();

void decodeer_lz (bool optimize);

void write_char(struct Decoder* dec, char character, bool optimize);
#endif //UKKONEN_LEMPEL_ZIV_H