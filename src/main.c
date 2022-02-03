#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "Input.h"
#include "HuffmanCompression.h"
#include "HuffmanDecompression.h"
#include "HashMap.h"
#include "HuffmanTree.h"
#include "Queue.h"
#include "debugmalloc.h"

#define LINE_BUFFER 128

void FreeTreeMemory(HuffmanTree* currentElement) {

    if(currentElement == NULL) {

        return;
    }

    FreeTreeMemory(currentElement->leftElementChild);
    FreeTreeMemory(currentElement->rightElementChild);

    free(currentElement->leftElementChild);
    free(currentElement->rightElementChild);

    currentElement->previousItem = NULL;
    
    return;
}

void FreeFileNames(char* inputFile, char* outputFile, char* decodeTableFile) {

    free(inputFile);
    free(outputFile);
    free(decodeTableFile);
}

void CloseFiles(FILE* inputFile, FILE* outputFile, FILE* decodeFile) {

    fclose(inputFile);
    fclose(outputFile);
    fclose(decodeFile);
}

void Encoding() {

    unsigned char* inputFile;
    unsigned char* outputFile;
    unsigned char* decodeTableFile;

    printf("-----------------------------------\n");
    printf("Gepelje be a kodolando .txt fajlt:\n");
    getchar();
    inputFile = GetStringFromInput(inputFile);

    printf("-----------------------------------\n");
    printf("Gepelje be az output .txt fajlt:\n");
    outputFile = GetStringFromInput(outputFile);

    printf("-----------------------------------\n");
    printf("Gepelje be az ures kod tabla .txt fajlt:\n");
    decodeTableFile = GetStringFromInput(decodeTableFile);

    if(inputFile == NULL || outputFile == NULL || decodeTableFile == NULL) {

        FreeFileNames(inputFile, outputFile, decodeTableFile);
        return;
    }

    FILE* input = fopen(inputFile, "r");
    FILE* output = fopen(outputFile, "wb");
    FILE* decodeTable = fopen(decodeTableFile, "wt");
    if(input == NULL || output == NULL || decodeTable == NULL) {

        perror("---unable to open either input, output or decode table file---\n");
        FreeFileNames(inputFile, outputFile, decodeTableFile);

        if(input != NULL) { fclose(input); }
        else if(output != NULL) { fclose(output); }
        else if(decodeTable != NULL) { fclose(decodeTable); }

        return;
    }

    hMap* hashMap = NULL;
    hashMap = InitializeHashMap(hashMap);
    int hashMapSize = 0;
    unsigned char* inputString;
    hashMap = ReadFromFile_encode(input, hashMap, &hashMapSize, &inputString);

    if(hashMap == NULL) {

        FreeFileNames(inputFile, outputFile, decodeTableFile);
        CloseFiles(input, output, decodeTable);
        return;
    }

    Queue* queue = NULL;
    queue = HuffScan(inputString, hashMap);
    if(queue == NULL) {

        FreeFileNames(inputFile, outputFile, decodeTableFile);
        CloseFiles(input, output, decodeTable);
        return;
    }

    output = HuffEncode(queue, hashMapSize, inputString, output, decodeTable);
    if(output == NULL) {

        FreeFileNames(inputFile, outputFile, decodeTableFile);
        CloseFiles(input, output, decodeTable);
        FreeHashMapMemory(hashMap);
        FreeTreeMemory(queue->firstItem);
        return;
    }

    FreeHashMapMemory(hashMap);
    FreeTreeMemory(queue->firstItem);
    FreeFileNames(inputFile, outputFile, decodeTableFile);

    free(queue->firstItem);
    free(queue);
    free(inputString);

    fclose(output);
    fclose(decodeTable);

    printf("*\n*\n*\n");
    printf("A kodolas megvan! Az eredmenyek megtekinthetok a fajlokban. Viszontlatasra!\n");
}


void Decoding() {

    unsigned char* decodeTableFile;
    unsigned char* outputFile;
    unsigned char* inputFile;

    printf("-----------------------------------\n");
    printf("Gepelje be a kodolt .txt fajl nevet: \n");
    getchar();
    inputFile = GetStringFromInput(inputFile);

    printf("-----------------------------------\n");
    printf("Gepelje be az ures output szoveg fajl nevet: \n");
    outputFile = GetStringFromInput(outputFile);

    printf("-----------------------------------\n");
    printf("Gepelje be a decode tabla fajl nevet: \n");
    decodeTableFile = GetStringFromInput(decodeTableFile);

    if(inputFile == NULL || outputFile == NULL || decodeTableFile == NULL) {

        FreeFileNames(inputFile, outputFile, decodeTableFile);
        return;
    }

    FILE* decodeTable = fopen(decodeTableFile, "r");
    FILE* input = fopen(inputFile, "rb");
    FILE* output = fopen(outputFile, "wt");
    if(decodeTable == NULL || output == NULL || input == NULL) {

        perror("---unable to open either input, output or decode table file---\n");
        FreeFileNames(inputFile, outputFile, decodeTableFile);

        if(input != NULL) { fclose(input); }
        else if(output != NULL) { fclose(output); }
        else if(decodeTable != NULL) { fclose(decodeTable); }

        return;
    }

    HuffmanTree* huffmanTree = NULL;
    int padding = 0;
    huffmanTree = HuffmanDecode(decodeTable, &padding, huffmanTree);
    if(huffmanTree == NULL) {

        FreeFileNames(inputFile, outputFile, decodeTableFile);
        CloseFiles(input, output, decodeTable);
        return;
    }

    int length;
    unsigned char* codedText;
    codedText = ReadFromFile_decode(input, codedText, &length);
    if(codedText == NULL) {

        FreeFileNames(inputFile, outputFile, decodeTableFile);
        CloseFiles(input, output, decodeTable);
        FreeTreeMemory(huffmanTree);
        return;
    }

    PrintToFile(output, huffmanTree, codedText, padding, length);

    free(codedText);
    FreeFileNames(inputFile, outputFile, decodeTableFile);
    CloseFiles(input, output, decodeTable);
    FreeTreeMemory(huffmanTree);
    free(huffmanTree);

    printf("*\n*\n*\n");
    printf("A dekodolas megvan! Az eredmenyek megtekinthetok a fajlokban. Viszontlatasra!\n");

    return;
}

int main() {

    printf("Kodolas (1), dekodolas (2), kilepes (egyeb karakter):\n");
    printf("-----------------------------------\n-----------------------------------\n");
    unsigned char menuPick;
    scanf("%c", &menuPick);
    if(menuPick == '1') {

        Encoding();
    }
    else if(menuPick == '2') {

        Decoding();
    }
    else {

        printf("Viszontlatasra!\n");
        return 0;
    }

    return 0;
}