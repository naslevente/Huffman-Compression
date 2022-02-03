#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "HashMap.h"
#include "Queue.h"
#include "HuffmanCompression.h"

hMap* FillHashMap(hMap* hashMap, unsigned char* input, int* hashMapSize) {

    for(int i = 0; i < strlen(input); ++i) {

        unsigned char character = input[i];
        if(IsInMap(hashMap, character)) {

            hashMap = AddToElement(hashMap, character);
        }
        else {

            hashMap = AddToElement(hashMap, character);
            (*hashMapSize) += 1;
        }
    }

    return hashMap;
}

Queue* HuffScan(unsigned char* input, hMap* hashMap) {

    Queue* queue = NULL;
    for(int i = 0; i < 255; ++i) {

        unsigned char key = i;
        if(hashMap->hashMap[i] != 0) {

            if(queue == NULL) {

                if(!InitializeQueue(&queue) || !AddNewElement(&queue, hashMap->hashMap[i], key)) {

                    perror("---memory allocation failed---\n");
                    return NULL;
                }
            }
            else {

                if(!AddNewElement(&queue, hashMap->hashMap[i], key)) {

                    perror("---memory allocation failed---\n");
                    return NULL;
                }
            }
        }
    }

    return queue;
}

void TraverseTree(EncodedElement*** elements, int* index, HuffmanTree* currentElement, 
    unsigned char* bitsequence, int height, FILE* decodeTable) {

    if(currentElement->leftElementChild == NULL && currentElement->rightElementChild == NULL) {

        unsigned char* finalBitsequence = (unsigned char*) malloc(sizeof(unsigned char) * (height + 1));
        finalBitsequence[height] = '\0';
        strcpy(finalBitsequence, bitsequence);

        fprintf(decodeTable, "%c: %s\n", currentElement->symbol, finalBitsequence);

        EncodedElement* newElement = (EncodedElement*) malloc(sizeof(EncodedElement));
        newElement->symbol = currentElement->symbol;
        newElement->bitsequence = finalBitsequence;

        (*elements)[(*index)++] = newElement;
        return;
    }

    unsigned char leftSequence[strlen(bitsequence)];
    unsigned char rightSequence[strlen(bitsequence)];

    strcpy(leftSequence, bitsequence);
    strcpy(rightSequence, bitsequence);

    strcat(leftSequence, "0");
    TraverseTree(elements, index, currentElement->leftElementChild, leftSequence, height, decodeTable);

    if(currentElement->rightElementChild != NULL) {

        strcat(rightSequence, "1");
        TraverseTree(elements, index, currentElement->rightElementChild, rightSequence, height, decodeTable);
    }
}

unsigned char* EncodeInput(EncodedElement** elements, unsigned char* input, int queueSize, FILE* decodeTable,
    FILE* outputFile) {

    unsigned char* output = (unsigned char*) malloc(sizeof(unsigned char) * strlen(input));
    if(output == NULL) {

        perror("---memory allocation failed---\n");
        return NULL;
    }

    output[0] = '\0';

    int count = 0;
    unsigned char codedChar = 0x00;
    int index = 0;

    for(int i = 0; i < strlen(input); ++i) {

        unsigned char currentChar = input[i];
        EncodedElement* encodedElement = NULL;
        for(int k = 0; k < queueSize; ++k) {

            if(elements[k]->symbol == currentChar) {

                encodedElement = elements[k];
                break;
            }
        }

        for(int j = 0; j < strlen(encodedElement->bitsequence); ++j) {

            if(count == 7) {

                if(encodedElement->bitsequence[j] == '1') {

                    codedChar = (codedChar << 1) | 0x01;
                }
                else {

                    codedChar = codedChar << 1;
                }

                fprintf(outputFile, "%c", codedChar);
                output[index++] = codedChar;
                output[index] = '\0';

                count = 0;
                codedChar = 0x00;                
            }
            else {

                if(encodedElement->bitsequence[j] == '1') {

                    codedChar = (codedChar << 1) | 0x01;
                }
                else {

                    codedChar = codedChar << 1;
                }
                ++count;
            }
        }
    }

    int padding = 0;
    while(count <= 7) {

        ++padding;
        codedChar = codedChar << 1;
        ++count;
    }

    fprintf(outputFile, "%c", codedChar);
    fprintf(decodeTable, "%d", padding);
    output[index++] = codedChar;
    output[index] = '\0';

    return output;
}

FILE* HuffEncode(Queue* queue, int queueSize, unsigned char* input, FILE* outputFile, FILE* decodeTable) {

    bool isEmpty = false;
    while(!isEmpty) {

        if(!BuildHuffmanTree(&queue, &isEmpty)) {

            fclose(outputFile);
            fclose(decodeTable);
            perror("---failed to build Huffman Tree---\n");
            return NULL;
        }
    }

    EncodedElement** elements = (EncodedElement**) malloc(sizeof(EncodedElement*) * queueSize);
    if(elements == NULL) {

        perror("---memory allocation failed---\n");
        exit(EXIT_FAILURE);
    }

    unsigned char* leftSequence = "0";
    unsigned char* rightSequence = "1";
    int index = 0;
    int height = queue->firstItem->height;

    TraverseTree(&elements, &index, queue->firstItem->leftElementChild, leftSequence, height, decodeTable);
    TraverseTree(&elements, &index, queue->firstItem->rightElementChild, rightSequence, height, decodeTable);

    unsigned char* outputString;
    outputString = EncodeInput(elements, input, queueSize, decodeTable, outputFile);

    for(int i = 0; i < queueSize; ++i) {

        EncodedElement* temp = elements[i];
        free(temp->bitsequence);
        free(temp);
    }
    free(elements);

    if(outputString == NULL) {

        free(outputString);
        fclose(outputFile);
        fclose(decodeTable);
        return NULL;
    }

    free(outputString);

    return outputFile;
}