#ifndef C_MAP
#define C_MAP

#include <stdlib.h>
#include "c-string.h"

typedef struct {
    string *key;
    int value;
    int isOccupied;
} StringToIntMapEntry;

typedef struct {
    int key;
    string *value;
    int isOccupied;
} IntToStringMapEntry;

typedef struct {
    int key;
    int value;
    int isOccupied;
} IntMapEntry;

typedef struct {
    size_t size;
    size_t count;
    float loadFactor;
    StringToIntMapEntry *entries;
} StringToIntMap;

typedef struct {
    size_t size;
    size_t count;
    float loadFactor;
    IntToStringMapEntry *entries;
} IntToStringMap;

typedef struct {
    size_t size;
    size_t count;
    float loadFactor;
    IntMapEntry *entries;
} IntMap;

StringToIntMap *newStringToIntMap(size_t size, float loadFactor);

IntToStringMap *newIntToStringMap(size_t size, float loadFactor);

IntMap *newIntMap(size_t size, float loadFactor);

void freeStringToIntMap(StringToIntMap *map);

void freeIntToStringMap(IntToStringMap *map);

void freeIntMap(IntMap *map);

size_t quadraticProbe(size_t hash, size_t attempt, size_t size);

size_t hashFunction(int key, size_t size);

int stringToInt(string *str);

void resizeStringToIntMap(StringToIntMap *map);

void StringToIntMapInsert(StringToIntMap *map, string *key, int value);

int StringToIntMapGet(StringToIntMap *map, string *key);

void StringToIntMapDelete(StringToIntMap *map, string *key);

void resizeIntToStringMap(IntToStringMap *map);

void IntToStringMapInsert(IntToStringMap *map, int key, string* value);

string *IntToStringMapGet(IntToStringMap *map, int key);

void IntToStringMapDelete(IntToStringMap *map, int key);

void resizeIntMap(IntMap *map);

void intMapInsert(IntMap *map, int key, int value);

int intMapGet(IntMap *map, int key);

void intMapDelete(IntMap *map, int key);

int StringToIntMapGetRaw(StringToIntMap *map, char *key);

string* serializeMap(StringToIntMap *map, char separator);

StringToIntMap* deserializeMap(FILE* file, char separator);

size_t readSizeT(FILE* file);

#endif