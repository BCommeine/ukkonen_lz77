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
    unsigned int active_char;
    unsigned int length;
};

struct Decoder {
    unsigned int n;
    unsigned int STRING_SIZE;
    char* begin;
};

void codeer_lz ();

struct Position* create_position(struct Tree* tree);

void search_match(struct Tree* tree);

bool search_char(struct Tree* tree, struct Position* position);

void output(uint32_t start, uint32_t length, uint8_t character);

struct Triple* read_triple();

void decodeer_lz ();

void write_char(struct Decoder* dec, char character);

#endif //UKKONEN_LEMPEL_ZIV_H