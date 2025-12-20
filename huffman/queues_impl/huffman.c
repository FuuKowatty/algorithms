#include <stdio.h>
#include <stdlib.h>
#include "c-map.h"

typedef struct Node
{
    struct Node** childrens;
    string* codeName;
    char letter;
    int count;
} Node;

typedef struct NodeWrapper
{
    Node* nodes;
    int size;
    int currentSize;
} NodeWrapper;

typedef struct NodeQueue 
{
    Node** nodes;
    int head;
    int tail;
    int size;
    int currentSize;
} NodeQueue;

void initChilds(Node* nodes, int size) {
    for (int i = 0; i < size; i++) {
        nodes[i].childrens = NULL;
    }
}

void addLetter(NodeWrapper* wrapper, char letter) {
    if (wrapper->size == wrapper->currentSize) {
        wrapper->nodes = (Node*) realloc(wrapper->nodes, sizeof(Node) * wrapper->size * 2);
    }
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

int compareNodes(const void *a, const void *b) {
    const Node* arg1 = (const Node*)a;
    const Node* arg2 = (const Node*)b;
    return (arg1->count - arg2->count);
}

const int NODE_INITIAL_SIZE = 10;
const char EMPTY_LETTER = '[';

Node** initItems(int size, int sizeToRecopy, Node* nodes) {
    Node** items = (Node**) malloc(size * sizeof(Node*));
    for (int i = 0; i < sizeToRecopy; i++) {
        items[i] = &nodes[i];
        nodes[i].childrens = NULL;
        nodes[i].codeName = NULL;
    }
    return items;
}

NodeQueue* initQueue(int size, int currentNodesSize, Node* nodes) {
    int startSize = size * 2;
    NodeQueue* q = (NodeQueue*) malloc(sizeof(NodeQueue));
    q->nodes = initItems(startSize, currentNodesSize, nodes);
    q->head = 0;
    q->tail = currentNodesSize;
    q->currentSize = currentNodesSize;
    q->size = startSize;
    return q;
}

typedef struct Tuple {
    Node* left;
    Node* right;
} Tuple;

Node* popMinimalFreqElement(NodeQueue* q1, NodeQueue* q2) {
    if (q2->currentSize == 0) {
        q1->currentSize--;
        return q1->nodes[q1->head++];
    }
    if (q1->currentSize == 0) {
        q2->currentSize--;
        return q2->nodes[q2->head++];
    } 
    Node* n1 = q1->nodes[q1->head];
    Node* n2 = q2->nodes[q2->head];
    if (n1->count < n2->count) {
        q1->currentSize--;
        q1->head++;
        return n1;
    }
    q2->currentSize--;
    q2->head++;
    return n2;
}

Node* popQueue(NodeQueue* q) {
    if (q->size == 0) {
        return NULL;
    }
    q->currentSize--;
    return q->nodes[q->head++];
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

void pushQueue(NodeQueue* q, Node* e) {
    if (q->tail == q->size) {
        Node** items = (Node**) malloc(q->size * 2 * sizeof(Node*));
        for (int i = 0; i < q->size; i++) {
            items[i] = q->nodes[i];
        }
        free(q->nodes);
        q->nodes = items;
        q->size *= 2;
    }
    q->nodes[q->tail++] = e;
    q->currentSize++;
}

void processCodes(NodeQueue* q, IntToStringMap* dictionaryCodes) {
    // printf("%c\n", q->nodes[q->head]->childrens[1]->childrens[0]->childrens[1]->childrens[1]->childrens[0]->letter); d
    // printf("%c\n", q->nodes[q->head]->childrens[1]->childrens[0]->childrens[1]->childrens[1]->childrens[1]->childrens[0]->letter); t
    q->nodes[q->head]->codeName=NULL;
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
    Node* emptyItems = (Node*) malloc(NODE_INITIAL_SIZE * sizeof(Node));
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
    //1
    // uzywam counts long counts[256] = {0};
    // licze liczbe wystapien counts[(unsigned char)buffer[i]]++
    // potem zamieniam na LinkedList

    //2 Dynamiczna lista ale do ostatniej fazy tworze 2 kolejki (tylko nwm czy wczesniejsze etapy da sie lepiej)
    // 1. kolejka z posortowanymi wartościami
    // 2. kolejka z nowymi node'ami. Wtedy porównuje head 1 i head 2 kolejki i biore minValue, tak samo minValue2 biore



    char* originallText = "siema tu siema i robi sie dobra siema";
    IntMap* letterToIndex = newIntMap(99, 0.75);
    Node* nodes = (Node*) malloc(NODE_INITIAL_SIZE * sizeof(Node));
    NodeWrapper* wrapper = (NodeWrapper*) malloc(sizeof(NodeWrapper));
    wrapper->size=NODE_INITIAL_SIZE;
    wrapper->currentSize = 0;
    wrapper->nodes = nodes;
    for (int i=0; originallText[i] != '\0'; i++) {
        int key = (int) originallText[i];
        int letterIndex = intMapGet(letterToIndex, key);
        if (letterIndex == -1) {
            char letter = originallText[i];
            addLetter(wrapper, letter);
            intMapInsert(letterToIndex, key, wrapper->currentSize-1);
        } else {
            incrementLetter(wrapper, letterIndex);
        }
    }
    qsort(wrapper->nodes, wrapper->currentSize, sizeof(Node), compareNodes);

    // do tej operacji LInkedList jest lepsze ale nie będę mógł sortować efektownie i mapa będzie usseles więc:
    // 1. LinkedLista/kolejka i jakoś wydajnie dodawać elementy (co increment musiałbym szukac całą liste a będzie ich dużo więc słabe chyba)
    // 2. Zwykła lista dynamiczna ale sortowanie co stworzenie nowego node'a długie ?? 
    // jak efektywnie dodawać w środku nowy element dla dynamicznej listy.

    IntToStringMap* codeDictionary = getCodesDictionary(wrapper);
    string* code = getCode(originallText, codeDictionary);

    char* text = decode(codeDictionary, code->str);
    printf("After decoding %s", text);
    freeIntToStringMap(codeDictionary);
    free(wrapper);

};