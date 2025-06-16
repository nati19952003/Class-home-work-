#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUM_NOTES 7

// Note names for indexing
const char NOTE_NAMES[NUM_NOTES] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};

// Structure representing a musical note
typedef struct {
    char name;   // 'A'~'G'
    int octave;  // 1~8
} Note;

// Structure representing a chord (root, third, fifth)
typedef struct {
    Note root;
    Note third;
    Note fifth;
} Chord;

// Helper to find index of note ('A'~'G'), returns -1 if invalid
int note_index(char note) {
    for (int i = 0; i < NUM_NOTES; ++i) {
        if (NOTE_NAMES[i] == note)
            return i;
    }
    return -1;
}

// Helper: pointer arithmetic: returns 1 if two notes are the same (name and octave)
int is_same_note(Note *a, Note *b) {
    return (a->name == b->name && a->octave == b->octave);
}

// Checks if all notes are in the same octave
int check_octave_match(Chord *chord) {
    int octave = chord->root.octave;
    return (chord->third.octave == octave && chord->fifth.octave == octave);
}

// Checks that all notes are different (no duplication)
int check_no_duplicate(Chord *chord) {
    Note *notes[3] = {&chord->root, &chord->third, &chord->fifth};
    for (int i = 0; i < 3; ++i) {
        for (int j = i + 1; j < 3; ++j) {
            if (is_same_note(notes[i], notes[j]))
                return 0;
        }
    }
    return 1;
}

// Compute pitch interval (in semitones) between two notes of the same octave
// For simplicity, we treat each note as one step (ignoring sharps/flats), so A=0, B=1, ..., G=6
int pitch_interval(Note *a, Note *b) {
    int idx1 = note_index(a->name);
    int idx2 = note_index(b->name);
    if (idx1 < 0 || idx2 < 0) return -1;
    int interval = idx2 - idx1;
    if (interval < 0) interval += NUM_NOTES; // wrap around
    return interval;
}

// Function pointer type for chord property checks
typedef int (*ChordCheckFunc)(Chord *);

// Harmony check: interval between root and third must be "small" (2 or 3 steps, like major/minor third)
// interval between root and fifth must be "large" (4 steps, like perfect fifth)
int check_harmony(Chord *chord) {
    int r3 = pitch_interval(&chord->root, &chord->third);
    int r5 = pitch_interval(&chord->root, &chord->fifth);
    // Major/minor third (2 or 3 steps), perfect fifth (4 steps)
    if ((r3 == 2 || r3 == 3) && (r5 == 4))
        return 1;
    return 0;
}

// Print chord info
void print_chord(const Chord *chord) {
    printf("Chord:\n");
    printf("  Root : %c%d\n", chord->root.name, chord->root.octave);
    printf("  Third: %c%d\n", chord->third.name, chord->third.octave);
    printf("  Fifth: %c%d\n", chord->fifth.name, chord->fifth.octave);
}

// Input note from user
int input_note(const char *label, Note *note) {
    char buf[16];
    printf("Enter %s note (A-G): ", label);
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    char noteName = toupper(buf[0]);
    if (note_index(noteName) == -1) {
        printf("Invalid note name.\n");
        return 0;
    }
    printf("Enter %s octave (1-8): ", label);
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    int octave = atoi(buf);
    if (octave < 1 || octave > 8) {
        printf("Invalid octave.\n");
        return 0;
    }
    note->name = noteName;
    note->octave = octave;
    return 1;
}

// Main function for music theory learning (menu action)
void learnMusicTheory() {
    Chord chord;
    printf("\n=== Music Theory: Chord Builder ===\n");
    if (!input_note("Root", &chord.root)) return;
    if (!input_note("Third", &chord.third)) return;
    if (!input_note("Fifth", &chord.fifth)) return;

    print_chord(&chord);

    // Use function pointers for validation checks
    ChordCheckFunc checks[2] = {check_octave_match, check_no_duplicate};
    const char *checkMsgs[2] = {
        "All notes are in the same octave.",
        "No duplicate notes."
    };
    int valid = 1;
    for (int i = 0; i < 2; ++i) {
        if (!checks[i](&chord)) {
            printf("Chord invalid: %s check failed.\n", checkMsgs[i]);
            valid = 0;
        } else {
            printf("Chord valid: %s\n", checkMsgs[i]);
        }
    }

    // Bonus: Harmony check
    if (valid) {
        if (check_harmony(&chord)) {
            printf("The chord is harmonious!\n");
        } else {
            printf("The chord is NOT harmonious (check intervals).\n");
        }
    } else {
        printf("Chord validation failed. Please try again.\n");
    }
    printf("=== End Music Theory ===\n");
}

