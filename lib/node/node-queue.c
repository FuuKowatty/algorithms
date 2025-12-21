#include <stdlib.h>

#include "node-queue.h"

Node* peekQueue(NodeQueue* q) {
    if (q->currentSize == 0) {
        return NULL;
    }
    return q->nodes[q->head];
}

Node* popQueue(NodeQueue* q) {
    if (q->currentSize == 0) {
        return NULL;
    }
    q->currentSize--;
    return q->nodes[q->head++];
}

Node** initItems(size_t size, size_t sizeToRecopy, Node* nodes) {
    Node** items = (Node**) malloc(size * sizeof(Node*));
    for (int i = 0; i < sizeToRecopy; i++) {
        items[i] = &nodes[i];
        nodes[i].children = NULL;
        nodes[i].codeName = NULL;
    }
    return items;
}

NodeQueue* initQueue(size_t size, size_t currentNodesSize, Node* nodes) {
    size_t startSize = size * 2;
    NodeQueue* q = (NodeQueue*) malloc(sizeof(NodeQueue));
    q->nodes = initItems(startSize, currentNodesSize, nodes);
    q->head = 0;
    q->tail = currentNodesSize;
    q->currentSize = currentNodesSize;
    q->size = startSize;
    return q;
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

void freeQueue(NodeQueue* q) {
    for (int i = 0; i < q->currentSize; i++) { // size?
        clearNode(q->nodes[i]);
    }   
    free(q->nodes);
    free(q);
}