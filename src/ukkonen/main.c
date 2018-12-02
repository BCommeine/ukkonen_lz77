//
// Created by bert on 30/11/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "ukkonen.h"

int main(int argc, char *argv[]) {
    freopen("../tests/testfiles/test_four.txt","r", stdin);
    struct Tree* tree = build_tree();
    printf("%s", tree->code);
    free_tree(tree);
}