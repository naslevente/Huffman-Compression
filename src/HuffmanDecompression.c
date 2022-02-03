#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "HuffmanDecompression.h"

#define LINE_BUFFER 128

HuffmanTree* RecreateHuffmanTree(HuffmanTree* huffmanTree) {

    huffmanTree = (HuffmanTree*) malloc(sizeof(HuffmanTree) * 1);
    if(huffmanTree == NULL) {

        return NULL;
    }

    huffmanTree->leftElementChild = NULL;
    huffmanTree->rightElementChild = NULL;

    return huffmanTree;
}

HuffmanTree* HuffmanDecode(FILE* decodeFile, int* padding, HuffmanTree* huffmanTree) {

    huffmanTree = RecreateHuffmanTree(huffmanTree);

    unsigned char buffer[LINE_BUFFER];
    size_t length = LINE_BUFFER;
    unsigned char* line = (unsigned char*) malloc(sizeof(unsigned char) * LINE_BUFFER);
    if(line == NULL) {

        perror("---memory allocation failed---\n");
        return NULL;
    }
    line[0] = '\0';

    while(fgets(buffer, sizeof(buffer), decodeFile) != NULL) {

        size_t usedLength = strlen(line);
        size_t usedBuffer = strlen(buffer);

        if(length - usedLength < usedBuffer) {

            length *= 2;
            if((line = realloc(line, length)) == NULL) {

                perror("---failed to reallocate memory for buffer---\n");
                free(line);
                return NULL;
            }
        }

        strncpy(line + usedLength, buffer, length - usedLength);
        usedLength += usedBuffer;

        if(line[usedLength - 1] == '\n' && strlen(line) > 1) {

            huffmanTree = ProcessCode(line, huffmanTree);
            if(ProcessCode == NULL) {

                return NULL;
            }
            line[0] = '\0';
        }
    }

    if(line[0] != '\n') {

        *padding = line[0] - 48;
    }

    free(line);

    return huffmanTree;
}

HuffmanTree* ProcessCode(unsigned char* inputLine, HuffmanTree* huffmanTree) {

    unsigned char* bitsequence = (unsigned char*) malloc(sizeof(unsigned char) * (strlen(inputLine - 2)));
    if(bitsequence == NULL) {

        perror("---memory allocation failed---\n");
        return NULL;
    }
    bitsequence[strlen(inputLine) - 2] = '\0';
    
    unsigned char symbol;
    sscanf(inputLine, "%c: %s", &symbol, bitsequence);

    huffmanTree = AddToHuffmanTree(bitsequence, symbol, huffmanTree);
    if(huffmanTree == NULL) {

        perror("---memory allocation failed---\n");
        return NULL;
    }

    free(bitsequence);

    return huffmanTree;
}

HuffmanTree* CreateNewTreeElement(HuffmanTree* newElement, unsigned char symbol) {

    newElement = (HuffmanTree*) malloc(sizeof(HuffmanTree) * 1);
    if(newElement == NULL) {

        return NULL;
    }

    newElement->symbol = symbol;
    newElement->leftElementChild = NULL;
    newElement->rightElementChild = NULL;

    return newElement;
}

HuffmanTree* AddToHuffmanTree(unsigned char* input, unsigned char inputSymbol, HuffmanTree* huffmanTree) {

    HuffmanTree* iterator = huffmanTree;
    for(int i = 0; i < strlen(input); ++i) {

        unsigned char currentBit = input[i];
        if(currentBit == '1') {

            if(iterator->rightElementChild == NULL) {

                if(i == strlen(input) - 1) {

                    iterator->rightElementChild = CreateNewTreeElement(iterator->rightElementChild, inputSymbol);
                    if(iterator->rightElementChild == NULL) {

                        return NULL;
                    }

                    return huffmanTree;
                }
                else {

                    iterator->rightElementChild = CreateNewTreeElement(iterator->rightElementChild, '*');
                    if(iterator->rightElementChild == NULL) {

                        return NULL;
                    }

                    iterator = iterator->rightElementChild;
                }
            }
            else {

                iterator = iterator->rightElementChild;
            }
        }
        else {

            if(iterator->leftElementChild == NULL) {

                if(i == strlen(input) - 1) {

                    iterator->leftElementChild = CreateNewTreeElement(iterator->leftElementChild, inputSymbol);
                    if(iterator->leftElementChild == NULL) {

                        return NULL;
                    }

                    return huffmanTree;
                }
                else {

                    iterator->leftElementChild = CreateNewTreeElement(iterator->leftElementChild, '*');
                    if(iterator->leftElementChild == NULL) {

                        return NULL;
                    }

                    iterator = iterator->leftElementChild;
                }
            }
            else {

                iterator = iterator->leftElementChild;
            }
        }
    }

    return huffmanTree;
}

void PrintToFile(FILE* outputFile, HuffmanTree* huffmanTree, unsigned char* codedText, int padding, int length) {

    HuffmanTree* iterator = huffmanTree;
    unsigned char iteratingChar = 0x80;
    int count = 0;
    int total = 0;
    for(int i = 0; i < length; ++i) {

        int loopCondition = (i == length - 2) ? 8 - padding : 8;
        for(int k = 0; k < loopCondition; ++k) {

            unsigned char temp = iteratingChar & codedText[i];
            if(temp == iteratingChar) {

                if(iterator->rightElementChild == NULL) {

                    fprintf(outputFile, "%c", iterator->symbol);
                    iterator = huffmanTree;
                    if(i == (length - 1)) break;
                }

                iterator = iterator->rightElementChild;
            }
            else {

                if(iterator->leftElementChild == NULL) {

                    fprintf(outputFile, "%c", iterator->symbol);
                    iterator = huffmanTree;
                    if(i == (length - 1)) break;
                }

                iterator = iterator->leftElementChild;
            }

            iteratingChar = iteratingChar >> 1;
        }
        
        iteratingChar = 0x80;
    }
}