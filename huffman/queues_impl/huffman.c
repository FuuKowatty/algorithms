#include <stdio.h>
#include <stdlib.h>
#include "c-map.h"
#include "node.h"
#include "node-queue.h"


// CONSTS
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
    wrapper->currentSize += 1;
    Node* node = &wrapper->nodes[wrapper->currentSize-1];
    node->count = 1;
    node->letter = letter;
    node->codeName = NULL;
    node->childrens = NULL;
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
        newCode = codeName;
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
    Node** childrens = (Node**) malloc(2 * sizeof(Node*));
    updateCode(e1, "0");
    updateCode(e2, "1");
    childrens[0] = e1;
    childrens[1] = e2;
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->count=e1->count + e2->count;
    newNode->letter = EMPTY_LETTER;
    newNode->childrens = childrens;
    newNode->codeName = NULL;
    return newNode;
}

void processCodes(NodeQueue* q, IntToStringMap* dictionaryCodes) {
    peekQueue(q)->codeName = NULL;
    while (q->currentSize != 0)
    {   
        Node* e = popQueue(q);
        if (e->codeName == NULL) { 
            pushQueue(q, e->childrens[0]);
            pushQueue(q, e->childrens[1]);
            continue;
        }
        string* codeName = e->codeName;
        if (e->childrens != NULL) {
            concatCode(e->childrens[0], codeName);
            concatCode(e->childrens[1], codeName);
            pushQueue(q, e->childrens[0]);
            pushQueue(q, e->childrens[1]);
            continue;
        }
        IntToStringMapInsert(dictionaryCodes, e->letter, e->codeName); 
    }
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
    return dictionaryCodes;
}

string* getCode(char* text, IntToStringMap* codeDictionary) {
    string* code = newString("");
    for (int i = 0; text[i] != '\0'; i++) {
        string* letterCode = IntToStringMapGet(codeDictionary, (int) text[i]);
        string* temp = concat(code, letterCode);
        free(code->str);
        free(code);
        code = temp;
    }
    return code;
}

StringToIntMap* invertMap(IntToStringMap* dict) {
    StringToIntMap* newMap = newStringToIntMap(dict->size, 1);
    for (int i = 0; i < dict->size; i++) {
        IntToStringMapEntry entry = dict->entries[i];
        if (entry.isOccupied) {
            StringToIntMapInsert(newMap, entry.value, entry.key);
        }
    }
    return newMap;
}

char* decode(IntToStringMap* codeDictionary, char* code) {
    string* decoded = newString("");
    StringToIntMap* invertedMap = invertMap(codeDictionary);
    string* potentialCode = newString("");
    for (int i = 0; code[i] != '\0'; i++) {
        char temp_buf[2] = {code[i], '\0'};
        string* temp1 = newString(temp_buf);
        string* temp2 = potentialCode;
        potentialCode = concat(temp2, temp1);
        free(temp1);
        free(temp2);        
        int potentialLetter = StringToIntMapGet(invertedMap, potentialCode);
        if (potentialLetter != -1) {
            string* temp4 = decoded;
            char temp_buff2[2] = {(char) potentialLetter, '\0'};
            string* temp5 = newString(temp_buff2);
            decoded = concat(temp4, temp5);
            free(temp4);
            free(temp5);
            string* temp3 = potentialCode;
            potentialCode = newString("");
            free(temp3);
        }
    }
    free(potentialCode);
    freeStringToIntMap(invertedMap);
    return decoded->str;
}

int main() {
    char* originallText = "siema tu siema i robi sie dobra siema";
    NodeWrapper* wrapper = countLetters(originallText);

    qsort(wrapper->nodes, wrapper->currentSize, sizeof(Node), compareNodes);

    IntToStringMap* codeDictionary = getCodesDictionary(wrapper);
    string* code = getCode(originallText, codeDictionary);

    char* text = decode(codeDictionary, code->str);
    printf("After decoding %s", text);
    freeIntToStringMap(codeDictionary);
    free(wrapper);
};