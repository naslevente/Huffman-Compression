#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "debugmalloc.h"

typedef struct HashMap {

    int* hashMap;

} hMap;

hMap* InitializeHashMap(hMap* hashMap);
bool IsInMap(hMap* hashMap, unsigned char key);
hMap* AddToElement(hMap* hashMap, unsigned char key);
void FreeHashMapMemory(hMap* hashMap);
void PrintHashMapItems(hMap* hashMap);