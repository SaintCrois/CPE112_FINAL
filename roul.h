#ifndef ROUL_H
#define ROUL_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include <windows.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h> 
#endif


#define RED     "\033[1;31m"
#define BLACK   "\033[1;30m"
#define GREEN   "\033[1;32m"
#define RESET   "\033[0m"
#define YELLOW  "\033[0;33m"

// typedef struct node {
//     int cash;
//     char name[100];
//     struct node *next;
// } Node;

// extern Node *head;

// typedef struct undoNode {
//     char betType[10];    
//     int inputNum[6];
//     int money;
//     int action;     
//     struct undoNode *next;
// } UndoNode;

typedef struct betNode {
    char betType[10];    
    int inputNum[6];     
    int money;

    int action;  
    struct betNode *next;
} BetNode;

typedef struct gameNode {
    int cash;
    char name[100];
    BetNode *bet;
    BetNode *undoStack;
    struct gameNode *next;
} GameNode;

extern GameNode *head;


// void addBetToPlayer(GameNode *player, char type[10], int input[], int inputMoney, int addToUndoStack, int inputAction)


void Register();
void Cash();
void list();
void insertEnd(struct gameNode **head, int cash, char namu[100]);
void printlist(struct gameNode **head);
void mergeSort(GameNode** headRef);

//  GameNode* createSortedCopy(GameNode *originalHead);

// roul.c
int clearter();
int gennumber();
void printTable(int result);
void printResult(int result);
int printresultroul();
void logresultroul(int result); const char* getColor(int number) ;
void format_centeRED(int num, char *out, int a);

// roulbet.c
void betMenu();

#endif