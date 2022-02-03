#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "debugmalloc.h"

// Priority queue eleje es veget tartalmazo struct. Innen lehet tovabb
// lepni a kovetkezo elemhez, de lehet a vegetol kezdeni. A vege elonyos amikor hozzaadunk
// a queue-hoz egy uj elemet. 
typedef struct Queue {

    struct HuffmanTree* lastItem;
    struct HuffmanTree* firstItem;

} Queue;

#include "HuffmanTree.h"

void PrintQueueElements(Queue* queue);

bool InitializeQueue(Queue** queue);
bool AddNewElement(Queue** queue, int key, unsigned char symbol);
void RestoreMin(Queue** queue);
HuffmanTree* PopMin(Queue** queue, bool* isEmpty);
void AddToLinkedList(Queue** queue, HuffmanTree* newElement);