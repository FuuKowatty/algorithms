#include "c-string.h"

#pragma once

typedef struct Node
{
    struct Node** children;
    string* codeName;
    unsigned char letter;
    size_t count;
} Node;

Node* reallocNodes(Node* nodes, size_t newSize);
Node* initNodes(size_t);
void clearNode(Node* nodes);