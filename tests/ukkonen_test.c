//
// Created by bert on 18/11/18.
//
#include "ukkonen_test.h"

bool test_number_of_leaves(char* fileName) {
    freopen(fileName, "r", stdin);
    struct Tree* tree = build_tree();
    int* counter = malloc(sizeof(int));
    count_leaves(tree->root, counter);
    print_and_free(tree);
    int result = *counter;
    free(counter);
    return result == strlen(tree->code);
}

void count_leaves(struct Edge* edge, int* counter) {
    if (is_leaf(edge)) {
        ++*counter;
    } else {
        for (int i = 0; i < ASCII_SIZE; i++) {
            if (edge->children[i] != NULL) {
                count_leaves(edge->children[i], counter);
            }
        }
    }
}

bool test_all_suffixes(char* fileName) {
    freopen(fileName, "r", stdin);
    struct Tree* tree = build_tree();
    int length = strlen(tree->code);
    for (int i = 0; i < length; i++) {
        int asdf = check_suffix(tree->root->children[tree->code[i]], tree->code, i);
        if (asdf != length) {
            return false;
        }
    }
    print_and_free(tree);
    return true;
}

int check_suffix(Edge* edge, char* suffix, int position) {
    position = position + *edge->end - edge->start + 1;
    if (is_leaf(edge)) {
        return position;
    } else {
            check_suffix(edge->children[suffix[position]], suffix, position);
    }
}