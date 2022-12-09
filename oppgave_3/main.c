#include "list.h"
#include "debugger.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void menu(struct Node **head);
void addReservation(struct Node **head);
void deleteOldReservations(struct Node **head);
void findReservationByName(struct Node **head);
void findTotalPriseForADay(struct Node **head);
void printAllReservationsWithMatchingDate(struct Node **head);
int getCurrentDate();

int main(void) {
    struct Node *head = NULL;

    menu(&head);

    freeList(head);
    return 0;
}

void menu(struct Node **head) {
    int choice = 0;
    char input;

    do {
        printf("\nSkriv inn ønsket kommando [1-7]:\n");
        printf("1. Legg til en reservasjon \n");
        printf("2. slett siste reservasjon\n");
        printf("3. slett alle reservasjoner eldre en dagens dato + x\n");
        printf("4. finn en reservasjon på ett navn\n");
        printf("5. summer sammen prisen av alle reservasjon i forhold til en dato \n");
        printf("6. skriv ut alle reservasjoner på en gitt dato\n");
        printf("7. Avslutte\n");

        do {
            input = getchar(); // to clear the buffer for \n
        } while (input == '\n');

        choice = atoi(&input);

        do {
            input = getchar(); // to clear the buffer for all other chars
        } while (input != EOF && input != '\n');

        switch (choice) {
        case 1:
            printf("Legg til en reservasjon\n");
            addReservation(head);
            break;
        case 2:
            removeLastNode(head);
            printf("slettet siste reservasjon\n");
            break;
        case 3:
            printf("slett alle reservasjoner eldre en dagens dato + x\n");
            deleteOldReservations(head);
            break;
        case 4:
            printf("finn en reservasjon på ett navn\n");
            findReservationByName(head);
            break;
        case 5:
            printf("summer sammen prisen av alle reservasjon i forhold til en dato \n");
            findTotalPriseForADay(head);
            break;
        case 6:
            printf("skriv ut alle reservasjoner på en gitt dato\n");
            printAllReservationsWithMatchingDate(head);
            break;
        case 7:
            printf("Avslutte\n");
            break;

        default:
            printf("ikke et tall mellom 1-7, prøv igjen\n");
            break;
        }

    } while (choice != 7);
}

void printAllReservationsWithMatchingDate(struct Node **head) {
    int date = 0;
    printf("Skriv inn dato: (YYYYMMDD)\n");
    if (scanf("%d", &date) < 1) {
        error("Error reading date");
        printf("Error reading date");
        return;
    }
    printListWithDate(*head, date);
}

void findTotalPriseForADay(struct Node **head) {
    int date;
    printf("Skriv inn dato: (YYYYMMDD)\n");
    if (scanf("%d", &date) < 1) {
        error("Error reading date");
        printf("Error reading date");
        return;
    }
    double totalPrise = getTotalPriseForADate(*head, date);
    printf("Totalprisen for %d er %lf\n", date, totalPrise);
}

void findReservationByName(struct Node **head) {
    char name[NAMESIZE];
    printf("Skriv inn navn: (max %d)\n", NAMESIZE);
    if (fgets(name, NAMESIZE, stdin) == NULL) {
        error("Error reading name");
        printf("Error reading name");
        return;
    }
    name[strcspn(name, "\n")] = '\0';

    struct Node *node = findNodeByName(*head, name);
    if (node != NULL) {
        printf("Fant reservasjon for %s på room %s for %f per natt,antall dager er %d som starter %d\n", node->name,
               node->room, node->prisePerDay, node->days, node->date);
    } else {
        printf("reservasjon ikke funnet på navn %s\n", name);
    }
}

void deleteOldReservations(struct Node **head) {
    int days;
    printf("Skriv inn antall dager:\n");
    if (scanf("%d", &days) < 1) {
        error("Error reading days");
        printf("Error reading days");
        return;
    }
    removeAllOlderNodesThenDate(head, getCurrentDate() + days);
}

void addReservation(struct Node **head) {
    char name[NAMESIZE] = {0};
    char room[ROOMSIZE] = {0};
    int date = 0;
    int days = 0;
    float prisePerDay = 0;

    printf("Skriv inn navn: (max %d)\n", NAMESIZE);
    if (fgets(name, NAMESIZE, stdin) == NULL) {
        error("Error reading name");
        printf("Error reading name");
        return;
    }
    name[strcspn(name, "\n")] = '\0';

    printf("Skriv inn rom: (max %d)\n", ROOMSIZE);
    if (fgets(room, ROOMSIZE, stdin) == NULL) {
        error("Error reading room");
        printf("Error reading room");
        return;
    }
    room[strcspn(room, "\n")] = '\0';

    printf("Skriv inn dato: (YYYYMMDD)\n");
    if (scanf("%d", &date) < 1) {
        error("Error reading date");
        printf("Error reading date");
        return;
    }

    printf("Skriv inn antall dager:\n");
    if (scanf("%d", &days) < 1) {
        error("Error reading days");
        printf("Error reading days");
        return;
    }

    printf("Skriv inn pris per dag:\n");
    if (scanf("%f", &prisePerDay) < 1) {
        error("Error reading prisePerDay");
        printf("Error reading prisePerDay");
        return;
    }
    insertAtEnd(head, name, room, date, days, prisePerDay);

    printf("Reservasjon lagt til på Navn: %s\n", name);
}

int getCurrentDate() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_mday + ((tm.tm_mon + 1) * 100) + ((tm.tm_year + 1900) * 10000);
}