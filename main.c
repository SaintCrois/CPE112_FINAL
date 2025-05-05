#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>

// #include "sort.h"
#include "roul.h"
GameNode *head = NULL;





void freeData(GameNode* head) 
{
    GameNode * temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}



void CashOut() 
{
    char inputName[100];
    int cash;
    int found = 0;
    printf("Please enter your Namo999's User Name: ");
    scanf("%s", inputName);
    GameNode *ptr = head;
    GameNode *prev = NULL;

    while(ptr != NULL) {
        if(strcmp(ptr->name, inputName) == 0) {
            found = 1;
            break;
        }
        prev = ptr;
        ptr = ptr->next;
    }

    if (!found) {
        printf("Sorry, the username was not found\n");
        return;
    }
    printf("Current Amount: %d\n",ptr->cash);
    printf("Cash out! Enter the amount to withdraw (enter full amount to delete account): ");
    scanf("%d", &cash);

    if (cash <= 0) {
        printf("Invalid amount. Canceling..\n");
        return;
    }

    if (ptr->cash < cash) {
        printf("Not enough funds! \n" );
        return;
    }

    ptr->cash -= cash;

    if (ptr->cash == 0) {
        if (prev == NULL) {
            head = ptr->next;  
        } else {
            prev->next = ptr->next;
        }
        free(ptr);
        printf("All cash withdrawn. User account removed.\n");
    } else {
        printf("Successfully cashed out %d. Remaining balance: %d\n", cash, ptr->cash);
    }

    FILE *fp = fopen("member.csv", "w");
    if (fp == NULL) {
        printf("Could not write to file.\n");
        return;
    }

    ptr = head;
    while (ptr != NULL) {
        fprintf(fp, "%s,%d\n", ptr->name, ptr->cash);
        ptr = ptr->next;
    }
    fclose(fp);
}


void loadData() {
    FILE *fp = fopen("member.csv", "r");
    if (fp == NULL) return;

    char line[100], name[100];
    int cash;
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%[^,],%d", name, &cash);
        insertEnd(&head, cash, name);
    }
    fclose(fp);
}


void Register() 
{
    char name[100];

    int c;

    printf("Register the User Name: ");
    scanf("%s", name);
    GameNode *ptr = head;

    while(ptr != NULL) 
    {
        if(strcmp(ptr->name, name) == 0) {
            printf("User already exists!\n");
            return;
        }
        ptr = ptr->next;
    }

    printf("Enter the amount of CASH to deposit: ");
    scanf("%d", &c);
    if (c < 0) {
        printf("Invalid cash amount.\n");
        return;
    }  
    insertEnd(&head, c, name);

    FILE *fp = fopen("member.csv", "a");
    if(fp == NULL){
        printf("File cannot be reached.\n");
        return;
    }

    fprintf(fp, "%s,%d\n", name, c);
    fclose(fp);
    printf("Successfully registered and deposited %d cash!\n", c);
    printf("Returning..\n");
}

void Cash() {
    char inputName[100];
    int cash;
    int found = 0;

    printf("Please enter your Namo999's User Name: ");
    scanf("%s", inputName);

    GameNode *ptr = head;
    while(ptr != NULL) {
        if(strcmp(ptr->name, inputName) == 0) {
            found = 1;
            break;
        }
        ptr = ptr->next;
    }

    if (!found) {
        printf("Sorry, the username was not found\n");
        return;
    }

    printf("Cash in! Enter the amount of CASH: ");
    scanf("%d", &cash);
    if (cash == 0) {
        printf("Canceling..\n");
        return;
    }

    ptr->cash += cash;
    printf("Successfully Cash in! Have fun %s!\n", ptr->name);
    FILE *fp = fopen("member.csv", "w");
    if (fp == NULL) 
    {
        printf("Could not write to file.\n");
        return;
    }

    ptr = head;
    while (ptr != NULL) 
    {
        fprintf(fp, "%s,%d\n", ptr->name, ptr->cash);
        ptr = ptr->next;
    }
    fclose(fp);
}

// GameNode* createheadCopy(GameNode *originalHead) {
//     GameNode *tempHead = NULL;
//     GameNode *ptr = originalHead;

//     while (ptr != NULL) {
//         GameNode *newGameNode = (GameNode *)malloc(sizeof(GameNode));
//         strcpy(newGameNode->name, ptr->name);
//         newGameNode->cash = ptr->cash;
//         newGameNode->next = NULL;
//         GameNode *tptr = tempHead, *prev = NULL;
//         while (tptr != NULL && newGameNode->cash <= tptr->cash) {
//             prev = tptr;
//             tptr = tptr->next;
//         }

//         if (prev == NULL) {
//             newGameNode->next = tempHead;
//             tempHead = newGameNode;
//         } else {
//             newGameNode->next = tptr;
//             prev->next = newGameNode;
//         }

//         ptr = ptr->next;
//     }

//     return tempHead;
// }


void list() {
    system("cls");

    // GameNode *head = createheadCopy(head);
    mergeSort(&head);

    int rank = 1;
    GameNode *curr = head;
    while (curr != NULL) {
        printf("RANK %d : %s | %d \n", rank++, curr->name, curr->cash);
        curr = curr->next;
    }

    // while (head != NULL) {
    //     curr = head->next;
    //     // free(head);
    //     // head = next;
    // }

    printf("\nPlease ENTER to return: ");
    // int choice;
    // scanf("%d", &choice);
    getchar();
    getchar();
}



void insertEnd(GameNode **head, int cash, char namu[100]) 
{
    GameNode *newGameNode = (GameNode *)malloc(sizeof(GameNode));
    newGameNode->next = NULL;
    newGameNode->cash = cash;
    newGameNode->bet = NULL;
    newGameNode->undoStack= NULL;
    strcpy(newGameNode->name, namu);

    if (*head == NULL) 
    {
        *head = newGameNode;
        return;
    }

    GameNode *ptr = *head;

    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = newGameNode;
}


void printlist(GameNode **head)
{
    int rank = 0;
    GameNode *ptr = *head;
    while(ptr != NULL){
        printf("RANK %d : %s | %d \n", ++rank, ptr->name, ptr->cash);
        ptr = ptr->next;
    }
}


int main() {
    int d;
    loadData();


    while (1) {
        system("cls");
        printf("Hello! Welcome to Namo999\n");
        printf("---------------------------\n");
        printf("ENTER 1: Register\n");
        printf("ENTER 2: Cash in\n");
        printf("ENTER 3: Cash out\n");
        printf("ENTER 4: List of the WEALTHIEST\n");
        printf("ENTER 5: START GAME\n");
        // printf("\033[1;31mTEST: 4 ROULETTE, 5 BET \033[0m\n");
        printf("ENTER 0: EXIT\n\n");
        printf("Please ENTER: ");
        scanf("%d", &d);

        // char buffer[100];
        // fgets(buffer, sizeof(buffer), stdin);
        // if (sscanf(buffer, "%d", &d) != 1) {
        //     printf("Invalid choice..\n");
        //     Sleep(2000);
        //     continue;  
        // }

        if (d == 1) {
            Register();Sleep(2000);
        } else if (d == 2) {
            
            if (head == NULL) {
                printf("No players registered! Please register first.\n");
            } else {Cash();Sleep(2000);
            }
        } else if (d == 4) {
            if (head == NULL) {
                printf("No players registered! Please register first.\n");
            } else {
            list();}
            
        } else if (d == 3) {
            if (head == NULL) {
                printf("No players registered! Please register first.\n");
            } else {CashOut();Sleep(2000);
            }
            
        } else if (d == 6) {
            int result = printresultroul();
            logresultroul(result);
            Sleep(3000);
        } else if (d == 5) {
            if (head == NULL) {
                printf("No players registered! Please register first.\n");
            } else {
                betMenu();
            }
        }
         else if (d == 0) {
            freeData(head);
            break;
        } else {
            printf("Invalid choice..\n");
            Sleep(2000);
        }
    }
    return 0;
}










GameNode* sortedMerge(GameNode* a, GameNode* b) {
    GameNode* result = NULL;

    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    if (a->cash >= b->cash) {
        result = a;
        result->next = sortedMerge(a->next, b);
    } else {
        result = b;
        result->next = sortedMerge(a, b->next);
    }

    return result;
}

void frontBackSplit(GameNode* source, GameNode** frontRef, GameNode** backRef) {
    GameNode* fast;
    GameNode* slow;
    if (source == NULL || source->next == NULL) {
        *frontRef = source;
        *backRef = NULL;
        return;
    }

    slow = source;
    fast = source->next;
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

void mergeSort(GameNode** headRef) {
    GameNode* head = *headRef;
    GameNode* a;
    GameNode* b;

    if (head == NULL || head->next == NULL)
        return;

    frontBackSplit(head, &a, &b);

    mergeSort(&a);
    mergeSort(&b);

    *headRef = sortedMerge(a, b);
}