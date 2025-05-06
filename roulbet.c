#include "roul.h"


GameNode *headGame = NULL;
GameNode *currentTurn = NULL;

void endRound(GameNode *player) 
{
    
    int result = printresultroul();      
     const   char* color = getColor(result);        
    logresultroul(result);                 

    GameNode *currPlayer = player;
    GameNode *prevPlayer = NULL;


    while (currPlayer != NULL) 
    {
        BetNode *currBet = currPlayer->bet;

        while (currBet != NULL) 
        {
            int payout = 0;

            if (strcmp(currBet->betType, "1") == 0 && currBet->inputNum[0] == result) 
            payout = 35;
            else if (strcmp(currBet->betType, "2") == 0) 
            {
                for (int i = 0; i < 2; i++) 
                if (currBet->inputNum[i] == result) 
                {payout = 17; break; }
            } 
            else if (strcmp(currBet->betType, "3") == 0) 
            {
                for (int i = 0; i < 3; i++) 
                if (currBet->inputNum[i] == result) 
                {payout = 11; break;}
            } 
            else if (strcmp(currBet->betType, "4") == 0) 
            {
                for (int i = 0; i < 4; i++) 
                if (currBet->inputNum[i] == result) 
                { payout = 8; break; }
            } 
            else if (strcmp(currBet->betType, "6") == 0) 
            {
                for (int i = 0; i < 6; i++) 
                if (currBet->inputNum[i] == result) 
                { payout = 5; break; }
            } 

            else if (strcasecmp(currBet->betType, "red")  == 0 && strcmp(color, "red") == 0) 
            payout = 1;
            else if (strcasecmp(currBet->betType, "black") == 0 && strcmp(color, "black") == 0) 
            payout = 1;
            else if (strcasecmp(currBet->betType, "even") == 0 && result != 0 && result % 2 == 0) 
            payout = 1;
            else if (strcasecmp(currBet->betType, "odd") == 0 && result % 2 == 1) 
            payout = 1;
            else if (strcmp(currBet->betType, "1-12") == 0 && result >= 1 && result <= 12) 
            payout = 2;
            else if (strcmp(currBet->betType, "13-24") == 0 && result >= 13 && result <= 24) 
            payout = 2;
            else if (strcmp(currBet->betType, "25-36") == 0 && result >= 25 && result <= 36) 
            payout = 2;
            else if (strcasecmp(currBet->betType, "row1") == 0 && result % 3 == 1) 
            payout = 2;
            else if (strcasecmp(currBet->betType, "row2") == 0 && result % 3 == 2) 
            payout = 2;
            else if (strcasecmp(currBet->betType, "row3") == 0 && result % 3 == 0 && result != 0) 
            payout = 2;

            if (payout != 0) 
            {
                currPlayer->cash += currBet->money * (payout + 1); 
            }

            // 

            currBet = currBet->next;
        }

        while (currPlayer->bet != NULL) 
        {
            BetNode *temp = currPlayer->bet;
            currPlayer->bet = currPlayer->bet->next;
            free(temp);
        }
        while (currPlayer->undoStack != NULL) 
        {
            BetNode *temp = currPlayer->undoStack;
            currPlayer->undoStack = currPlayer->undoStack->next;
            free(temp);
        }

        if (currPlayer->cash == 0) 
        {
            GameNode *toDelete = currPlayer;
            if (prevPlayer == NULL) {
                player = currPlayer->next;
                currPlayer = player;
            } else {
                prevPlayer->next = currPlayer->next;
                currPlayer = currPlayer->next;
            }
            free(toDelete);
        } 
        else 
        {
            prevPlayer = currPlayer;
            currPlayer = currPlayer->next;
        }
    }

    FILE *fp = fopen("member.csv", "w");
    if (!fp) {
        perror("Could not open member.csv for writing");
        return;
    }

    currPlayer = player;
    while (currPlayer != NULL) {
        fprintf(fp, "%s,%d\n", currPlayer->name, currPlayer->cash);
        currPlayer = currPlayer->next;
    }

    fclose(fp);
    printf("Round ended. Winnings distributed.\n");
    Sleep(5000);
}

void printPlayerBets(GameNode *player) {
    if (player == NULL) {
        printf("No player provided.\n");
        return;
    }

    printf("Player: %s | Cash: %d\n", player->name, player->cash);

    BetNode *bet = player->bet;
    int betCount = 1;

    if (bet == NULL) {
        printf("  No bets placed.\n");
        return;
    }

    while (bet != NULL) 
    {
        printf("Bet %d: Type = %s | Money = %d ", betCount++, bet->betType, bet->money);
        for (int i = 0; i < 6; i++) {
            if (bet->inputNum[i] != -1) 
            {
                if (i==0)printf("| Numbers = ");
                printf("%d ", bet->inputNum[i]);
            }
        }
        printf("\n");
        bet = bet->next;
    }
}


// void addBetToPlayer(GameNode *player, const char *type, int input[], int count, int inputMoney, int addToUndoStack, int inputAction)
void addBetToPlayer(GameNode *player, char type[10], int input[], int inputMoney, int addToUndoStack, int inputAction)
{

    if(inputMoney > player->cash && addToUndoStack == 0)
    {
        printf("Not enough money.\n");
        // printf("%d\n", player->cash);
        // printf("%d\n", inputMoney);
        Sleep(1500);
        return;
    }

    // player->cash -= inputMoney;
    // printf("test:");Sleep(1000);
    BetNode *newBet = (BetNode *)malloc(sizeof(BetNode));
    // printf("test:");Sleep(1000);
    if (!newBet) {
        printf("Memory allocation failed for new bet.\n");
        return;
    }
    strncpy(newBet->betType, type, sizeof(newBet->betType) - 1);
    newBet->action = inputAction;
    newBet->money = inputMoney;

    // printf("test:");Sleep(1000);
    for (int i = 0; i < 6; i++) {
        if (input[i] != -1) {
            newBet->inputNum[i] = input[i];
        } else {
            newBet->inputNum[i] = -1;
        }
    }
    // printf("test:");Sleep(1000);
    newBet->next = NULL;

    // printf("test:");Sleep(1000);
    if (addToUndoStack == 0)
    {    player->cash -= inputMoney;
        if (player->bet == NULL) {
            player->bet = newBet;
        } else {
            BetNode *ptr = player->bet;
            while (ptr->next != NULL) {
                ptr = ptr->next;
            }
            ptr->next = newBet;
        }}
    if (addToUndoStack == 1)
    {    if (player->undoStack == NULL) {
            player->undoStack = newBet;
        } else {
            BetNode *ptr = player->undoStack;
            while (ptr->next != NULL) {
                ptr = ptr->next;
            }
            ptr->next = newBet;
        }}
    // printf("last:");Sleep(1000);
}

void delLastBet(GameNode *player, BetNode *target)
{
    BetNode *curr = player->bet;
    BetNode *prev = NULL;

    while (curr != NULL) {
        int match = 1;

        if (strcmp(curr->betType, target->betType) != 0)
            match = 0;

        for (int i = 0; i < 6; i++) {
            if (curr->inputNum[i] != target->inputNum[i]) {
                match = 0;
                break;
            }
        }
        if (curr->money != target->money)
            match = 0;
        if (match == 1) {
            if (prev == NULL)
                player->bet = curr->next;
            else
                prev->next = curr->next;

            player->cash += curr->money;
            free(curr);
            printf("Matching bet removed from bet list.\n");
            return;
        }

        prev = curr;
        curr = curr->next;
    }


    // if (isUndo ==0)
    // {
    //     // addBetToPlayer(GameNode *player, const char *type, int input[], int count, int inputMoney, int addToUndoStack, int inputAction)
    //     addBetToPlayer(player, curr->betType,curr->inputNum,6,curr->inputMoney,1, 0);
    // }

    if (prev == NULL) {
        player->bet = NULL;
    } else {
        prev->next = NULL;
    };
    
    free(curr);
}

void popUndo(GameNode *player)
{
    if (player->undoStack == NULL) {
        printf("Undo stack is empty.\n");Sleep(1500);
        return;
    }
    BetNode *curr = player->undoStack;
    BetNode *prev = NULL;
    while (curr->next != NULL) {
        prev = curr;
        curr = curr->next;
    }
    // printf("(((%d)))", curr -> action);Sleep(1500);

    if (curr -> action == 1)
    {
        delLastBet(player, curr);
        // printPlayerBets(player);
        // printf("(((%d)))", curr -> action);Sleep(1500);
    }

    if (curr -> action == 0)
    {
        addBetToPlayer(player, curr->betType,curr->inputNum,curr->money,0, -1);
    }

    if (prev == NULL) {
        player->undoStack = NULL;
    } else {
        prev->next = NULL;
    }

    free(curr);

}

void sort(int arr[], int n)
{
    for (int i = 1; i < n; i++) 
    {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

int conCheck(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++) {
        if (arr[i + 1] != arr[i] + 1) {
            return 0;  
        }
    }
    return 1;
}

int check1(int input)
{
    if (input>=0 && input<=36 ) return 1;
    else return 0;
}

int check2(int input[])
{
    if (input[0] == 0 && input[1] == 1) 
    return 1;
    else if (input[0] == 0 && input[1] == 1)
    return 1;
    else if (input[0] == 0 && input[1] == 2)
    return 1;
    else if (input[0] == 0 && input[1] == 3)
    return 1;
    else if (input[1] > 36 || input[0] <= 0)
    return 0;
    else if ((input[0] % 3 == 1 && input[1] % 3 == 2) || (input[0] % 3 == 2 && input[1] % 3 == 0)) 
    {
        if (conCheck(input, 2)) return 1;
    }
    else if (input[0]+3 == input[1])
    return 1;
    else return 0;
}

int check3(int input[])
{
    for (int i = 0; i < 3; i++) {
        printf("_%d_ ", input[i]);
    }
    if (input[0] == 0 && input[1] == 1 && input[2]== 2) 
    return 1;
    else if (input[0] == 0 && input[1] == 2 && input[2]== 3)
    return 1;
    else if (input[2] > 36 || input[0] <= 0)
    return 0;
    else if (input[0]%3 == 1 && input[1]%3 == 2 && input[2]%3== 0&& conCheck(input, 3))
    return 1;
    else return 0;
}

int check4(int input[])
{
    if (input[0] == 0 && input[1] == 1 && input[2] == 2 && input[3] == 3) 
    return 1;
    else if (input[3] > 36 || input[0] <= 0)
    return 0;

    // else if ((input[0] % 3 == 1 && input[1] % 3 == 2) || (input[0] % 3 == 2 && input[1] % 3 == 0)) 
    // {
    //     if (conCheck(input, 2)) return 1;
    // }

    else if (input[0]+3 == input[2] && input[1]+3 == input[3] && input[0] + 1 == input[1])
    return 1;
    else return 0;
}

int check6(int input[])
{
    // sort(input, 6);
    // for (int i = 0; i < 6; i++) {
    //     printf("_%d_ ", input[i]);
    // }
    if (input[1]==0)return 0;

    else if (input[0]%3 == 1 && input[1]%3 == 2 && input[2]%3== 0 &&input[3]%3 == 1 && input[4]%3 == 2 && input[5]%3== 0 && conCheck(input,6))
    return 1;
    else return 0;
}

void delBet(GameNode *player, int index)
{
    BetNode *curr = player->bet;
    BetNode *prev = NULL;
    int count = 1;
    if (player->bet == NULL || index <= 0) {
        printf("No bet to delete.\n");
        Sleep(1500);
        return;
    }
    while (curr != NULL && count < index) {
        prev = curr;
        curr = curr->next;
        count++;
    }
    if (curr == NULL) {
        printf("No bet to delete.\n");
        Sleep(1500);
        return;
    }
    
    if (prev == NULL) 
        player->bet = curr->next;  
    else 
        prev->next = curr->next;
    
        player->cash += curr->money;
        addBetToPlayer(player, curr->betType,curr->inputNum,curr->money,1, 0);
    free(curr);
    printf("Bet %d removed successfully.\n", index);
    Sleep(1000);
}

void playerTurn(GameNode *player)

{
    char d[100];
    int c;
    int m;
    int e[6];
    while (1)
    {
        for (int i = 0; i < 6; i++)
        {
            e[i] = -1;
        }

        clearter();

        printTable(-1);
        // printf("Enter Your Bet %s\n", player->name);
        // printf("--------------------------------------------\n");
        printPlayerBets(player);
        printf("--------------------------------------------\n");
        printf("ENTER 1: Straight Up       (1 Number ) 35:1\n");
        printf("ENTER 2: Split             (2 Numbers) 17:1\n");
        printf("ENTER 3: Street            (3 Numbers) 11:1\n");
        printf("ENTER 4: Corner/Top Line   (4 Numbers)  8:1\n");
        printf("ENTER 6: Line              (6 Numbers)  5:1\n");
    
        printf("ENTER Row1/Row2/Row3/1-12/13-24/25-16   2:1\n");
        printf("ENTER Odd/Even/Black/White              1:1\n");
        printf("--------------------------------------------\n");
        // printf("\033[1;31mTEST: 4 ROULETTE, 5 BET \033[0m\n");
        printf("ENTER DEL: Delete Bet (DEL 1) \n");
        printf("ENTER UNDO: Undo Recent Action\n");
        printf("ENTER 0: Next Player\n\n");
        printf("Please ENTER: ");

        scanf("%s", d);

        if (strcmp(d, "1") == 0) {
            printf("Choose your one lucky number:");
            scanf("%d", &c);  
            if (check1(c)) {
                printf ("Enter money amout: ");
                scanf("%d",&m);
                int temp[1] = {c};
                addBetToPlayer(player, "1", temp,  m, 0, -1);
                addBetToPlayer(player, "1", temp,  m, 1, 1);
            }
            else
            {
                printf("invalid\n");Sleep(1500);
            }
        } 
        else if (strcmp(d, "2") == 0) {
            printf("\033[1;4m%sWarning%s\033[0m Choose numbers next to each other when making a split bet.\n", RED,RESET);
            for (int i = 0; i < 2; i++) {
                printf("Choose the lucky number(%d):", i+1);
                scanf("%d", &c);

                e[i] = c;
            }sort(e,2);
            if (check2(e)) {
                printf ("Enter money amout: ");
                scanf("%d",&m);
                addBetToPlayer(player, "2", e, m, 0 ,-1);
                addBetToPlayer(player, "2", e,  m, 1, 1);
            } else {
                printf("invalid\n");Sleep(1500);
            }
        } 
        else if (strcmp(d, "3") == 0) {
            printf("\033[1;4m%sWarning%s\033[0m choose three numbers in a vertical row.\n", RED, RESET);
            for (int i = 0; i < 3; i++) {
                printf("Choose the lucky number(%d):", i+1);
                scanf("%d", &c);
                e[i] = c;
            }sort(e,3);
            if (check3(e)) {                printf ("Enter money amout: ");
                scanf("%d",&m);
                addBetToPlayer(player, "3", e, m, 0, -1);
                // printf("Bet recorded.\n");
                addBetToPlayer(player, "3", e,  m, 1, 1);
            } else {
                printf("invalid\n");Sleep(1500);
            }
        } 
        else if (strcmp(d, "4") == 0) {
            printf("\033[1;4m%sWarning%s\033[0m select four numbers that form a square on the layout.(Example: 1, 2, 4, 5)\n", RED, RESET);
            for (int i = 0; i < 4; i++) {
                printf("Choose the lucky number(%d):", i+1);
                scanf("%d", &c);
                e[i] = c;
            }sort(e,4);
            if (check4(e)) {                printf ("Enter money amout: ");
                scanf("%d",&m);
                addBetToPlayer(player, "4", e,m, 0 ,-1);addBetToPlayer(player, "4", e,  m, 1, 1);
            } else {
                printf("invalid\n");Sleep(1500);
            }
        } 
        else if (strcmp(d, "6") == 0) {
            printf("\033[1;4m%sWarning%s\033[0m select six numbers in two connected rows (a double street)\n", RED, RESET);
            for (int i = 0; i < 6; i++) {
                printf("Choose the lucky number(%d):", i+1);
                scanf("%d", &c);
                e[i] = c;
            }sort(e,6);
            if (check6(e)) {                printf ("Enter money amout: ");
                scanf("%d",&m);
                addBetToPlayer(player, "6", e, m, 0, -1);addBetToPlayer(player, "6", e,  m, 1, 1);
                printf("Bet recorded.\n");
            } else {
                printf("invalid\n");Sleep(1500);
            }
        } 

        else if  (strcasecmp(d, "red") == 0 || strcasecmp(d, "black") == 0 ||strcasecmp(d, "even") == 0 || strcasecmp(d, "odd") == 0 ||strcmp(d, "1-12") == 0 || strcmp(d, "13-24") == 0 || strcmp(d, "25-36") == 0 ||strcasecmp(d, "row1") == 0 || strcasecmp(d, "row2") == 0 || strcasecmp(d, "row3") == 0)
        {
            printf("Enter money amount: ");
            scanf("%d", &m);
            addBetToPlayer(player, d, e, m, 0, -1);
            addBetToPlayer(player, d, e, m, 1, 1);
        }

        else if (strcasecmp(d, "del") == 0) { 
            scanf("%d", &c); 
            delBet(player,c);

        }        else if (strcasecmp(d, "undo") == 0) { 
            popUndo(player);

        }
        else if (strcmp(d, "0") == 0) {
            // printPlayerBets(player);
            Sleep(1500);
            break;
        } 
        else {
            printf("Invalid choice..\n");
            Sleep(1500);
        }
    }

}

// GameNode* createGameCopy(GameNode *originalHead) {
//     GameNode *tempHead = NULL;
//     GameNode *ptr = originalHead;

//     while (ptr != NULL) {
//         GameNode *newNode = (GameNode *)malloc(sizeof(GameNode));
//         newNode->bet = NULL;
//         strcpy(newNode->name, ptr->name);
//         newNode->cash = ptr->cash;
//         newNode->next = NULL;
//         GameNode *tptr = tempHead, *prev = NULL;
//         while (tptr != NULL && newNode->cash <= tptr->cash) {
//             prev = tptr;
//             tptr = tptr->next;
//         }

//         if (prev == NULL) {
//             newNode->next = tempHead;
//             tempHead = newNode;
//         } else {
//             newNode->next = tptr;
//             prev->next = newNode;
//         }

//         ptr = ptr->next;
//     }

//     return tempHead;
// }

void printTurn(GameNode*head)
{
    clearter();
    GameNode*tempPrint ;
    printf("\033[1;4mTurn Order\033[0m\n");

    while (tempPrint != NULL)
    {
        printf("-> |%s|\n", tempPrint->name);
        tempPrint = tempPrint->next;
    }
    printf("\n%sENTER%s: Start Game", GREEN, RESET);
    getchar();
    getchar();
}

void betMenu()
{
    mergeSort(&head);
    // headGame = createGameCopy(head);
    currentTurn = head;
printTurn(head);
    while (currentTurn != NULL)
    {
        playerTurn(currentTurn);
        currentTurn = currentTurn->next;
    }
endRound(head);
}