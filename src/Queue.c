#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "Queue.h"

void PrintQueueELements(Queue* queue) {

    HuffmanTree* iterator = queue->firstItem;
    while(iterator != NULL) {

        printf("%c", iterator->symbol);
        iterator = iterator->nextItem;
    }
    printf("\n");

    return;
}

bool InitializeQueue(Queue** queue) {

    *queue = (Queue*) malloc(sizeof(Queue));
    if(*queue == NULL) {

        return false;
    }

    (*queue)->firstItem = NULL;
    (*queue)->lastItem = NULL;

    return true;
}

bool AddNewElement(Queue** queue, int key, unsigned char symbol) {

    HuffmanTree* newElement = (HuffmanTree*) malloc(sizeof(HuffmanTree));
    if(newElement == NULL) {

        return false;
    }

    newElement->key = key;
    newElement->symbol = symbol;
    newElement->height = 1;

    newElement->rightElementChild = NULL;
    newElement->leftElementChild = NULL;

    AddToLinkedList(queue, newElement);

    return true;
}

void RestoreMin(Queue** queue) {

    HuffmanTree* iterator = (*queue)->lastItem;
    while(iterator != (*queue)->firstItem) {

        if(iterator->key < iterator->previousItem->key) {

            if(iterator->previousItem == (*queue)->firstItem) {

                (*queue)->firstItem = iterator;
                iterator->previousItem->nextItem = iterator->nextItem;
                iterator->nextItem = iterator->previousItem;
                if((*queue)->lastItem == iterator) {

                    iterator->previousItem->nextItem = NULL;
                    (*queue)->lastItem = iterator->previousItem;
                }
                else {

                    iterator->previousItem->nextItem->previousItem = iterator->previousItem;
                }

                iterator->previousItem = NULL;
                iterator->nextItem->previousItem = iterator;
            }
            else {

                iterator->previousItem->previousItem->nextItem = iterator;
                iterator->previousItem->nextItem = iterator->nextItem;
                iterator->nextItem = iterator->previousItem;
                if((*queue)->lastItem == iterator) {

                    iterator->previousItem->nextItem = NULL;
                    (*queue)->lastItem = iterator->previousItem;
                }
                else {

                    iterator->previousItem->nextItem->previousItem = iterator->previousItem;
                }

                iterator->previousItem = iterator->nextItem->previousItem;
                iterator->nextItem->previousItem = iterator;
            }
        }
        else {

            iterator = iterator->previousItem;
        }
    }
}

HuffmanTree* PopMin(Queue** queue, bool* isEmpty) {

    HuffmanTree* minElement = (*queue)->firstItem;
    if((*queue)->firstItem == (*queue)->lastItem) {

        *isEmpty = true;
        (*queue)->firstItem = NULL;
        (*queue)->lastItem = NULL;
    }
    else {

        // remove minElement from linked list
        (*queue)->firstItem = minElement->nextItem;
        minElement->nextItem->previousItem = NULL;
    }

    return minElement;
}

void AddToLinkedList(Queue** queue, HuffmanTree* newElement) {

    HuffmanTree* iterator = (*queue)->firstItem;
    while(iterator != NULL && iterator != (*queue)->lastItem) {

        iterator = iterator->nextItem;
    }

    if(iterator == NULL) {

        (*queue)->firstItem = newElement;
        newElement->previousItem = (*queue)->firstItem;
        newElement->nextItem = (*queue)->lastItem;
        (*queue)->lastItem = newElement;
    }
    else {

        iterator->nextItem = newElement;
        newElement->previousItem = iterator;
        newElement->nextItem = NULL;
        (*queue)->lastItem = newElement;
    }

    RestoreMin(queue);
}