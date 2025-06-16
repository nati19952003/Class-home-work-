#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 24
#define QUIZ_COUNT 5

typedef struct {
    char hangul[10];
    char engAlpha[5];
    char engPron[20];
} HangulCard;

typedef struct {
    char name[50];
    char challengeType[3];  // "KE" or "EK"
    int totalScore;
    int attemptCount;
} FlashResult;

HangulCard cards[MAX] = {
    {"ㄱ", "g", "giyeok"}, {"ㄴ", "n", "nieun"}, {"ㄷ", "d", "digeut"},
    {"ㄹ", "r", "rieul"}, {"ㅁ", "m", "mieum"}, {"ㅂ", "b", "bieup"},
    {"ㅅ", "s", "siot"}, {"ㅇ", "-", "ieung"}, {"ㅈ", "j", "jieut"},
    {"ㅊ", "ch", "chieut"}, {"ㅋ", "k", "kieuk"}, {"ㅌ", "t", "tieut"},
    {"ㅍ", "p", "pieup"}, {"ㅎ", "h", "hieut"},
    {"ㅏ", "a", "a"}, {"ㅑ", "ya", "ya"}, {"ㅓ", "eo", "eo"},
    {"ㅕ", "yeo", "yeo"}, {"ㅗ", "o", "o"}, {"ㅛ", "yo", "yo"},
    {"ㅜ", "u", "u"}, {"ㅠ", "yu", "yu"}, {"ㅡ", "eu", "eu"}, {"ㅣ", "i", "i"}
};

FlashResult results[10];
int resultCount = 0;

void shuffle(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        int j = rand() % size;
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void playKor2EngGame(char *name) {
    int indices[MAX];
    for (int i = 0; i < MAX; i++) indices[i] = i;
    shuffle(indices, MAX);

    int score = 0;
    char inputAlpha[10], inputPron[20];

    printf("\n--- Hangul to English Quiz ---\n");
    for (int i = 0; i < QUIZ_COUNT; i++) {
        int idx = indices[i];
        printf("%d. What is the English for %s? (alpha,pronunciation): ", i+1, cards[idx].hangul);
        scanf("%s %s", inputAlpha, inputPron);

        if (strcmp(inputAlpha, cards[idx].engAlpha) == 0 &&
            strcmp(inputPron, cards[idx].engPron) == 0) {
            printf("✅ Correct!\n");
            score += 10;
        } else {
            printf("❌ Incorrect. Correct: %s, %s\n", cards[idx].engAlpha, cards[idx].engPron);
        }
    }

    printf(">> Total Score: %d/50\n", score);

    // Store result
    strcpy(results[resultCount].name, name);
    strcpy(results[resultCount].challengeType, "KE");
    results[resultCount].totalScore = score;
    results[resultCount].attemptCount = 1;
    resultCount++;
}

void playEng2KorGame(char *name) {
    int indices[MAX];
    for (int i = 0; i < MAX; i++) indices[i] = i;
    shuffle(indices, MAX);

    int score = 0;
    char inputKor[10];

    printf("\n--- English to Hangul Quiz ---\n");
    for (int i = 0; i < QUIZ_COUNT; i++) {
        int idx = indices[i];
        printf("%d. What is the Hangul for %s (%s)? ", i+1, cards[idx].engAlpha, cards[idx].engPron);
        scanf("%s", inputKor);

        if (strcmp(inputKor, cards[idx].hangul) == 0) {
            printf("✅ Correct!\n");
            score += 10;
        } else {
            printf("❌ Incorrect. Correct: %s\n", cards[idx].hangul);
        }
    }

    printf(">> Total Score: %d/50\n", score);

    strcpy(results[resultCount].name, name);
    strcpy(results[resultCount].challengeType, "EK");
    results[resultCount].totalScore = score;
    results[resultCount].attemptCount = 1;
    resultCount++;
}

void showResult() {
    printf("\n--- Results ---\n");
    for (int i = 0; i < resultCount; i++) {
        printf("%s | Type: %s | Score: %d | Attempts: %d\n",
               results[i].name,
               results[i].challengeType,
               results[i].totalScore,
               results[i].attemptCount);
    }
}

void learnHangul() {
    char name[50];
    int choice;

    printf("Enter your name: ");
    scanf("%s", name);

    while (1) {
        printf("\n1. Hangul to English\n");
        printf("2. English to Hangul\n");
        printf("3. View Results\n");
        printf("0. Back\n");
        printf("Select an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                playKor2EngGame(name);
                break;
            case 2:
                playEng2KorGame(name);
                break;
            case 3:
                showResult();
                break;
            case 0:
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}

int main() {
    srand(time(NULL));
    int menu;

    while (1) {
        printf("\n=== Language and Pronunciation Training ===\n");
        printf("1. Hangul Jamo Learning\n");
        printf("0. Exit\n");
        printf("Select menu: ");
        scanf("%d", &menu);

        if (menu == 1) {
            learnHangul();
        } else if (menu == 0) {
            break;
        } else {
            printf("Invalid input.\n");
        }
    }

    return 0;
}
