#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Queue.h"
#include "HuffmanTree.h"
#include "HuffmanCompression.h"
#include "debugmalloc.h"

HuffmanTree* RecreateHuffmanTree(HuffmanTree* huffmanTree);
HuffmanTree* HuffmanDecode(FILE* decodeFile, int* padding, HuffmanTree* huffmanTree);
HuffmanTree* ProcessCode(unsigned char* inputLine, HuffmanTree* huffmanTree);
HuffmanTree* CreateNewTreeElement(HuffmanTree* newElement, unsigned char symbol);
HuffmanTree* AddToHuffmanTree(unsigned char* input, unsigned char inputSymbol, HuffmanTree* huffmanTree);
void PrintToFile(FILE* outputFile, HuffmanTree* huffmanTree, unsigned char* codedText, int padding, int length);