//
// Created by bert on 18/11/18.
//

#ifndef UKKONEN_UKKONEN_TEST_H
#define UKKONEN_UKKONEN_TEST_H

#include "ukkonen/ukkonen.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool test_number_of_leaves(char* fileName);

bool test_all_suffixes(char* fileName);

int check_suffix(struct Edge* edge, char* suffix, int position);

void count_leaves(struct Edge* edge, int* counter);

#endif //UKKONEN_UKKONEN_TEST_H