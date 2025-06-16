#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PATTERN 6
#define MAX_PATTERN_NAME 32
#define MAX_PATTERN_DESC 256
#define MAX_SONG 4
#define MAX_SONG_TITLE 64
#define MAX_SONG_PATTERNS 16
#define MAX_NODE 32

// --- Data Structures ---

typedef struct {
    char name[MAX_PATTERN_NAME];
    char desc[MAX_PATTERN_DESC];
} DancePattern;

typedef struct PatternNode {
    char pattern_name[MAX_PATTERN_NAME];
    struct PatternNode *next;
} PatternNode;

typedef struct {
    char title[MAX_SONG_TITLE];
    PatternNode *head;
} SongPatternTree;

// --- Utility Functions ---

// Random int in [0, n)
int random_int(int n) {
    return rand() % n;
}
void strip_newline(char *s) {
    char *p = strchr(s, '\n');
    if (p) *p = 0;
}
// Remove all spaces from src and copy to dest
void remove_spaces(const char *src, char *dest) {
    while (*src) {
        if (*src != ' ') {
            *dest++ = *src;
        }
        ++src;
    }
    *dest = '\0';
}
// Lowercase string compare for pattern names
int stricmp(const char *a, const char *b) {
    while (*a && *b) {
        char ca = *a, cb = *b;
        if (ca >= 'A' && ca <= 'Z') ca += 'a' - 'A';
        if (cb >= 'A' && cb <= 'Z') cb += 'a' - 'A';
        if (ca != cb) return 1;
        ++a; ++b;
    }
    return *a || *b;
}
// Clear screen
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// --- File IO ---

// Load patterns from dance_pattern.txt
// Format: PatternName|Description
int load_patterns(const char *filename, DancePattern patterns[MAX_PATTERN]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Cannot open %s\n", filename);
        return 0;
    }
    char line[512];
    int cnt = 0;
    while (fgets(line, sizeof(line), fp) && cnt < MAX_PATTERN) {
        strip_newline(line);
        char *name = strtok(line, "|");
        char *desc = strtok(NULL, "|");
        if (!name || !desc) continue;
        strncpy(patterns[cnt].name, name, MAX_PATTERN_NAME);
        strncpy(patterns[cnt].desc, desc, MAX_PATTERN_DESC);
        cnt++;
    }
    fclose(fp);
    return cnt;
}

// Load K-POP song choreography pattern report as a tree
// analyz_dance-pattern.csv format: SongTitle,Pattern1,Pattern2,...
int load_song_patterns(const char *filename, SongPatternTree song_trees[MAX_SONG]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: Cannot open %s\n", filename);
        return 0;
    }
    char line[512];
    int cnt = 0;
    while (fgets(line, sizeof(line), fp) && cnt < MAX_SONG) {
        strip_newline(line);
        char *tok = strtok(line, ",");
        if (!tok) continue;
        strncpy(song_trees[cnt].title, tok, MAX_SONG_TITLE);
        PatternNode *head = NULL, *tail = NULL;
        int pat_cnt = 0;
        while ((tok = strtok(NULL, ",")) != NULL && pat_cnt < MAX_SONG_PATTERNS) {
            PatternNode *node = (PatternNode*)malloc(sizeof(PatternNode));
            strncpy(node->pattern_name, tok, MAX_PATTERN_NAME);
            node->next = NULL;
            if (!head) head = tail = node;
            else { tail->next = node; tail = node; }
            pat_cnt++;
        }
        song_trees[cnt].head = head;
        cnt++;
    }
    fclose(fp);
    return cnt;
}
void free_song_trees(SongPatternTree *trees, int n) {
    for (int i = 0; i < n; ++i) {
        PatternNode *cur = trees[i].head;
        while (cur) {
            PatternNode *tmp = cur;
            cur = cur->next;
            free(tmp);
        }
        trees[i].head = NULL;
    }
}

// --- Core Logic ---

// Pick a random pattern description, show a 10-char substring (no spaces), ask for pattern name
int ask_pattern_question(DancePattern patterns[MAX_PATTERN], int pattern_count, int *used) {
    int idx;
    do {
        idx = random_int(pattern_count);
    } while (used[idx]);
    used[idx] = 1;

    // Remove spaces for random substring
    char desc_nospace[MAX_PATTERN_DESC] = {0};
    remove_spaces(patterns[idx].desc, desc_nospace);
    int len = strlen(desc_nospace);
    int start = (len > 10) ? random_int(len - 9) : 0;
    printf("\nPattern description: %s\n", patterns[idx].desc);
    printf("Hint (random substring): [");
    for (int i = 0; i < 10 && start + i < len; ++i)
        putchar(desc_nospace[start + i]);
    printf("]\n");
    printf("Enter the pattern name: ");
    char input[64];
    if (!fgets(input, sizeof(input), stdin)) return 0;
    strip_newline(input);

    if (stricmp(input, patterns[idx].name) == 0) {
        printf("Correct!\n");
        return 1;
    } else {
        printf("Wrong. Correct answer: %s\n", patterns[idx].name);
        return 0;
    }
}

// Traverse and print all song trees
void traverse_song_trees(SongPatternTree *trees, int song_count) {
    for (int i = 0; i < song_count; ++i) {
        printf("\nSong: %s\nPatterns: ", trees[i].title);
        PatternNode *cur = trees[i].head;
        int first = 1;
        while (cur) {
            if (!first) printf(" -> ");
            printf("%s", cur->pattern_name);
            first = 0;
            cur = cur->next;
        }
        printf("\n");
    }
}

// Bonus: Let user play sequence for one selected song
void song_pattern_quiz(SongPatternTree *trees, int song_count) {
    printf("\nAvailable Songs:\n");
    for (int i = 0; i < song_count; ++i) {
        printf("  %d. %s\n", i + 1, trees[i].title);
    }
    printf("Select a song (1-%d): ", song_count);
    char input[16];
    if (!fgets(input, sizeof(input), stdin)) return;
    int sel = atoi(input);
    if (sel < 1 || sel > song_count) {
        printf("Invalid selection.\n");
        return;
    }
    PatternNode *cur = trees[sel - 1].head;
    if (!cur) {
        printf("No patterns in song.\n");
        return;
    }
    printf("\nFirst pattern: %s\n", cur->pattern_name);
    cur = cur->next;
    while (cur) {
        printf("Enter next pattern: ");
        if (!fgets(input, sizeof(input), stdin)) return;
        strip_newline(input);
        if (stricmp(input, cur->pattern_name) == 0) {
            printf("Correct!\n");
            cur = cur->next;
        } else {
            printf("Wrong! The correct answer was: %s\n", cur->pattern_name);
            printf("Returning to previous menu.\n");
            return;
        }
    }
    printf("Congratulations! You completed the song pattern sequence!\n");
}

// --- Main Entry Point for Menu ---
void learnDancePattern() {
    srand((unsigned int)time(NULL));
    DancePattern patterns[MAX_PATTERN];
    int pattern_count = load_patterns("dance_pattern.txt", patterns);
    if (pattern_count != MAX_PATTERN) {
        printf("Error: Unable to load all choreography patterns. (%d loaded)\n", pattern_count);
        return;
    }

    printf("\n=== Choreography Pattern Learning ===\n");
    int correct = 0, used[MAX_PATTERN] = {0};
    int round = 0;
    for (; round < 4; ++round) {
        if (ask_pattern_question(patterns, pattern_count, used)) {
            ++correct;
        }
        // If fewer than 3 correct after 4 rounds, exit
        if (round == 3 && correct < 3) {
            printf("\nYou answered fewer than 3 correctly. Returning to previous menu.\n");
            return;
        }
    }

    // If 3 or more correct, load song trees and print
    SongPatternTree trees[MAX_SONG];
    int song_count = load_song_patterns("analyz_dance-pattern.csv", trees);
    if (song_count != MAX_SONG) {
        printf("Error: Unable to load song analysis report. (%d loaded)\n", song_count);
        return;
    }
    traverse_song_trees(trees, song_count);

    // Bonus: Let user try a song pattern quiz
    printf("\nBonus: Try to name all patterns for a selected song in order!\n");
    song_pattern_quiz(trees, song_count);

    free_song_trees(trees, song_count);
    clear_screen();
    printf("All song trees have been displayed. Screen cleared.\n");
}
