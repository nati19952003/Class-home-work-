#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_DANCE 6
#define MAX_STEP 16
#define MAX_STEP_LEN 32
#define MAX_NAME_LEN 32
#define MAX_MEMBER 10
#define MAX_NICKNAME 32

// ----- Data structures -----

typedef struct {
    char kor_name[MAX_NAME_LEN];
    char eng_name[MAX_NAME_LEN];
    int step_count;
    char steps[MAX_STEP][MAX_STEP_LEN];
} Dance;

typedef struct {
    char name[MAX_NAME_LEN];
    char nickname[MAX_NICKNAME];
    int dance_score;
} Member;

// Example: You should load actual members from your database/system.
Member group_members[MAX_MEMBER] = {
    {"Ariel", "ariel", 0},
    {"Arthur", "arthur", 0},
    {"Milly", "milly", 0},
    {"Cody", "cody", 0},
    {"Sam", "sam", 0},
    {"Jin", "jin", 0}
};
// For simplicity, MAX_MEMBER = 6 here. Adjust as needed.

// ----- Utility Functions -----

// Generates a random integer in [0, n)
int random_int(int n) {
    return rand() % n;
}

// Strips trailing newline from fgets
void strip_newline(char *s) {
    char *p = strchr(s, '\n');
    if (p) *p = 0;
}

// Fisher-Yates shuffle for steps
void shuffle_steps(char arr[MAX_STEP][MAX_STEP_LEN], int n) {
    for (int i = n - 1; i > 0; --i) {
        int j = random_int(i + 1);
        char tmp[MAX_STEP_LEN];
        strcpy(tmp, arr[i]);
        strcpy(arr[i], arr[j]);
        strcpy(arr[j], tmp);
    }
}

// Loads dance steps from file into dances[]
// File format per dance: Korean_name|English_name|step1,step2,step3,...
// e.g.:
// 왈츠|Waltz|Step1,Step2,Step3,Step4
int load_dance_steps(const char *filename, Dance dances[MAX_DANCE]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Cannot open %s\n", filename);
        return 0;
    }
    int cnt = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp) && cnt < MAX_DANCE) {
        strip_newline(line);
        char *kor = strtok(line, "|");
        char *eng = strtok(NULL, "|");
        char *stepstr = strtok(NULL, "|");
        if (!kor || !eng || !stepstr) continue;
        strncpy(dances[cnt].kor_name, kor, MAX_NAME_LEN);
        strncpy(dances[cnt].eng_name, eng, MAX_NAME_LEN);
        dances[cnt].step_count = 0;
        char *s = strtok(stepstr, ",");
        while (s && dances[cnt].step_count < MAX_STEP) {
            strncpy(dances[cnt].steps[dances[cnt].step_count++], s, MAX_STEP_LEN);
            s = strtok(NULL, ",");
        }
        cnt++;
    }
    fclose(fp);
    return cnt;
}

// Find member by nickname, returns pointer or NULL
Member* find_member_by_nick(const char *nickname) {
    for (int i = 0; i < MAX_MEMBER; ++i) {
        if (strcmp(group_members[i].nickname, nickname) == 0)
            return &group_members[i];
    }
    return NULL;
}

// Case-insensitive string compare
int stricmp(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 1;
        ++a; ++b;
    }
    return *a || *b;
}

// ----- Main dance learning logic -----

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Show the correct dance and steps for 10 seconds, then erase (bonus)
void show_dance_steps_for_seconds(const Dance *d, int seconds) {
    printf("Memorize this dance!\n");
    printf("Dance: %s (%s)\n", d->kor_name, d->eng_name);
    printf("Steps: ");
    for (int i = 0; i < d->step_count; ++i) {
        printf("%s", d->steps[i]);
        if (i < d->step_count - 1) printf(" -> ");
    }
    printf("\n");
    fflush(stdout);

    // Wait for N seconds
    time_t start = time(NULL);
    while (time(NULL) - start < seconds) {
        // Just wait
    }
    clear_screen();
}

// Check input steps against real steps, return score (and print feedback)
int evaluate_steps(const Dance *d, char user_steps[MAX_STEP][MAX_STEP_LEN], int user_count) {
    int right_order = 1, all_right = 1, any_right = 0;

    if (user_count != d->step_count) right_order = 0;

    // Check for correct steps and order
    for (int i = 0; i < d->step_count; ++i) {
        if (user_count <= i || stricmp(d->steps[i], user_steps[i]) != 0)
            right_order = 0;
    }

    // Check if all the steps are present (regardless of order)
    int matched[MAX_STEP] = {0};

    for (int i = 0; i < d->step_count; ++i) {
        int found = 0;
        for (int j = 0; j < user_count; ++j) {
            if (stricmp(d->steps[i], user_steps[j]) == 0) {
                found = 1;
                matched[i] = 1;
                break;
            }
        }
        if (!found) all_right = 0;
    }

    // Check if any step is correct
    for (int i = 0; i < d->step_count; ++i) {
        if (matched[i]) any_right = 1;
    }

    if (right_order && all_right) return 100;
    if (all_right) return 50;
    if (any_right) return 20;
    return 0;
}

// Main function
void learnDanceStep() {
    srand((unsigned int)time(NULL));
    Dance dances[MAX_DANCE];
    int dance_count = load_dance_steps("dance_step.txt", dances);
    if (dance_count != MAX_DANCE) {
        printf("Error: Could not load all dance steps! (%d loaded)\n", dance_count);
        return;
    }

    char nickname[MAX_NICKNAME];
    int found = 0;
    for (int tries = 0; tries < 3; ++tries) {
        printf("Enter your Milliways nickname: ");
        if (!fgets(nickname, sizeof(nickname), stdin)) return;
        strip_newline(nickname);
        Member *learner = find_member_by_nick(nickname);
        if (learner) {
            found = 1;
            printf("Welcome, %s!\n", learner->name);
            // Pick a random dance
            int didx = random_int(dance_count);
            Dance *d = &dances[didx];

            // Bonus: Show correct steps for 10 seconds and erase
            show_dance_steps_for_seconds(d, 10);

            // Shuffle steps for quiz
            char shuffled_steps[MAX_STEP][MAX_STEP_LEN];
            for (int i = 0; i < d->step_count; ++i)
                strcpy(shuffled_steps[i], d->steps[i]);
            shuffle_steps(shuffled_steps, d->step_count);

            printf("Dance selected: %s (%s)\n", d->kor_name, d->eng_name);
            printf("Shuffled steps: ");
            for (int i = 0; i < d->step_count; ++i) {
                printf("%s", shuffled_steps[i]);
                if (i < d->step_count - 1) printf(" / ");
            }
            printf("\nPlease enter the steps in order, separated by commas:\n");

            char user_input[256];
            if (!fgets(user_input, sizeof(user_input), stdin)) return;
            strip_newline(user_input);

            char user_steps[MAX_STEP][MAX_STEP_LEN];
            int user_count = 0;
            char *tok = strtok(user_input, ",");
            while (tok && user_count < MAX_STEP) {
                // Remove leading/trailing spaces
                while (isspace((unsigned char)*tok)) tok++;
                char *end = tok + strlen(tok) - 1;
                while (end > tok && isspace((unsigned char)*end)) *end-- = 0;
                strncpy(user_steps[user_count++], tok, MAX_STEP_LEN);
                tok = strtok(NULL, ",");
            }

            int score = evaluate_steps(d, user_steps, user_count);

            printf("\nCorrect steps: ");
            for (int i = 0; i < d->step_count; ++i) {
                printf("%s", d->steps[i]);
                if (i < d->step_count - 1) printf(" -> ");
            }
            printf("\nYour input: ");
            for (int i = 0; i < user_count; ++i) {
                printf("%s", user_steps[i]);
                if (i < user_count - 1) printf(" -> ");
            }
            printf("\nScore: %d\n", score);

            learner->dance_score = score;
            printf("Your current basic dance learning score: %d\n", learner->dance_score);

            return;
        } else {
            printf("Nickname not found. Please try again.\n");
        }
    }
    printf("Too many failed attempts. Returning to the previous menu.\n");
}
