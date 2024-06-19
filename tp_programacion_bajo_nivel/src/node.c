#include <stdlib.h>
#include "node.h"

Node* newNode(void *value) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->value = value;
    node->next = NULL;
    return node;
}
