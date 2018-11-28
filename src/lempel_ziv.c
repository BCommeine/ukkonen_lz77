//
// Created by bert on 22/11/18.
//

#include <string.h>
#include "lempel_ziv.h"

void codeer_lz (){
    struct ActivePoint active_point;
    struct Tree* tree = malloc(sizeof(struct Tree));

    tree->root = create_edge();
    tree->root->begin_suffix = 0;
    tree->STRING_SIZE = 1024;

    // ACTIVE POINT
    active_point.activeChar = 0;
    active_point.activeNode = tree->root;
    active_point.activeLength = 0;

    tree->code = malloc(tree->STRING_SIZE * sizeof(char));
    tree->n = -1;

    read_char(tree);

    active_point.remaining = 1;
    tree->lastSplit= NULL;

    output (0,0, (uint8_t) tree->c);
    add_char(tree, &active_point);
    read_char(tree);

    uint32_t l = (uint32_t) 0;

    while (tree->c != EOF) {
        active_point.remaining++;
        tree->lastSplit = NULL;
        reset_active_edge(&active_point, tree->n);
        if (add_char(tree, &active_point)) {
            l++;
        } else {
            if (l > 0){
                output(tree->beg, l, (uint8_t) tree->c);
            } else {
                output(0, 0, (uint8_t ) tree->c);
            }
            l = (uint32_t) 0;
        }
        read_char(tree);
    }
//    print_and_free(tree);

}

void output(uint32_t start, uint32_t length, uint8_t character) {
//    printf("Begin: %u | Lengte: %u | Character: %c\n",(uint32_t) start, (uint32_t) length, character);
    fwrite(&start, sizeof(uint32_t), 1, stdout);
    fwrite(&length, sizeof(uint32_t), 1, stdout);
    fwrite(&character, sizeof(uint8_t), 1, stdout);

}

void decodeer_lz () {
    char* code = malloc(sizeof(char) * 10000);
    char* begin = code;
    struct Triple* triple;
    for (int i = 0; i < 2000; ++i) {
        triple = read_triple();
        for (uint index = 0; index < triple->length; index++) {
            *code++ = begin[index + triple->start];
        }
        *code++ = triple->character;
    }
    printf("%s", begin);
}

struct Triple* read_triple(){
    struct Triple* triple = malloc(sizeof(struct Triple));
    fread(&triple->start, sizeof(uint32_t), 1, stdin);
    fread(&triple->length, sizeof(uint32_t), 1, stdin);
    fread(&triple->character, sizeof(uint8_t), 1, stdin);
    return triple;
}