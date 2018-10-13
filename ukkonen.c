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

    Edge* activeEdge;
    Edge* activeNode = root;
    int activeLength = 0;
    int remainder = 1;

    if (file == NULL) return;
    long f_size = ftell(file);
    code = malloc(f_size);

    while ((c = fgetc(file)) != EOF) {
        //  Go to next character;
        code[++n] = (char)c;
        *current = (int) n;

        do {
            // Check if we're dealing with duplicates
            if (activeNode == root && activeLength == 0) {
                if (root->children[c] == NULL) {
                    struct Edge* new = malloc (sizeof(struct Edge));
                    new->start = n;
                    new->end = current;
                    root->children[c] = new;
                    break;
                } else {
                    activeEdge = root->children[c];
                    activeLength = 1;
                    remainder++;
                    break;
                }
            } else {
                if (activeLength < *activeEdge->end - activeEdge->start) {
                    // Er zijn nog characters op de huidige edge
                    if (code[activeEdge->start + activeLength] == c) {
                        // Het volgend karakter zit al in de trie
                        activeLength++;
                        remainder++;
                        break;
                    } else {
                        // Split een edge in de originele en een nieuwe
                        struct Edge *newEdge = malloc(sizeof(struct Edge));
                        newEdge->start = n;
                        newEdge->end = current;
                        activeEdge->children[c] = newEdge;
                        struct Edge *splitEdge = malloc(sizeof(struct Edge));
                        splitEdge->start = activeEdge->start + activeLength;
                        splitEdge->end = activeEdge->end;
                        activeEdge->children[code[activeEdge->start + activeLength]] = splitEdge;
                        int end = malloc(sizeof(int));
                        end = activeEdge->start + activeLength;
                        activeEdge->end = &end;
                        remainder--;
                        if (activeNode == root) {
                            activeEdge = root->children[code[--activeLength]];
                        }
                    }
                } else {
                    // Vorige c is het laatste karakter op de edge
                    if (activeEdge->children[c] == NULL) {
                        struct Edge *new = malloc(sizeof(struct Edge));
                        new->start = n;
                        new->end = current;
                        activeEdge->children[c] = new;
                        remainder--;
                    } else {
                        activeNode = activeEdge;
                        activeEdge = activeEdge->children[c];
                        activeLength = 1;
                        remainder++;
                        break;
                    }
                }
            }
        } while (1);
      }
}
