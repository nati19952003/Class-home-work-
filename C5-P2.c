#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TRAINEES 100
#define MAX_FEEDBACK_LEN 51
#define MAX_MIDI 127

typedef struct {
    char name[50];
    int vocalRange;
    int midiValues[12];
    int midiCount;
    float vocalStability;
    char feedback[MAX_FEEDBACK_LEN];
} Trainee;

Trainee trainees[MAX_TRAINEES];
int traineeCount = 0;

// Generate unique MIDI values
void generateMIDI(int *arr, int count, int maxVal) {
    int used[MAX_MIDI + 1] = {0};
    for (int i = 0; i < count;) {
        int val = rand() % (maxVal + 1);
        if (!used[val]) {
            arr[i++] = val;
            used[val] = 1;
        }
    }
}

// Calculate average diff for stability
float calculateStability(int *arr, int count) {
    float sum = 0;
    for (int i = 1; i < count; i++) {
        sum += abs(arr[i] - arr[i - 1]);
    }
    return sum / (count - 1);
}

// Function pointer used for display
void (*displayTrainee)(Trainee) = NULL;

void printOneTrainee(Trainee t) {
    printf("\nName: %s\n", t.name);
    printf("Vocal Stability: %.2f\n", t.vocalStability);
    printf("Feedback: %s\n", t.feedback);
}

void evalVocal() {
    Trainee t;
    int midiCount;

    printf("Enter trainee name: ");
    scanf(" %[^\n]", t.name);

    printf("Enter highest vocal range (0-127): ");
    scanf("%d", &t.vocalRange);
    if (t.vocalRange < 0 || t.vocalRange > MAX_MIDI) {
        printf("Invalid vocal range!\n");
        return;
    }

    printf("Enter number of MIDI values (6-12): ");
    scanf("%d", &midiCount);
    if (midiCount < 6 || midiCount > 12 || midiCount > t.vocalRange) {
        printf("Invalid MIDI count!\n");
        return;
    }

    t.midiCount = midiCount;
    generateMIDI(t.midiValues, midiCount, t.vocalRange);
    t.vocalStability = calculateStability(t.midiValues, midiCount);

    printf("Provide feedback (max 50 characters): ");
    scanf(" %[^\n]", t.feedback);
    t.feedback[MAX_FEEDBACK_LEN - 1] = '\0';  // truncate if too long

    trainees[traineeCount++] = t;
    printf("Evaluation complete!\n");
}

// Sorting by vocal stability
int compareStability(const void *a, const void *b) {
    float s1 = ((Trainee *)a)->vocalStability;
    float s2 = ((Trainee *)b)->vocalStability;
    return (s1 > s2) - (s1 < s2);
}

void printVocalInfo() {
    if (traineeCount == 0) {
        printf("No evaluations yet.\n");
        return;
    }

    qsort(trainees, traineeCount, sizeof(Trainee), compareStability);
    printf("\n--- Vocal Evaluation Results ---\n");

    for (int i = 0; i < traineeCount; i++) {
        if (displayTrainee)
            displayTrainee(trainees[i]);
    }
}

int main() {
    srand((unsigned)time(NULL));
    displayTrainee = &printOneTrainee;

    int choice;
    while (1) {
        printf("\n[4. Vocal Training > A. Vocal Evaluation]\n");
        printf("1. Evaluate Vocal\n2. Print Vocal Info\n0. Exit\nSelect: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                evalVocal();
                break;
            case 2:
                printVocalInfo();
                break;
            case 0:
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }
}
