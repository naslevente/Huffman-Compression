#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "Queue.h"
#include "HashMap.h"
#include "debugmalloc.h"

typedef struct EncodedElement {

    unsigned char* bitsequence;
    unsigned char symbol;

} EncodedElement;

hMap* FillHashMap(hMap* hashMap, unsigned char*input, int* hashMapSize);
Queue* HuffScan(unsigned char* input, hMap* hashMap);
void TraverseTree(EncodedElement*** elements, int* index, HuffmanTree* currentElement, 
    unsigned char* bitsequence, int height, FILE* decodeTable);
unsigned char* EncodeInput(EncodedElement** elements, unsigned char* input, int queueSize, FILE* decodeTable,
    FILE* outputFile);
FILE* HuffEncode(Queue* queue, int queueSize, unsigned char* input, FILE* outputFile, FILE* decodeTable);