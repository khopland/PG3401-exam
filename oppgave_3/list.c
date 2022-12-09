#include "list.h"
#include "debugger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node *create_node(char *name, char *room, int date, int days, float prisePerDay);

struct Node *create_node(char *name, char *room, int date, int days, float prisePerDay) {
    struct Node *node = malloc(sizeof(struct Node));
    if (node == NULL) {
        error("Error: Could not allocate memory for node");
        return NULL;
    }

    strncpy(node->name, name, NAMESIZE - 1);
    strncpy(node->room, room, ROOMSIZE - 1);
    node->date = date;
    node->days = days;
    node->prisePerDay = prisePerDay;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void insertAtEnd(struct Node **head, char *name, char *room, int date, int days, float prisePerDay) {
    struct Node *node = create_node(name, room, date, days, prisePerDay);
    if (*head == NULL) {
        *head = node;
    } else {
        struct Node *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = node;
        node->prev = current;
    }
}

void removeLastNode(struct Node **head) {
    if (*head == NULL)
        return;

    struct Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    if (current == *head) {
        *head = NULL;
    } else {
        current->prev->next = NULL;
    }
    free(current);
}

double getTotalPriseForADate(struct Node *head, int date) {
    double prise = 0;
    struct Node *current = head;
    while (current != NULL) {
        if (current->date == date) {
            prise += current->days * current->prisePerDay;
        }
        current = current->next;
    }
    return prise;
}

void removeAllOlderNodesThenDate(struct Node **head, int date) {
    struct Node *current = *head;
    while (current != NULL) {
        if (current->date < date) {
            if (current == *head) {
                *head = current->next;
                if (*head != NULL) {
                    (*head)->prev = NULL;
                }
            } else {
                current->prev->next = current->next;
                if (current->next != NULL) {
                    current->next->prev = current->prev;
                }
            }
            struct Node *next = current->next;
            free(current);
            current = next;
        } else {
            current = current->next;
        }
    }
}

struct Node *findNodeByName(struct Node *head, char *name) {
    struct Node *current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void printListWithDate(struct Node *head, int date) {
    struct Node *current = head;
    printf("get list with date %d\n", date);
    while (current != NULL) {
        if (current->date == date) {
            printf("Name: %s, ", current->name);
            printf("Room: %s, ", current->room);
            printf("Date: %d, ", current->date);
            printf("Days: %d, ", current->days);
            printf("Prise per day: %.2f, ", current->prisePerDay);
            printf("\n");
        }
        current = current->next;
    }
}

void freeList(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        struct Node *next = current->next;
        free(current);
        current = next;
    }
}