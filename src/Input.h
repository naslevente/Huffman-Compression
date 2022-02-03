#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "HashMap.h"
#include "HuffmanCompression.h"
#include "debugmalloc.h"

unsigned char* GetStringFromInput(unsigned char* inputString);
hMap* ReadFromFile_encode(FILE* inputFile, hMap* hashMap, int* hashMapSize, unsigned char** inputString);
unsigned char* ReadFromFile_decode(FILE* inputFile, unsigned char* charArray, int* length);