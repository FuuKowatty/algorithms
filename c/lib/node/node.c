#include <stdlib.h>
#include "node.h"


Node* reallocNodes(Node* nodes, size_t newSize) {
    Node* temp = (Node*) realloc(nodes, sizeof(Node) * newSize);
    if (temp == NULL) {
        return NULL;
    }
    return temp;
}

Node* initNodes(size_t size) {
    Node* nodes = (Node*) malloc(size * sizeof(Node));
    if (nodes == NULL) {
        return NULL;
    }
    return nodes;
}

void clearNode(Node* node) {
    if (node->children != NULL) {
        clearNode(node->children[0]);
        clearNode(node->children[1]);
        free(node->children);
    }
    freeString(node->codeName);
    free(node);
}