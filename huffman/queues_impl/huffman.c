#include <stdio.h>
#include <stdlib.h>
#include "c-map.h"
#include <string.h>
#include "node.h"
#include "node-queue.h"


// CONSTS
#define POTENTIAL_CODE_MAX 31
const char* FILENAME_TO_COMPRESS = "file.txt";
const char* COMPRESSED_FILENAME = "newFIle";
const size_t NODE_INITIAL_SIZE = 10;
const char EMPTY_LETTER = '[';

int compareNodes(const void *a, const void *b) {
    const Node* arg1 = (const Node*)a;
    const Node* arg2 = (const Node*)b;
    return (arg1->count - arg2->count);
}


// WRAPPER
typedef struct NodeWrapper
{
    Node* nodes;
    size_t size;
    size_t currentSize;
} NodeWrapper;

NodeWrapper* initWrapper(Node* nodes) {
    NodeWrapper* wrapper = (NodeWrapper*) malloc(sizeof(NodeWrapper));
    wrapper->size=NODE_INITIAL_SIZE;
    wrapper->currentSize = 0;
    wrapper->nodes = nodes;
    return wrapper;
}

void ensureWrapperSize(NodeWrapper* wrapper) {
    if (wrapper->size == wrapper->currentSize) {
        size_t newSize = wrapper->size * 2;
        Node* temp = reallocNodes(wrapper->nodes, newSize);
        if (temp != NULL) {
            wrapper->nodes = temp;
            wrapper->size = newSize;
        }
    }
}

void addLetter(NodeWrapper* wrapper, char letter) {
    ensureWrapperSize(wrapper);
    Node* node = &wrapper->nodes[wrapper->currentSize];
    wrapper->currentSize += 1;
    node->count = 1;
    node->letter = letter;
    node->codeName = NULL;
    node->children = NULL;
}

void incrementLetter(NodeWrapper* wrapper, int letterIndex) {
    Node* node = &wrapper->nodes[letterIndex];
    node->count += 1;
}

NodeWrapper* countLetters(char* text) {
    IntMap* letterToIndex = newIntMap(99, 0.75);
    Node* nodes = initNodes(NODE_INITIAL_SIZE);
    NodeWrapper* wrapper = initWrapper(nodes);
    for (int i=0; text[i] != '\0'; i++) {
        int key = (int) text[i];
        int letterIndex = intMapGet(letterToIndex, key);
        if (letterIndex == -1) {
            char letter = text[i];
            addLetter(wrapper, letter);
            intMapInsert(letterToIndex, key, wrapper->currentSize-1);
        } else {
            incrementLetter(wrapper, letterIndex);
        }
    }
    freeIntMap(letterToIndex);
    return wrapper;
}
 

// HUFFMAN
Node* popMinimalFreqElement(NodeQueue* q1, NodeQueue* q2) {
    if (q2->currentSize == 0) {
        return popQueue(q1);
    }
    if (q1->currentSize == 0) {
        return popQueue(q2);
    } 
    Node* n1 = peekQueue(q1);
    Node* n2 = peekQueue(q2);
    if (n1->count < n2->count) {
        return popQueue(q1);
    }
    return popQueue(q2);
}

void concatCode(Node* e, string* codeName) {
    string* previousCode = e->codeName;
    string* newCode;
    if (previousCode) {
        newCode = concat(codeName, previousCode);
    } else {
        newCode = copyString(codeName);
    }
    free(previousCode);
    e->codeName = newCode;
}

void updateCode(Node* e, char* s) {
    string* c = e->codeName;
    e->codeName = newString(s);
    if (c) {
        free(c->str);
        free(c);
    }
}

Node* createNode(Node* e1, Node* e2) {
    Node** children = (Node**) malloc(2 * sizeof(Node*));
    updateCode(e1, "0");
    updateCode(e2, "1");
    children[0] = e1;
    children[1] = e2;
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->count=e1->count + e2->count;
    newNode->letter = EMPTY_LETTER;
    newNode->children = children;
    newNode->codeName = NULL;
    return newNode;
}

void processCodes(NodeQueue* q, IntToStringMap* dictionaryCodes) {
    peekQueue(q)->codeName = NULL;
    while (q->currentSize != 0)
    {   
        Node* e = popQueue(q);
        if (e->codeName == NULL) { 
            pushQueue(q, e->children[0]);
            pushQueue(q, e->children[1]);
            continue;
        }
        string* codeName = e->codeName;
        if (e->children != NULL) {
            concatCode(e->children[0], codeName);
            concatCode(e->children[1], codeName);
            pushQueue(q, e->children[0]);
            pushQueue(q, e->children[1]);
            continue;
        }
        IntToStringMapInsert(dictionaryCodes, e->letter, e->codeName); 
    }
}

// cant clean random index of wrapper->nodes which are leafs
void clearHuffmanNode(Node* node) {
    if (node->children != NULL) {
        clearNode(node->children[0]);
        clearNode(node->children[1]);
        free(node->children);
        freeString(node->codeName);
        free(node);
    }
}

void clearHuffmanQueue(NodeQueue* q) {
    for (size_t i = 0; i < q->currentSize; i++) {
        clearHuffmanNode(q->nodes[i]);
    }   
    free(q->nodes);
    free(q);
}

void freeWrapper(NodeWrapper* wrapper) {
    for (size_t i = 0; i < wrapper->currentSize; i++) {
        Node* node = (&wrapper->nodes[i]);
        if (node->codeName) {
            freeString(node->codeName);
        }
    }
    free(wrapper->nodes);
    free(wrapper);
}

IntToStringMap* getCodesDictionary(NodeWrapper* wrapper) {
    NodeQueue* q1 = initQueue(wrapper->size, wrapper->currentSize, wrapper->nodes);
    Node* emptyItems = initNodes(NODE_INITIAL_SIZE);
    NodeQueue* q2 = initQueue(NODE_INITIAL_SIZE, NODE_INITIAL_SIZE, emptyItems);
    q2->tail = 0;
    q2->currentSize = 0;

    while (q1->currentSize != 0 || q2->currentSize != 1)
    {
        if (q1->currentSize + q2->currentSize < 2) {
            break;
        }
        Node* e1 = popMinimalFreqElement(q1, q2);
        Node* e2 = popMinimalFreqElement(q1, q2);
        Node* e3 = createNode(e1, e2);
        pushQueue(q2, e3);
    }
    IntToStringMap* dictionaryCodes = newIntToStringMap(99, 0.75);
    processCodes(q2, dictionaryCodes);
    free(q1);
    clearHuffmanQueue(q2);
    return dictionaryCodes;
}

string* getCode(string* text, IntToStringMap* codeDictionary) {
    string* code = newString("");
    for (size_t i = 0; i < text->length; i++) {
        string* letterCode = IntToStringMapGet(codeDictionary, (int) text->str[i]);
        string* temp = concat(code, letterCode);
        freeString(code);
        code = temp;
    }
    return code;
}

StringToIntMap* invertMap(IntToStringMap* dict) {
    StringToIntMap* newMap = newStringToIntMap(dict->size, 1);
    for (size_t i = 0; i < dict->size; i++) {
        IntToStringMapEntry entry = dict->entries[i];
        if (entry.isOccupied) {
            StringToIntMapInsert(newMap, entry.value, entry.key);
        }
    }
    return newMap;
}

string* cleanString(string* code) {
    free(code);
    return newString("");
}

char* decode(StringToIntMap* codeDictionary, char* code) {
    size_t codeLen = strlen(code);
    char* decoded = malloc(codeLen + 1);
    size_t decodedIdx = 0;
    char potentialCode[POTENTIAL_CODE_MAX];
    size_t potentialLen = 0;
    for (size_t i = 0; i < codeLen; i++) {
        potentialCode[potentialLen] = code[i];
        potentialCode[potentialLen+1] = '\0';
        potentialLen++;
        int potentiallyUncoded = StringToIntMapGetRaw(codeDictionary, potentialCode);
        if (potentiallyUncoded != -1) {
            decoded[decodedIdx] = potentiallyUncoded;
            decodedIdx++;
            potentialCode[0] = '\0';
            potentialLen = 0;
        }
        if (potentialLen >= POTENTIAL_CODE_MAX) {
            printf("ERROR: EXCEED POTENTIAL_CODE_MAX");
            return decoded;
        }
    }
    decoded[decodedIdx] = '\0';
    return decoded;
}

size_t readFile(const char* filename, char* bufor) {
    FILE* file = fopen(filename, "rb");
    if (file != NULL) {
        size_t bufor_idx = 0;
        do
        {
            char c = fgetc(file);
            if (feof(file)) break;
            bufor[bufor_idx++] = c;
            // if exceed buffor index
        } while(1);
        fclose(file);
        return bufor_idx;
    }
    return 0;
}

size_t roundUp(size_t length) {
    return (length + 7) / 8;
}

void writeFile(const char* filename, string* content) {
    FILE* newFile = fopen(filename, "wb");
    if (newFile != NULL) {
        size_t codeLength = content->length; 
        size_t roundedLength = roundUp(codeLength);
        char* codeInBytes = calloc(roundedLength, 1);
        for (size_t i = 0; i < codeLength; i++) {
            if (content->str[i] == '1') {
                codeInBytes[i / 8] |= 1 << (7 - (i % 8));
            }
        }
        printf("compressed bytes: %zu\n", roundUp(content->length));
        fwrite(codeInBytes, sizeof(char), roundedLength, newFile);
        fclose(newFile);
    }
}

char* readByBits(char* compressedFileContent, size_t contentLength) {
    char* decompressed = (char*) malloc(contentLength * 8 * sizeof(char) + 1);
    printf("rezerwuje %zu\n", contentLength * sizeof(char));
    size_t decompressedSize = 0;
    for (size_t i = 0; i < contentLength;  i++) {
        for (size_t j = 0; j < 8; j++) {
           decompressed[decompressedSize++] = '0' + ((compressedFileContent[i] >> (7-j)) & 1); 
        }
    }
    decompressed[decompressedSize] = '\0';
    return decompressed;
}

int main() {
    // STEP 1 compression
    char bufor[4096];
    readFile(FILENAME_TO_COMPRESS, bufor);
    string* fileContent = newString(bufor);
    printf("oryginal length: %zu\n", fileContent->length);
    if (fileContent == NULL) {
        return 0;
    }
    NodeWrapper* wrapper = countLetters(fileContent->str);

    qsort(wrapper->nodes, wrapper->currentSize, sizeof(Node), compareNodes);

    IntToStringMap* codeDictionary = getCodesDictionary(wrapper);
    string* code = getCode(fileContent, codeDictionary);
    printf("code->length %zu\n", code->length);
    writeFile(COMPRESSED_FILENAME, code);
    // STEP 2 DECOMPRESSION
    char compressedCode[32768];
    size_t compressedLength = readFile(COMPRESSED_FILENAME, compressedCode);
    printf("compressedLength %zu\n", compressedLength);
    char* decompressedCode = readByBits(compressedCode, compressedLength);
    decompressedCode[code->length] = '\0';
    printf("After decompression");
    StringToIntMap* invertedMap = invertMap(codeDictionary);
    char* text = decode(invertedMap, decompressedCode);
    printf("after decoding: %s\n", text);

    freeStringToIntMap(invertedMap);
    freeString(code);
    free(decompressedCode);
    freeIntToStringMap(codeDictionary);
    freeWrapper(wrapper);
    free(text);
};

// notatki z tego co mam + jak czyscimy pointery (ze wskazujemy zawsze na 1 adres bo robi -16 zeby odczytac metadane)
// implementacja z LinkedList