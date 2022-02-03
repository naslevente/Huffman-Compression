#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "debugmalloc.h"

typedef struct HuffmanTree {

    int key;
    unsigned char symbol;

    int height;

    struct HuffmanTree* leftElementChild;
    struct HuffmanTree* rightElementChild;

    struct HuffmanTree* nextItem;
    struct HuffmanTree* previousItem;

} HuffmanTree;

#include "Queue.h"

bool BuildHuffmanTree(Queue** queue, bool* isEmpty);
bool ConnectTrees(HuffmanTree* lChild, HuffmanTree* rChild, HuffmanTree** newNode);