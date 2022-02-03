#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "Queue.h"
 
bool BuildHuffmanTree(Queue** queue, bool* isEmpty) {

    HuffmanTree* leftChild = NULL;
    leftChild = PopMin(queue, isEmpty);

    HuffmanTree* rightChild = NULL;
    if(!(*isEmpty)) {

        rightChild = PopMin(queue, isEmpty);
    }

    HuffmanTree* newNode = NULL;
    if(leftChild != NULL) {

        int result = ConnectTrees(leftChild, rightChild, &newNode);
        if(result) {

            AddToLinkedList(queue, newNode);
            return true;
        }

        if(result == 1) {

            perror("argument error passing to function ConnectTrees\n");
        }
        else {

            perror("failed to allocate memory\n");
        }
    }

    return false;
}

bool ConnectTrees(HuffmanTree* lChild, HuffmanTree* rChild, HuffmanTree** newNode) {

    if(lChild != NULL && rChild != NULL) {

        *newNode = (HuffmanTree*) malloc(sizeof(HuffmanTree));
        if(*newNode == NULL) {

            return false;
        }

        (*newNode)->key = lChild->key + rChild->key;
        (*newNode)->height = lChild->height > rChild->height ? lChild->height + 1 : rChild->height + 1;

        (*newNode)->leftElementChild = lChild;
        (*newNode)->rightElementChild = rChild;
    }

    return true;
}