//
// Created by bert on 22/11/18.
//

#include <memory.h>
#include "lempel_ziv.h"

void codeer_lz (){
    struct ActivePoint active_point;
    struct Tree* tree = malloc(sizeof(struct Tree));

    tree->root = create_edge();
    tree->root->begin_suffix = 0;
    tree->STRING_SIZE = 800;

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

    while (tree->c != EOF) {
        read_char(tree);
        int begin = tree->n;
        search_match(tree);
        int end = tree->n;
        for (int i = begin; i <= end; i++) {
            tree->n = i;
            tree->c = tree->code[i];
            active_point.remaining++;
            tree->lastSplit = NULL;
            if (tree->c != EOF) {
                reset_active_edge(&active_point, tree->n);
                add_char(tree, &active_point);
            }
        }
    }

    free_tree(tree);
}

struct Position* create_position(struct Tree* tree) {
    struct Position* position = calloc(1, sizeof(struct Position));
    position->edge = tree->root;
    position->active_char = 0;
    position->length = 0;
    return position;
}

void search_match(struct Tree* tree) {
    struct Position* position = create_position(tree);
    while (search_char(tree, position)) {
        read_char(tree);
    }
    free(position);
}

bool search_char(struct Tree* tree, struct Position* position) {
    if (tree->c == EOF) {
        output(position->edge->begin_suffix, (uint32_t) position->length, (uint8_t) '\0');
        return false;
    }
    int length = position->edge->end == NULL? 0:  *position->edge->end - position->edge->start + 1;
    if (position->active_char < length) {
        if (tree->code[position->edge->start + position->active_char] == tree->c) {
            position->active_char++;
            position->length++;
            return true;
        } else {
            output(position->edge->begin_suffix, (uint32_t) position->length, (uint8_t) tree->c);
            return false;
        }
    } else {
        if (position->edge->children != NULL && position->edge->children[(unsigned int) tree->c] != NULL){
            position->active_char -= length;
            position->edge =  position->edge->children[(unsigned int) tree->c];
            position->active_char++;
            position->length++;
            return true;
        } else {
            output(position->edge->begin_suffix, (uint32_t) position->length, (uint8_t) tree->c);
            return false;
        }
    }
}

void output(uint32_t start, uint32_t length, uint8_t character) {
    if (length > 0) {
        fwrite(&start, sizeof(uint32_t), 1, stdout);
        fwrite(&length, sizeof(uint32_t), 1, stdout);
        fwrite(&character, sizeof(uint8_t), 1, stdout);
    } else {
        int zero = 0;
        fwrite(&zero, sizeof(uint32_t), 1, stdout);
        fwrite(&zero, sizeof(uint32_t), 1, stdout);
        fwrite(&character, sizeof(uint8_t), 1, stdout);
    }
}

void decodeer_lz () {
    char* code = calloc(1000000, sizeof(char));
    char* begin = code;
    struct Triple* triple = read_triple();
    while (triple->character != '\0') {
        for (uint32_t index = 0; index < triple->length; index++) {
            *code++ = begin[index + triple->start];
        }
        *code++ = triple->character;
        free(triple);
        triple = read_triple();
    }
    for (uint32_t index = 0; index < triple->length; index++) {
        *code++ = begin[index + triple->start];
    }
    printf("%s", begin);
    free(begin);
    free(triple);
}

struct Triple* read_triple(){
    struct Triple* triple = calloc(1, sizeof(struct Triple));
    fread(&triple->start, sizeof(uint32_t), 1, stdin);
    fread(&triple->length, sizeof(uint32_t), 1, stdin);
    fread(&triple->character, sizeof(uint8_t), 1, stdin);
    return triple;
}