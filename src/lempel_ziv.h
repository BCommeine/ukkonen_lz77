//
// Created by bert on 22/11/18.
//

#ifndef UKKONEN_LEMPEL_ZIV_H
#define UKKONEN_LEMPEL_ZIV_H

#endif //UKKONEN_LEMPEL_ZIV_H

#include <stdint.h>
#include "ukkonen.h"

struct Triple {
    uint32_t start;
    uint32_t length;
    uint8_t character;
};

void codeer_lz ();

void output(uint32_t start, uint32_t length, uint8_t character);

struct Triple* read_triple();

void decodeer_lz ();
