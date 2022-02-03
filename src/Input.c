#include "Input.h"

#define LINE_BUFFER 128

unsigned char* GetStringFromInput(unsigned char* inputString) {

    inputString = (unsigned char*) malloc(sizeof(unsigned char) * 1);
    if(inputString == NULL) {

        perror("---memory allocation failed---\n");
        return NULL;
    }
    inputString[0] = '\0';

    unsigned char inputChar;
    while(scanf("%c", &inputChar) && inputChar != '\n') {

        int previousLength = strlen(inputString);

        inputString = (unsigned char*) realloc(inputString, sizeof(unsigned char) * (previousLength + 2));
        inputString[previousLength + 1] = '\0';
        inputString[previousLength] = inputChar;
    }

    return inputString;
}

hMap* ReadFromFile_encode(FILE* inputFile, hMap* hashMap, int* hashMapSize, unsigned char** inputString) {

    unsigned char* string = (unsigned char*) malloc(sizeof(unsigned char) * 1);
    if(string == NULL) {

        perror("---memory allocation failed---\n");
        return NULL;
    }
    string[0] = '\0';

    unsigned char buffer[LINE_BUFFER];
    size_t length = sizeof(buffer);
    unsigned char* line = (unsigned char*) malloc(sizeof(unsigned char) * LINE_BUFFER);
    if(line == NULL) {

        perror("---memory allocation failed---\n");
        return NULL;
    }
    line[0] = '\0';

    while(fgets(buffer, sizeof(buffer), inputFile) != NULL) {

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

        if(line[usedLength - 1] == '\n') {

            string = (unsigned char*) realloc(string, sizeof(unsigned char) * (strlen(string) + strlen(line) + 1));
            strcat(string, line);

            hashMap = FillHashMap(hashMap, line, hashMapSize);
            line[0] = '\0';
        }
    }

    if(line[0] != '\n') {

        string = (unsigned char*) realloc(string, sizeof(unsigned char) * (strlen(string) + strlen(line) + 1));
        strcat(string, line);

        hashMap = FillHashMap(hashMap, line, hashMapSize);
        line[0] = '\0';
    }

    *inputString = string;

    fclose(inputFile);
    free(line);

    return hashMap;
}

unsigned char* ReadFromFile_decode(FILE* inputFile, unsigned char* charArray, int* length) {

    charArray = (unsigned char*) malloc(sizeof(unsigned char));
    if(charArray == NULL) {

        perror("---memory allocation failed---\n");
        return NULL;
    }

    int c;
    int index = 0;
    while((c = fgetc(inputFile)) != EOF) {

        charArray = (unsigned char*) realloc(charArray, sizeof(unsigned char) * (index + 1));
        if(charArray == NULL) {

            perror("---failed to reallocate memory---\n");
        }
        charArray[index++] = (unsigned char) c;
    }

    *length = (++index);

    return charArray;
}