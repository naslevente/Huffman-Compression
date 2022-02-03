#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "HashMap.h"

hMap* InitializeHashMap(hMap* hashMap) {

    hashMap = (hMap*) malloc(sizeof(hMap) * 1);
    if(hashMap == NULL) {

        perror("failed to allocate memory\n");
        return NULL;
    }

    hashMap->hashMap = (int*) malloc(sizeof(int) * 255);
    if(hashMap->hashMap == NULL) {

        perror("failed to allocate memory\n");
        return NULL;
    }

    for(int i = 0; i < 255; ++i) {

        hashMap->hashMap[i] = 0;
    }

    return hashMap;
}

bool IsInMap(hMap* hashMap, unsigned char key) {

    return (hashMap->hashMap[key] != 0) ? true : false;
}

hMap* AddToElement(hMap* hashMap, unsigned char key) {

    hashMap->hashMap[key] += 1;
    return hashMap;
}

void FreeHashMapMemory(hMap* hashMap) {

    free(hashMap->hashMap);
    free(hashMap);

    return;
}

void PrintHashMapItems(hMap* hashMap) {

    for(int i = 0; i < 255; ++i) {

        unsigned char key = i;
        printf("%c: %d\n", key, hashMap->hashMap[i]);
    }

    return;
}

