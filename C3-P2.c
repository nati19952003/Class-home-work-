// stage2.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MEMBERS 8
#define MAX_QUESTIONS 5
#define MAX_RESPONSES 3
#define MAX_RESPONSE_LEN 101

typedef struct {
    char nickname[50];
    int age;
    char trauma[200];
    int hasTrauma;
} Member;

typedef struct {
    int id;
    char content[150];
} Question;

typedef struct {
    char nickname[50];
    char question[150];
    char response[MAX_RESPONSE_LEN];
} Counseling;

Member members[MAX_MEMBERS] = {
    {"Arthur", 22, "", 0},
    {"Bella", 20, "", 0},
    {"Cecil", 23, "", 0},
    {"Dana", 21, "", 0},
    {"Eli", 19, "", 0},
    {"Faye", 22, "", 0},
    {"Gus", 20, "", 0},
    {"Hana", 21, "", 0}
};

const Question questions[MAX_QUESTIONS] = {
    {1, "In what situations have you experienced this trauma?"},
    {2, "How has this situation affected your daily life and emotions?"},
    {3, "How have you tried to overcome this trauma?"},
    {4, "What emotions do you associate with this trauma?"},
    {5, "What kind of support do you think is necessary to overcome this trauma?"}
};

Counseling counselingData[MAX_MEMBERS * MAX_RESPONSES];
int counselingCount = 0;

// Find member index by nickname
int findMemberIndex(const char *nickname) {
    for (int i = 0; i < MAX_MEMBERS; i++) {
        if (strcmp(members[i].nickname, nickname) == 0) {
            return i;
        }
    }
    return -1;
}

// Trauma input menu
void inputTrauma() {
    char name[50];
    char traumaDesc[200];
    int index;

    while (1) {
        printf("\nEnter member nickname (or type 'exit' to go back): ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';
        if (strcmp(name, "exit") == 0) break;

        index = findMemberIndex(name);
        (index != -1) ? printf("Enter trauma description: ") : printf("Nickname not found. Try again.\n");

        if (index != -1) {
            fgets(traumaDesc, sizeof(traumaDesc), stdin);
            traumaDesc[strcspn(traumaDesc, "\n")] = '\0';
            strcpy(members[index].trauma, traumaDesc);
            members[index].hasTrauma = 1;
            printf("Trauma saved for %s.\n", name);
        }
    }
}

// Get random unique questions
void getRandomQuestions(int out[], int count) {
    int used[MAX_QUESTIONS] = {0}, chosen = 0, r;

    srand((unsigned int) time(NULL));
    while (chosen < count) {
        r = rand() % MAX_QUESTIONS;
        if (!used[r]) {
            used[r] = 1;
            out[chosen++] = r;
        }
    }
}

// Validate response
int isValidResponse(const char *response) {
    return strlen(response) > 0 && strlen(response) < MAX_RESPONSE_LEN;
}

// Start counseling session
void startCounseling() {
    char name[50];
    int index;
    int selected[3];

    printf("\nMembers with trauma:\n");
    for (int i = 0; i < MAX_MEMBERS; i++) {
        if (members[i].hasTrauma)
            printf("- %s\n", members[i].nickname);
    }

    printf("Enter member nickname to start session: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    index = findMemberIndex(name);
    if (index == -1 || !members[index].hasTrauma) {
        printf("Invalid selection or no trauma data.\n");
        return;
    }

    getRandomQuestions(selected, 3);

    for (int i = 0; i < 3; i++) {
        printf("\nQ%d: %s\n", i + 1, questions[selected[i]].content);
        printf("Your response (max 100 chars): ");

        char buffer[MAX_RESPONSE_LEN];
        while (1) {
            fgets(buffer, MAX_RESPONSE_LEN, stdin);
            buffer[strcspn(buffer, "\n")] = '\0';
            if (isValidResponse(buffer)) break;
            printf("Invalid response. Please re-enter: ");
        }

        strcpy(counselingData[counselingCount].nickname, name);
        strcpy(counselingData[counselingCount].question, questions[selected[i]].content);
        strcpy(counselingData[counselingCount].response, buffer);
        counselingCount++;
    }

    printf("\nCounseling session completed for %s.\n", name);
}

// View completed sessions
void viewCounselingResults() {
    char name[50];
    printf("\nCompleted counseling members:\n");
    for (int i = 0; i < counselingCount; i++) {
        if (i == 0 || strcmp(counselingData[i].nickname, counselingData[i - 1].nickname) != 0)
            printf("- %s\n", counselingData[i].nickname);
    }

    printf("Enter nickname to view details: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    int index = findMemberIndex(name);
    if (index == -1 || !members[index].hasTrauma) {
        printf("Invalid nickname or no trauma data.\n");
        return;
    }

    printf("\n--- Counseling Report for %s ---\n", name);
    printf("Age: %d\n", members[index].age);
    printf("Trauma: %s\n", members[index].trauma);

    for (int i = 0; i < counselingCount; i++) {
        if (strcmp(counselingData[i].nickname, name) == 0) {
            printf("Q: %s\nA: %s\n", counselingData[i].question, counselingData[i].response);
        }
    }
}

// Trauma menu system
void overcomeTrauma() {
    char choice[10];

    while (1) {
        printf("\n[II. Training > 2. Self-Management and Teamwork > B. Trauma Management]\n");
        printf("1. Input Trauma\n2. Start Counseling Session\n3. View Counseling Results\n4. Back to Main Menu\n");
        printf("Select an option: ");
        fgets(choice, sizeof(choice), stdin);

        switch (choice[0]) {
            case '1': inputTrauma(); break;
            case '2': startCounseling(); break;
            case '3': viewCounselingResults(); break;
            case '4': return;
            default: printf("Invalid option. Try again.\n");
        }
    }
}

// Main menu (simplified)
int main() {
    char choice[10];
    while (1) {
        printf("\nMain Menu\n");
        printf("A. Mentoring\nB. Trauma Management\nQ. Quit\n");
        printf("Select an option: ");
        fgets(choice, sizeof(choice), stdin);

        switch (choice[0]) {
            case 'A':
            case 'a':
                printf("Mentoring not implemented here.\n");
                break;
            case 'B':
            case 'b':
                overcomeTrauma();
                break;
            case 'Q':
            case 'q':
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid option. Try again.\n");
        }
    }
}
