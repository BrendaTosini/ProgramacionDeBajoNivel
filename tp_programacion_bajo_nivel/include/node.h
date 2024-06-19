#ifndef NODE_H
#define NODE_H

typedef struct Node {
    void *value;           
    struct Node *next;
} Node;

Node* newNode(void *value);

#endif
