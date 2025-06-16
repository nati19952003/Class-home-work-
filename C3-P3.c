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
int isEasterEgg(char input[], const char *keywo*
