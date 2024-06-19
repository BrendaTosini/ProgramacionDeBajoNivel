#include <stdlib.h>
#include <stdio.h>
#include "linkedlist.h"

LinkedList* newLinkedList(void) {
    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void append(LinkedList *list, void *value) {
    Node* node = newNode(value);
    list->size++;
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
}

void prepend(LinkedList *list, void *value) {
    Node* node = newNode(value);
    list->size++;
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        node->next = list->head;
        list->head = node;
    }
}

void insertAt(LinkedList *list, void *value, int position) {
    if (position < 0) return;
    if (position == 0) {
        prepend(list, value);
        return;
    }
    Node* current = list->head;
    while (current != NULL && position > 1) {
        current = current->next;
        position--;
    }
    if (current == NULL) return;
    Node* node = newNode(value);
    node->next = current->next;
    current->next = node;
    list->size++;
}

void removeNode(LinkedList *list, void *value, bool (*equals)(void*, void*)) {
    if (list->head == NULL) return;
    list->size--;
    if (equals(list->head->value, value)) {
        Node* temp = list->head;
        list->head = list->head->next;
        free(temp);
        return;
    }
    Node* current = list->head;
    while (current->next != NULL) {
        if (equals(current->next->value, value)) {
            Node* temp = current->next;
            current->next = current->next->next;
            free(temp);
            return;
        }
        current = current->next;
    }
}

void removeAt(LinkedList *list, int position) {
    if (position < 0 || position >= list->size) return;
    list->size--;
    if (position == 0) {
        Node* temp = list->head;
        list->head = list->head->next;
        free(temp);
        return;
    }
    Node* current = list->head;
    while (current != NULL && position > 1) {
        current = current->next;
        position--;
    }
    if (current == NULL || current->next == NULL) return;
    Node* temp = current->next;
    current->next = current->next->next;
    free(temp);
}

void printList(LinkedList *list, void (*printValue)(void*)) {
    Node* current = list->head;
    printf("[");
    while (current != NULL) {
        printValue(current->value);
        if (current->next != NULL) {
            printf(", ");
        }
        current = current->next;
    }
    printf("]\n");
}

int search(LinkedList *list, void *value, bool (*equals)(void*, void*)) {
    Node* current = list->head;
    int position = 0;
    while (current != NULL) {
        if (equals(current->value, value)) {
            return position;
        }
        current = current->next;
        position++;
    }
    return -1;
}

void* get(LinkedList *list, int position) {
    if (position < 0 || position >= list->size) return NULL;
    Node* current = list->head;
    while (current != NULL && position > 0) {
        current = current->next;
        position--;
    }
    return current->value;
}

Node* getNode(LinkedList *list, int position) {
    if (position < 0 || position >= list->size) return NULL;
    Node* current = list->head;
    while (current != NULL && position > 0) {
        current = current->next;
        position--;
    }
    return current;
}

void updateNodeValue(LinkedList *list, void *targetValue, void *newValue, bool (*equals)(void*, void*)) {
    Node* current = list->head;
    while (current != NULL) {
        if (equals(current->value, targetValue)) {
            current->value = newValue;
            return;
        }
        current = current->next;
    }
}

int size(LinkedList *list) {
    return list->size;
}

void** toArray(LinkedList *list) {
    void** array = malloc(list->size * sizeof(void*));
    Node* current = list->head;
    int index = 0;
    while (current != NULL) {
        array[index++] = current->value;
        current = current->next;
    }
    return array;
}
