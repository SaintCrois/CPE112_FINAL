#include "roul.h"

#ifdef _WIN32
#define IS_WINDOWS 1
#else
#define IS_WINDOWS 0
#endif

#if IS_WINDOWS
    #define BOX_TOP_LEFT     "+"
    #define BOX_TOP_RIGHT    "+"
    #define BOX_BOTTOM_LEFT  "+"
    #define BOX_BOTTOM_RIGHT "+"
    #define BOX_HORIZONTAL   "-"
    #define BOX_VERTICAL     "|"
    #define BOX_CROSS        "+"
    #define BOX_T_UP         "+"
    #define BOX_T_DOWN       "+"
    #define EMOJI_GREEN      "GREEN"
    #define EMOJI_RED        "RED"
    #define EMOJI_BLACK      "BLACK"
#else
    #define BOX_TOP_LEFT     "╔"
    #define BOX_TOP_RIGHT    "╗"
    #define BOX_BOTTOM_LEFT  "╚"
    #define BOX_BOTTOM_RIGHT "╝"
    #define BOX_HORIZONTAL   "═"
    #define BOX_VERTICAL     "║"
    #define BOX_CROSS        "╬"
    #define BOX_T_UP         "╩"
    #define BOX_T_DOWN       "╦"
    #define EMOJI_GREEN      "🟢"
    #define EMOJI_RED        "🔴"
    #define EMOJI_BLACK      "⚫"
#endif

const char* numbercolor[37] = {
    GREEN, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK,
    RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK,
    RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK, RED, BLACK,
    RED, BLACK, RED, BLACK, RED, BLACK
};

int clearter(){
    system("cls"); 
    return 0;
}


int gennumber(){
    return rand() % 37;
}

const char* getColor(int number) 
{
    return numbercolor[number];
}


void format_centeRED(int num, char *out, int a) {
    if (num < 10)
    if (a!= num)
        sprintf(out, " %d  ", num);
        else 
            sprintf(out, ">%d< ", num); 
        
    else
    {
        if (a!=num)
            sprintf(out, " %d ", num);
            else 
        sprintf(out, ">%d<", num); 
    }
}

void printTable(int result){
    // const char* BOLD = "\033[1m";

    
    // int RED_numbers[] = {
    //     1, 3, 5, 7, 9, 12, 14, 16, 18, 19,
    //     21, 23, 25, 27, 30, 32, 34, 36
    // };
    // int is_RED[37] = {0};
    // for (int i = 0; i < sizeof(RED_numbers) / sizeof(RED_numbers[0]); i++) {
    //     is_RED[RED_numbers[i]] = 1;
    // }
    int rows = 3;
    // int cols = 12;
    int cols = 13;

    printf("%s", BOX_TOP_LEFT);
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < 7; j++) printf("%s", BOX_HORIZONTAL);
        printf("%s", BOX_T_DOWN);
    }
    printf("\b%s\n", BOX_TOP_RIGHT);

    for (int j = 0; j < rows; j++) {
        if (j == 1) printf("%s%s   0   %s%s",BOX_VERTICAL, numbercolor[0], RESET, BOX_VERTICAL);
        else printf("%s       %s", BOX_VERTICAL, BOX_VERTICAL);
        for (int i = 0; i < 12; i++) {
            int number = (j + 1) + (i * 3);
            if (number >= 37) {
                printf("       %s", BOX_VERTICAL);
                continue;
            }

            const char* color = numbercolor[number];
            if (number == result) {
                printf(" %s(%2d)%s  %s", color, number, RESET, BOX_VERTICAL);
            } else {
                printf(" %s %2d %s  %s", color, number, RESET, BOX_VERTICAL);
            }
        }
        printf("\n");

        if (j < rows - 1) {
            // printf("%s%s%s%s%s%s%s%s", BOX_CROSS, BOX_HORIZONTAL, BOX_HORIZONTAL, BOX_HORIZONTAL, BOX_HORIZONTAL, BOX_HORIZONTAL, BOX_HORIZONTAL, BOX_HORIZONTAL);
            // printf("%s%s%s%s%s%s%s%s%s", BOX_CROSS, BOX_HORIZONTAL, BOX_HORIZONTAL, BOX_HORIZONTAL, BOX_HORIZONTAL, BOX_HORIZONTAL, BOX_HORIZONTAL, BOX_HORIZONTAL, BOX_CROSS);
            printf("%s       %s", BOX_CROSS, BOX_CROSS);

            for (int i = 0; i < 12; i++) {
                for (int k = 0; k < 7; k++) printf("%s", BOX_HORIZONTAL);
                printf("%s", BOX_CROSS);
            }
            printf("\b%s\n", BOX_CROSS);
        }
    }

    printf("%s", BOX_BOTTOM_LEFT);
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < 7; j++) printf("%s", BOX_HORIZONTAL);
        printf("%s", BOX_T_UP);
    }
    printf("\b%s\n", BOX_BOTTOM_RIGHT);
    // printf("%s    0   %s\n", numbercolor[0], RESET);
}

void printResult(int result) {
    const char* color = numbercolor[result];
    const char* emoji;

    if (result == 0) emoji = EMOJI_GREEN;
    else if (color == RED) emoji = EMOJI_RED;
    else emoji = EMOJI_BLACK;

    printf("\nTADAAA! ROULETTE LANDED ON %s %s %d %s%s\n", color, emoji, result, emoji, RESET);
}


int printresultroul(){
    srand(time(0));
    int result = gennumber();
    printTable(result);
    printResult(result);
    return result;
}

void logresultroul(int result){
    FILE* log = fopen("Roulettelog.csv", "a");
    if (log != NULL) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        fprintf(log, "%02d-%02d-%04d %02d:%02d:%02d %02d\n",
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
            tm.tm_hour, tm.tm_min, tm.tm_sec, result);
        fclose(log);
    }
}
