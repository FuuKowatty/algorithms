#include "c-map.h"

size_t quadraticProbe(size_t hash, size_t attempt, size_t size) {
    return (hash + attempt * attempt) % size;
}

size_t hashFunction(int key, size_t size) {
    return key % size;
}

int charToInt(char* str) {
    int result = 0;
    while (*str != '\0') {
        result = result * 31 + (int) *str++;
    }
    return result;
}

int stringToInt(string *str) {
    int result = 0;
    for (size_t i = 0; i < str->length; i++) {
        result = result * 31 + (int)str->str[i];
    }
    return result;
}

StringToIntMap *newStringToIntMap(size_t size, float loadFactor) {
    StringToIntMap *map = (StringToIntMap*) malloc(sizeof(StringToIntMap));
    if (map == NULL) {
        return NULL;
    }

    map->size = size;
    map->loadFactor = loadFactor;
    map->count = 0;
    map->entries = (StringToIntMapEntry*) malloc(map->size * sizeof(StringToIntMapEntry));
    if (map->entries == NULL) {
        free(map);
        return NULL;
    }

    for (size_t i = 0; i < map->size; i++) {
        map->entries[i].isOccupied = 0;
    }

    return map;
}

IntToStringMap *newIntToStringMap(size_t size, float loadFactor) {
    IntToStringMap *map = (IntToStringMap*) malloc(sizeof(IntToStringMap));
    if (map == NULL) {
        return NULL;
    }

    map->size = size;
    map->loadFactor = loadFactor;
    map->count = 0;
    map->entries = (IntToStringMapEntry*) malloc(map->size * sizeof(IntToStringMapEntry));
    if (map->entries == NULL) {
        free(map);
        return NULL;
    }

    for (size_t i = 0; i < map->size; i++) {
        map->entries[i].isOccupied = 0;
    }

    return map;
}

IntMap *newIntMap(size_t size, float loadFactor) {
    IntMap *map = (IntMap*) malloc(sizeof(IntMap));
    if (map == NULL) {
        return NULL;
    }

    map->size = size;
    map->loadFactor = loadFactor;
    map->count = 0;
    map->entries = (IntMapEntry*) malloc(map->size * sizeof(IntMapEntry));
    if (map->entries == NULL) {
        free(map);
        return NULL;
    }

    for (size_t i = 0; i < map->size; i++) {
        map->entries[i].isOccupied = 0;
    }

    return map;
}

void freeStringToIntMap(StringToIntMap *map) {
    free(map->entries);
    free(map);
}

void freeIntToStringMap(IntToStringMap *map) {
    free(map->entries);
    free(map);
}

void freeIntMap(IntMap *map) {
    free(map->entries);
    free(map);
}

void resizeStringToIntMap(StringToIntMap *map) {
    size_t oldSize = map->size;
    size_t newSize = oldSize * 2;
    StringToIntMapEntry *newEntries = (StringToIntMapEntry*) malloc(newSize * sizeof(StringToIntMapEntry));
    if (newEntries == NULL) {
        return;
    }
    for (size_t i = 0; i < newSize; i++) {
        newEntries[i].isOccupied = 0;
    }
    StringToIntMapEntry *entries = map->entries;
    map->entries = newEntries;
    map->size = newSize;
    map->count = 0;
    for (size_t i = 0; i < oldSize; i++) {
        if (entries[i].isOccupied) {
            StringToIntMapInsert(map, entries[i].key, entries[i].value);
        }
    }
    free(entries);
}

void resizeIntToStringMap(IntToStringMap *map) {
    size_t oldSize = map->size;
    size_t newSize = oldSize * 2;
    IntToStringMapEntry *newEntries = (IntToStringMapEntry*) malloc(newSize * sizeof(IntToStringMapEntry));
    if (newEntries == NULL) {
        return;
    }
    for (size_t i = 0; i < newSize; i++) {
        newEntries[i].isOccupied = 0;
    }
    IntToStringMapEntry *entries = map->entries;
    map->entries = newEntries;
    map->size = newSize;
    map->count = 0;
    for (size_t i = 0; i < oldSize; i++) {
        if (entries[i].isOccupied) {
            IntToStringMapInsert(map, entries[i].key, entries[i].value);
        }
    }
    free(entries);
}

void resizeIntMap(IntMap *map) {
    size_t newSize = map->size * 2;
    IntMapEntry *newEntries = (IntMapEntry*) malloc(newSize * sizeof(IntMapEntry));
    if (newEntries == NULL) {
        return;
    }

    for (size_t i = 0; i < newSize; i++) {
        newEntries->isOccupied = 0;
    }

    for (size_t i = 0; i < map->size; i++) {
        if (map->entries[i].isOccupied) {
            int key = map->entries[i].key;
            int value = map->entries[i].value;
            size_t hash = hashFunction(key, newSize);
            size_t attempt = 0;
            size_t index = quadraticProbe(hash, attempt, newSize);
            while (newEntries[index].isOccupied) {
                attempt += 1;
            }
            newEntries[index].key = key;
            newEntries[index].value = value;
            newEntries[index].isOccupied = 1;
        }
    }

    free(map->entries);
    map->entries = newEntries;
    map->size = newSize;
}

void StringToIntMapInsert(StringToIntMap *map, string *key, int value) {
    if ((float) map->count / map->size >= map->loadFactor) {
        resizeStringToIntMap(map);
    }

    size_t hash = hashFunction(stringToInt(key), map->size);
    size_t attempt = 0;
    
    size_t index = quadraticProbe(hash, attempt, map->size);
    while (map->entries[index].isOccupied) {
        attempt++;
        index = quadraticProbe(hash, attempt, map->size);
    }
    
    map->entries[index].key = key;
    map->entries[index].value = value;
    map->entries[index].isOccupied = 1;
    map->count++;
}

void IntToStringMapInsert(IntToStringMap *map, int key, string* value) {
    if ((float) map->count / map->size >= map->loadFactor) {
        resizeIntToStringMap(map);
    }

    size_t hash = hashFunction(key, map->size);
    size_t attempt = 0;
    
    size_t index = quadraticProbe(hash, attempt, map->size);
    while (map->entries[index].isOccupied) {
        attempt++;
        index = quadraticProbe(hash, attempt, map->size);
    }
    
    map->entries[index].key = key;
    map->entries[index].value = value;
    map->entries[index].isOccupied = 1;
    map->count++;
}

void intMapInsert(IntMap *map, int key, int value) {
    if ((float) map->count / map->size >= map->loadFactor) {
        resizeIntMap(map);
    }

    size_t hash = hashFunction(key, map->size);
    size_t attempt = 0;

    size_t index = quadraticProbe(hash, attempt, map->size);
    while (map->entries[index].isOccupied) {
        attempt++;
        index = quadraticProbe(hash, attempt, map->size);
    }

    map->entries[index].key = key;
    map->entries[index].value = value;
    map->entries[index].isOccupied = 1;
    map->count++;
}


int StringToIntMapGetRaw(StringToIntMap *map, char *key) {
    size_t hash = hashFunction(charToInt(key), map->size);
    size_t attempt = 0;

    size_t index = quadraticProbe(hash, attempt, map->size);
    while (map->entries[index].isOccupied) {
        if (compareChar(map->entries[index].key, key) == 1) {
            return map->entries[index].value;
        }
        attempt++;
        index = quadraticProbe(hash, attempt, map->size);
    }

    return -1;
}

int StringToIntMapGet(StringToIntMap *map, string *key) {
    size_t hash = hashFunction(stringToInt(key), map->size);
    size_t attempt = 0;

    size_t index = quadraticProbe(hash, attempt, map->size);
    while (map->entries[index].isOccupied) {
        if (compare(map->entries[index].key, key) == 1) {
            return map->entries[index].value;
        }
        attempt++;
        index = quadraticProbe(hash, attempt, map->size);
    }

    return -1;
}

string *IntToStringMapGet(IntToStringMap *map, int key) {
    size_t hash = hashFunction(key, map->size);
    size_t attempt = 0;

    size_t index = quadraticProbe(hash, attempt, map->size);
    while (map->entries[index].isOccupied) {
        if (map->entries[index].key == key) {
            return map->entries[index].value;
        }
        attempt++;
        index = quadraticProbe(hash, attempt, map->size);
    }

    return NULL;
}

int intMapGet(IntMap *map, int key) {
    size_t hash = hashFunction(key, map->size);
    size_t attempt = 0;

    size_t index = quadraticProbe(hash, attempt, map->size);
    while (map->entries[index].isOccupied) {
        if (map->entries[index].key == key) {
            return map->entries[index].value;
        }
        attempt++;
        index = quadraticProbe(hash, attempt, map->size);
    }

    return -1;
}

void StringToIntMapDelete(StringToIntMap *map, string *key) {
    size_t hash = hashFunction(stringToInt(key), map->size);
    size_t attempt = 0;

    size_t index = quadraticProbe(hash, attempt, map->size);
    while (map->entries[index].isOccupied) {
        if (compare(map->entries[index].key, key) == 1) {
            map->entries[index].isOccupied = 0;
            map->count--;
            return;
        }
        attempt++;
        index = quadraticProbe(hash, attempt, map->size);
    }
} 

void IntToStringMapDelete(IntToStringMap *map, int key) {
    size_t hash = hashFunction(key, map->size);
    size_t attempt = 0;

    size_t index = quadraticProbe(hash, attempt, map->size);
    while (map->entries[index].isOccupied) {
        if (map->entries[index].key == key) {
            map->entries[index].isOccupied = 0;
            map->count--;
            return;
        }
        attempt++;
        index = quadraticProbe(hash, attempt, map->size);
    }
}

void intMapDelete(IntMap *map, int key) {
    size_t hash = hashFunction(key, map->size);
    size_t attempt = 0;

    size_t index = quadraticProbe(hash, attempt, map->size);
    while (map->entries[index].isOccupied) {
        if (map->entries[index].key == key) {
            map->entries[index].isOccupied = 0;
            map->count--;
            return;
        }
        attempt++;
        index = quadraticProbe(hash, attempt, map->size);
    }
}

size_t countHowMuchAllocate(StringToIntMap *map) {
    size_t size = sizeof(char);
    for(size_t i = 0; i < map->size; i++) {
        if (map->entries[i].isOccupied) {
            size += map->entries[i].key->length + 1 + 1; 
        }
    }
    return size;
}

string* serializeMap(StringToIntMap *map, char separator) {
    string* entries = newEmptyStringWithFixedLength(countHowMuchAllocate(map));
    size_t entriesSize = 0;
    entries->str[entriesSize++] = map->count;
    for(size_t i = 0; i < map->size; i++) {
        if (map->entries[i].isOccupied) {
            entries->str[entriesSize++] = map->entries[i].value;
            for(size_t j = 0; j < map->entries[i].key->length; j++) {
                entries->str[entriesSize++] = map->entries[i].key->str[j];
            }
            entries->str[entriesSize++] = separator;
        }
    }
    entries->str[entriesSize] = '\0';
    return entries;
}

size_t readSizeT(FILE* file) {
    size_t el;
    fread(&el, sizeof(size_t), 1, file);
    return el;
}

StringToIntMap* deserializeMap(FILE* file, char separator) {
    size_t mapByteLength = readSizeT(file);
    char* mapContent = malloc(mapByteLength+1);
    fread(mapContent, sizeof(char), mapByteLength+1, file);
    mapContent[mapByteLength+1] = '\0';
    size_t mapCount = (size_t) mapContent[0];
    StringToIntMap* map = newStringToIntMap(mapCount, 0.5);
    size_t mapContentIndex = 1;
    for (size_t i = 1; i < mapByteLength; i++) {
        if (mapContent[i] == separator) {
            char value = mapContent[mapContentIndex];
            string* key = newEmptyStringWithFixedLength(i - mapContentIndex - sizeof(char));
            mapContentIndex++;
            size_t keyIndex = 0;
            while (mapContentIndex < i) {
                key->str[keyIndex++] = mapContent[mapContentIndex];
                mapContentIndex++; 
            }
            key->str[keyIndex] = '\0';
            mapContentIndex++;
            StringToIntMapInsert(map, key, value);
        }
    }
    return map;
} 