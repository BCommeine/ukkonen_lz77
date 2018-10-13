//
// Created by bert on 12/10/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "ukkonen.h"

void readFile(char *fileName) {
    FILE *file = fopen(fileName, "r");
    char *code;
    int n = -1;
    int c;
    int* current = malloc(sizeof(int));
    struct Edge* root = malloc (sizeof(struct Edge));

    Edge* activeEdge = root;
    int activeLength = 0;
    int remainder = 1;

    if (file == NULL) return;
    long f_size = ftell(file);
    code = malloc(f_size);

    while ((c = fgetc(file)) != EOF) {
        //  Go to next character;
        code[++n] = (char)c;
        *current = (int) n;

        // Check if we're dealing with duplicates
        if (activeEdge == root && activeLength == 0) {
            if (root->children[c] == NULL) {
                struct Edge* new = malloc (sizeof(struct Edge));
                new->start = n;
                new->end = current;
                root->children[c] = new;
            } else {
                activeEdge = root->children[c];
                activeLength = 1;
                remainder++;
            }
        } else {
            if  (activeLength < *activeEdge->end - activeEdge->start) {
                if (code[activeEdge->start + activeLength] == c) {
                    activeLength++;
                    remainder++;
                } else {
                    struct Edge* newEdge = malloc (sizeof(struct Edge));
                    newEdge->start = n;
                    newEdge->end = current;
                    activeEdge->children[c] = newEdge;
                    struct Edge* splitEdge = malloc (sizeof(struct Edge));
                    splitEdge->start = activeEdge->start + activeLength;
                    splitEdge->end = activeEdge->end;
                    activeEdge->children[code[activeEdge->start + activeLength]] = splitEdge;
                    int end = malloc(sizeof(int));
                    end = activeEdge->start + activeLength;
                    activeEdge->end = &end;
                }
            } else {
                if (activeEdge->children[c] == NULL) {
                    struct Edge* new = malloc (sizeof(struct Edge));
                    new->start = n;
                    new->end = current;
                    activeEdge->children[c] = new;
                } else {
                    activeEdge = activeEdge->children[c];
                    activeLength = 1;
                    remainder++;
                }
            }
        }
//        //  Check if already in root
//        if (root->children[c] == NULL){
//            struct Edge* new = malloc (sizeof(struct Edge));
//            new->start = n;
//            new->end = current;
//            root->children[c] = new;
//        } else {
//            if (activeEdge == NULL) {
//                activeEdge = root->children[c];
//                activeLength = 1;
//                remainder++;
//                printf("%c DOUBLE \n", c);
//
//            }
//        }

//        printf("%c\n", c);
    }

    code[n] = '\0';
}