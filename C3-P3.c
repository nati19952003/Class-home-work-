#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BIN_LEN 8
#define MAX_INPUT 20

// Struct for Arthur's trauma
typedef struct {
    char keyword[10];
    char message[300];
} ArthurTrauma;

// Function to convert a character to 8-bit binary
void charToBinary(char c, char *binary) {
    for (int i = 7; i >= 0; i--) {
        binary[7 - i] = (c & (1 << i)) ? '1' : '0';
    }
    binary[8] = '\0';
}

// Reverse the binary string
void reverseString(char *s) {
    int len = strlen(s);
    for (int i = 0; i < len / 2; i++) {
        char tmp = s[i];
        s[i] = s[len - 1 - i];
        s[len - 1 - i] = tmp;
    }
}

// Verify user entered the correct characters
int isRightChar(char reversedBinary[][9], char userInput[]) {
    for (int i = 0; i < 7; i++) {
        char expectedChar = 0;
        for (int j = 0; j < 8; j++) {
            expectedChar = (expectedChar << 1) | (reversedBinary[i][j] - '0');
        }
        if (userInput[i] != expectedChar)
            return 0;
    }
    return 1;
}

// Compare final word with keyword
int isEasterEgg(char input[], const char *keyword) {
    return strcmp(input, keyword) == 0;
}

// Easter egg trigger
void find_easter_egg() {
    ArthurTrauma arthur = {
        "specter",
        "I confess. After graduating from university, I was blinded by the arrogance of starting a startup and recklessly blocked my friends' paths. I painfully learned that when I am the only one convinced by my idea, it leads to disastrous results. The past Arthur was a ghost of dogmatism and stubbornness."
    };

    char reversedBinary[7][9];  // 7 characters, 8-bit binary + null
    printf("<<Arthur's Easter Egg>>\n");

    // Convert each char in "specter" to binary and reverse
    for (int i = 0; i < 7; i++) {
        charToBinary(arthur.keyword[i], reversedBinary[i]);
        reverseString(reversedBinary[i]);
        printf("Char %d binary (reversed): %s\n", i + 1, reversedBinary[i]);
    }

    printf("\nEnter the characters that match these reversed binary values (7 letters): ");
    char input[10];
    scanf("%s", input);

    if (isRightChar(reversedBinary, input)) {
        printf("\nGood job! Now, enter the word you formed: ");
        char word[MAX_INPUT];
        scanf("%s", word);
        if (isEasterEgg(word, arthur.keyword)) {
            printf("\n##Easter Egg Discovered!$$\n%s\n", arthur.message);
        } else {
            printf("Incorrect keyword. Returning to menu...\n");
        }
    } else {
        printf("Incorrect character entry. Try again.\n");
    }
}

// Bonus: scramble and process keyword
void bonus_scramble_and_binary(const char *keyword) {
    int len = strlen(keyword);
    char odd[10] = "", even[10] = "";
    char binary[9];

    // Collect odd-indexed chars (1-based)
    for (int i = 0; i < len; i++) {
        if ((i + 1) % 2 == 1) strncat(odd, &keyword[i], 1);
        else strncat(even, &keyword[i], 1);
    }

    printf("\n[Bonus] Odd-indexed letters: %s\n", odd);
    printf("[Bonus] Even-indexed letters: %s\n", even);

    printf("\nBinary representation (reversed) for each character:\n");
    for (int i = 0; i < strlen(odd); i++) {
        charToBinary(odd[i], binary);
        reverseString(binary);
        printf("%c -> %s\n", odd[i], binary);
    }
    for (int i = 0; i < strlen(even); i++) {
        charToBinary(even[i], binary);
        reverseString(binary);
        printf("%c -> %s\n", even[i], binary);
    }
}

// Example menu
void selfManagementMenu() {
    char input[20];
    printf("\n[II. Training > 2. Self-Management and Teamwork]\n");
    printf("Enter your choice or type 'Arthur' to reveal the Easter egg: ");
    scanf("%s", input);

    if (strcmp(input, "Arthur") == 0 || strcmp(input, "arthur") == 0) {
        find_easter_egg();
    } else {
        printf("Regular self-management feature: %s\n", input);
    }
}

int main() {
    srand(time(NULL)); // For future randomness

    // Example call
    selfManagementMenu();

    // Bonus scramble (can comment out if not testing)
    bonus_scramble_and_binary("specter");

    return 0;
}
