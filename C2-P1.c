#include <stdio.h>
#include <string.h>
#include <ctype.h> // for toupper()
#include <stdlib.h>

#define NUM_TRAINING 8

// Main menu options
char *mainMenu[3] = {
    "Audition Management",
    "Training",
    "Debut"
};

// Training stages
char *trainingStages[NUM_TRAINING] = {
    "1. Physical Strength & Knowledge",
    "2. Self-Management & Teamwork",
    "3. Language & Pronunciation",
    "4. Vocal",
    "5. Dance",
    "6. Visual & Image",
    "7. Acting & Stage Performance",
    "8. Fan Communication"
};

// 'N' = Not done, 'P' = Passed, 'F' = Failed
char trainingStatus[NUM_TRAINING] = {'N','N','N','N','N','N','N','N'};

// Show the main menu
void showMainMenu() {
    printf("\n=== Main Menu ===\n");
    for (int i = 0; i < 3; i++) {
        printf("%d. %s\n", i + 1, mainMenu[i]);
    }
    printf("Enter your choice (0 or Q/q to quit): ");
}

// Show the training menu
void showTrainingMenu() {
    int choice;
    while (1) {
        printf("\n=== Training Menu ===\n");
        for (int i = 0; i < NUM_TRAINING; i++) {
            printf("%d. %s [Status: %c]\n", i + 1, trainingStages[i], trainingStatus[i]);
        }

        printf("Choose a training stage (0 to go back): ");
        scanf("%d", &choice);

        if (choice == 0) {
            return; // go back to main menu
        }

        if (choice < 1 || choice > NUM_TRAINING) {
            printf("Invalid choice. Try again.\n");
            continue;
        }

        // Rules: Stage 1 and 2 must be passed first
        if (choice > 2 && (trainingStatus[0] != 'P' || trainingStatus[1] != 'P')) {
            printf("You must pass stages 1 and 2 before doing this one.\n");
            continue;
        }

        // Already passed
        if (trainingStatus[choice - 1] == 'P') {
            printf("You already passed this stage.\n");
            continue;
        }

        // Ask for evaluation input
        char answer;
        printf("Would you like to enter the evaluation result? (Y/N): ");
        scanf(" %c", &answer);
        answer = toupper(answer);

        if (answer == 'Y') {
            char result;
            printf("Did you pass the training? (P/F): ");
            scanf(" %c", &result);
            result = toupper(result);

            if (result == 'P' || result == 'F') {
                trainingStatus[choice - 1] = result;
                printf("Result recorded.\n");
            } else {
                printf("Invalid input. Must be P or F.\n");
            }
        } else {
            printf("Returning to training menu.\n");
        }
    }
}

// Main program loop
int main() {
    char input[10];

    while (1) {
        showMainMenu();
        scanf("%s", input);

        // Quit if user enters 0, Q, or q
        if (strcmp(input, "0") == 0 || strcmp(input, "Q") == 0 || strcmp(input, "q") == 0) {
            printf("Exiting the program.\n");
            break;
        }

        int menuChoice = atoi(input);

        if (menuChoice == 1) {
            printf("Audition Management is not available yet.\n");
        } else if (menuChoice == 2) {
            showTrainingMenu();
        } else if (menuChoice == 3) {
            printf("Debut is not available yet.\n");
        } else {
            printf("Invalid option. Try again.\n");
        }
    }

    return 0;
}
