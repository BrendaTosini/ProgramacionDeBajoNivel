#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdbool.h>
#include "node.h"

typedef struct LinkedList {
    Node *head;
    Node *tail;
    int size; 
} LinkedList;

LinkedList* newLinkedList(void);

void append(LinkedList *list, void *value);

void prepend(LinkedList *list, void *value);

void insertAt(LinkedList *list, void *value, int position);

void removeNode(LinkedList *list, void *value, bool (*equals)(void*, void*));

void removeAt(LinkedList *list, int position);

void printList(LinkedList *list, void (*printValue)(void*));

int search(LinkedList *list, void *value, bool (*equals)(void*, void*));

void* get(LinkedList *list, int position);

Node* getNode(LinkedList *list, int position);

void updateNodeValue(LinkedList *list, void *targetValue, void *newValue, bool (*equals)(void*, void*));

int size(LinkedList *list);

void** toArray(LinkedList *list);

#endif
