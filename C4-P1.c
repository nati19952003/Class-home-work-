#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TRAINEES 4
#define MAX_QUESTIONS 10
#define QUIZ_SIZE 5

typedef struct {
    int id;
    char question[100];
    char answer[100];
} KoreanQuiz;

typedef struct {
    char name[50];
    char nickname[30];
    char nationality[30];
    int score;
    char pass;
} Trainee;

typedef struct {
    int qid;
    char result; // 'O' or 'X'
} QuizAnswer;

// Question bank (10 questions)
KoreanQuiz questionBank[MAX_QUESTIONS] = {
    {1, "Hello", "안녕하세요"},
    {2, "Thank you", "감사합니다"},
    {3, "Sorry", "죄송합니다"},
    {4, "Yes", "네"},
    {5, "No", "아니요"},
    {6, "Delicious", "맛있어요"},
    {7, "Where is the bathroom?", "화장실 어디에요?"},
    {8, "I don’t know", "모르겠어요"},
    {9, "Help me", "도와주세요"},
    {10, "Goodbye", "안녕히 가세요"}
};

// Final Millieway trainees (1 Korean + 3 others)
Trainee trainees[MAX_TRAINEES] = {
    {"Park Ji-yeon", "Jiyeon", "Korean", 0, 'X'},
    {"Liam Chen", "Liam", "American", 0, 'X'},
    {"Sakura Tanaka", "Sakura", "Japanese", 0, 'X'},
    {"Mei Ling", "Mei", "Chinese", 0, 'X'}
};

// Quiz answers
QuizAnswer quizResults[QUIZ_SIZE];

// Function to check answer
char isAnswer(int qid, const char *userAns) {
    for (int i = 0; i < MAX_QUESTIONS; i++) {
        if (questionBank[i].id == qid) {
            return strcmp(questionBank[i].answer, userAns) == 0 ? 'O' : 'X';
        }
    }
    return 'X';
}

// Select a random non-Korean trainee
Trainee* selectRandomTakers() {
    int idx;
    do {
        idx = rand() % MAX_TRAINEES;
    } while (strcmp(trainees[idx].nationality, "Korean") == 0);

    printf("Welcome, %s, to the Korean quiz session!\n", trainees[idx].name);
    return &trainees[idx];
}

// Serve quiz
void serveRandomQuiz(Trainee *taker) {
    int chosen[QUIZ_SIZE] = {0}; // Store selected indexes
    int count = 0, totalScore = 0;
    char userAnswer[100];

    printf("The quiz will begin in 30 seconds. Prepare yourself...\n");
    sleep(1); // For actual pause, replace with sleep(30) in real usage

    while (count < QUIZ_SIZE) {
        int index = rand() % MAX_QUESTIONS;

        // Ensure unique questions
        int duplicate = 0;
        for (int i = 0; i < count; i++) {
            if (chosen[i] == index) {
                duplicate = 1;
                break;
            }
        }
        if (duplicate) continue;

        chosen[count] = index;
        printf("Q%d: Translate to Korean: \"%s\"\n", count + 1, questionBank[index].question);
        printf("Your answer: ");
        fgets(userAnswer, sizeof(userAnswer), stdin);
        userAnswer[strcspn(userAnswer, "\n")] = '\0'; // Remove newline

        // Check answer once and record
        quizResults[count].qid = questionBank[index].id;
        quizResults[count].result = isAnswer(questionBank[index].id, userAnswer);

        if (quizResults[count].result == 'O') totalScore += 20;

        count++;
    }

    taker->score = totalScore;
    taker->pass = (totalScore >= 80) ? 'O' : 'X';

    printf("\n--- Quiz Result for %s ---\n", taker->nickname);
    for (int i = 0; i < QUIZ_SIZE; i++) {
        printf("Q%d [%s] - %c\n", i + 1, questionBank[chosen[i]].question, quizResults[i].result);
    }
    printf("Total Score: %d\n", taker->score);
    printf("Status: %s\n", taker->pass == 'O' ? "Pass" : "Fail");
}

// Korean test menu function
void testKoreanLang() {
    Trainee *chosen = selectRandomTakers();
    serveRandomQuiz(chosen);
}

// Main menu
void showMenu() {
    char choice;
    do {
        printf("\n=== Millieway Training Menu ===\n");
        printf("1. Korean Quiz\n");
        printf("0. Exit\n");
        printf("Select menu: ");
        scanf(" %c", &choice);
        getchar(); // consume newline
        switch (choice) {
            case '1':
                testKoreanLang();
                break;
            case '0':
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid input.\n");
        }
    } while (choice != '0');
}

// Entry point
int main() {
    srand((unsigned int)time(NULL));
    showMenu();
    return 0;
}
