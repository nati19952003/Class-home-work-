#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_CANDIDATES 6
#define NUM_CATEGORIES 5

// Candidate Info: Name + Unique 6-digit ID
char *candidateNames[NUM_CANDIDATES] = {
    "Jiyeon Park", "Ethan Smith", "Helena Silva",
    "Liam Wilson", "Minji Lee", "Noah Brown"
};

int candidateIDs[NUM_CANDIDATES] = {
    102345, 102346, 102347, 102348, 102349, 102350
};

// Score categories
char *categories[NUM_CATEGORIES] = {
    "Music Proficiency", "Dance", "Vocal", "Visual", "Expression"
};

// Stores scores per candidate: [ID, cat1, cat2, cat3, cat4, cat5, total]
int scores[NUM_CANDIDATES][7];

// Function to get validated score (between 10 and 100)
int getValidatedScore(char *category) {
    int score;
    while (1) {
        printf("%s: ", category);
        scanf("%d", &score);
        if (score >= 10 && score <= 100) return score;
        printf("Invalid score. Please enter a value between 10 and 100.\n");
    }
}

// Input judge and score data
void enterScores() {
    char judgeName[50], expertise[50];

    printf("\n####################################\n");
    printf("#     Audition Evaluation Entry    #\n");
    printf("####################################\n");
    printf("> Judge Name: ");
    scanf(" %[^\n]", judgeName);
    printf("> Expertise: ");
    scanf(" %[^\n]", expertise);
    printf("++++++++++++++++++++++++++++++++++++\n");

    for (int i = 0; i < NUM_CANDIDATES; i++) {
        printf("Candidate: %s\n", candidateNames[i]);
        scores[i][0] = candidateIDs[i];
        int total = 0;
        for (int j = 0; j < NUM_CATEGORIES; j++) {
            scores[i][j + 1] = getValidatedScore(categories[j]);
            total += scores[i][j + 1];
        }
        scores[i][6] = total;
        printf("------------------------------------\n");
    }
}

// Review scores and submit
int reviewAndSubmit() {
    char choice;
    printf("\n++++++++++++++++++++++++++++++++++++\n");
    printf("Submission completed.\nPlease review your input!\n");
    printf("------------------------------------\n");
    for (int i = 0; i < NUM_CANDIDATES; i++) {
        printf("%s: %d\n", candidateNames[i], scores[i][1]); // You can sum/display more
    }

    printf("Would you like to submit? (Y/N): ");
    scanf(" %c", &choice);

    return (choice == 'Y' || choice == 'y');
}

void finalSubmission() {
    printf("*** Final submission completed. ***\n");

    // Bonus: Sorting and final selection
    // Bubble sort based on total score (7th column)
    for (int i = 0; i < NUM_CANDIDATES - 1; i++) {
        for (int j = i + 1; j < NUM_CANDIDATES; j++) {
            if (scores[i][6] < scores[j][6]) {
                int temp[7];
                memcpy(temp, scores[i], sizeof(int) * 7);
                memcpy(scores[i], scores[j], sizeof(int) * 7);
                memcpy(scores[j], temp, sizeof(int) * 7);

                // Swap names too
                char *tempName = candidateNames[i];
                candidateNames[i] = candidateNames[j];
                candidateNames[j] = tempName;
            }
        }
    }

    printf("=======================================\n");
    printf("Compiling final selection results...\n");
    printf("=======================================\n");
    printf("###########################################\n");
    printf("# Congratulations! Welcome to Milliways!  #\n");
    printf("###########################################\n");

    for (int i = 0; i < 4; i++) {
        printf("%d. %s\n", i + 1, candidateNames[i]);
    }
}

int main() {
    while (1) {
        enterScores();
        if (reviewAndSubmit()) {
            finalSubmission();
            break;
        } else {
            printf("Restarting score entry process...\n");
        }
    }

    return 0;
}
