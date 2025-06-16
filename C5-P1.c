#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SENTENCES 10
#define MAX_WORDS 10
#define MAX_LEN 100
#define TRAINEE_COUNT 3

// Structs
typedef struct {
    char eng[MAX_LEN];
    char kor[MAX_WORDS][MAX_LEN];
    int wordCount;
} SentencePair;

typedef struct {
    char nickname[MAX_LEN];
    int score;
} GrammarScore;

// Global Variables
SentencePair sentences[MAX_SENTENCES] = {
    {"I eat rice.", {"나는", "밥을", "먹는다", ".", "", "", "", "", "", ""}, 4},
    {"He goes to school.", {"그는", "학교에", "간다", ".", "", "", "", "", "", ""}, 4},
    {"She reads a book.", {"그녀는", "책을", "읽는다", ".", "", "", "", "", "", ""}, 4},
    {"We love Korea.", {"우리는", "한국을", "사랑한다", ".", "", "", "", "", "", ""}, 4},
    {"They play soccer.", {"그들은", "축구를", "한다", ".", "", "", "", "", "", ""}, 4},
    {"You are kind.", {"당신은", "친절하다", ".", "", "", "", "", "", "", ""}, 3},
    {"This is my house.", {"이것은", "나의", "집이다", ".", "", "", "", "", "", ""}, 4},
    {"He drinks water.", {"그는", "물을", "마신다", ".", "", "", "", "", "", ""}, 4},
    {"I watch TV.", {"나는", "TV를", "본다", ".", "", "", "", "", "", ""}, 4},
    {"She writes a letter.", {"그녀는", "편지를", "쓴다", ".", "", "", "", "", "", ""}, 4}
};

GrammarScore scores[TRAINEE_COUNT] = {
    {"Arthur", 0},
    {"Zaphod", 0},
    {"Trillian", 0}
};

// Utility
void shuffleWords(char words[][MAX_LEN], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp[MAX_LEN];
        strcpy(temp, words[i]);
        strcpy(words[i], words[j]);
        strcpy(words[j], temp);
    }
}

// Core Game Logic
void composeSentences(char *nickname) {
    int total = 0;
    for (int q = 0; q < 5; q++) {
        int idx = rand() % MAX_SENTENCES;
        SentencePair sp = sentences[idx];

        printf("\nQ%d: Translate this to Korean:\n%s\n", q + 1, sp.eng);

        char quizWords[MAX_WORDS][MAX_LEN];
        for (int i = 0; i < sp.wordCount; i++)
            strcpy(quizWords[i], sp.kor[i]);

        shuffleWords(quizWords, sp.wordCount);

        printf("Shuffled Korean words:\n");
        for (int i = 0; i < sp.wordCount; i++) printf("%s ", quizWords[i]);
        printf("\nEnter the correct sentence:\n");

        char userInput[500];
        getchar();  // flush newline
        fgets(userInput, sizeof(userInput), stdin);
        userInput[strcspn(userInput, "\n")] = 0;

        // Check correctness
        char correct[500] = "";
        for (int i = 0; i < sp.wordCount; i++) {
            strcat(correct, sp.kor[i]);
            strcat(correct, " ");
        }
        correct[strlen(correct) - 1] = 0; // remove last space

        if (strcmp(userInput, correct) == 0) {
            printf("Correct!\n");
            total += 20;
        } else {
            printf("Incorrect. Answer: %s\n", correct);
        }
    }

    // Update score
    for (int i = 0; i < TRAINEE_COUNT; i++) {
        if (strcmp(scores[i].nickname, nickname) == 0) {
            scores[i].score += total;
            break;
        }
    }

    printf("\nTotal Score: %d\n", total);
}

void printScore() {
    printf("\n=== Grammar Scores ===\n");
    for (int i = 0; i < TRAINEE_COUNT; i++) {
        printf("%s: %d\n", scores[i].nickname, scores[i].score);
    }
}

// Main Menu
void learnKoreanGrammar() {
    char nickname[MAX_LEN];
    int attempts = 3, found = 0;

    while (attempts--) {
        printf("Enter your nickname: ");
        scanf("%s", nickname);

        for (int i = 0; i < TRAINEE_COUNT; i++) {
            if (strcmp(scores[i].nickname, nickname) == 0) {
                found = 1;
                break;
            }
        }

        if (found) break;
        printf("Nickname not found. Attempts left: %d\n", attempts);
    }

    if (!found) return;

    while (1) {
        int choice;
        printf("\n1. Compose Korean Sentences\n2. Check Scores\n3. Exit\nChoice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            composeSentences(nickname);
        } else if (choice == 2) {
            printScore();
        } else {
            break;
        }
    }
}

int main() {
    srand((unsigned) time(NULL));
    learnKoreanGrammar();
    return 0;
}
