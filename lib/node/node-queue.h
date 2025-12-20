#include "node.h"

#pragma once

typedef struct NodeQueue 
{
    Node** nodes;
    size_t head;
    size_t tail;
    size_t size;
    size_t currentSize;
} NodeQueue;

NodeQueue* initQueue(size_t size, size_t currentNodesSize, Node* nodes);
Node* popQueue(NodeQueue* q);
Node* peekQueue(NodeQueue* q);
void pushQueue(NodeQueue* q, Node* e);