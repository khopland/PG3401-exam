#ifndef __LIST_H__
#define __LIST_H__
#define NAMESIZE 50
#define ROOMSIZE 10
struct Node {
    char name[NAMESIZE];
    char room[ROOMSIZE];
    int date;
    int days;
    float prisePerDay;
    struct Node *prev;
    struct Node *next;
};

void insertAtBeginning(struct Node **head, char *name, char *room, int date, int days, float prisePerDay);
void insertAtEnd(struct Node **head, char *name, char *room, int date, int days, float prisePerDay);
void removeLastNode(struct Node **head);
void removeAllOlderNodesThenDate(struct Node **head, int date);
double getTotalPriseForADate(struct Node *head, int date);
struct Node *findNodeByName(struct Node *head, char *name);
void printListWithDate(struct Node *head, int date);
void freeList(struct Node *head);


#endif