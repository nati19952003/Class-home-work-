#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for sleep function

#define WIDTH 100 // Width for centering the text

// Function to print spaces for centering text
void print_spaces(int count) {
    for (int i = 0; i < count; i++) {
        printf(" ");
    }
}

// Function to display the splash screen with decorations
void display_splash_screen(const char *name, const char *date) {
    system("clear"); // Clear the screen (use "cls" for Windows)
    
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    
    // Print left-aligned right-angled triangle
    for (int i = 1; i <= 5; i++) {
        for (int j = 0; j < i; j++) {
            printf("*");
        }
        print_spaces(WIDTH - 10); // Adjust width spacing
        for (int j = 0; j < (6 - i); j++) {
            printf("*");
        }
        printf("\n");
    }
    
    // Print centered splash screen text
    print_spaces(20); printf("[Magrathea ver 0.1]\n");
    print_spaces(10); printf("Magrathea, where a shining planet is created in a wasteland with no grass,\n");
    print_spaces(5); printf("a place where unseen potential is discovered and gems are polished by the hands of experts,\n");
    print_spaces(25); printf("Welcome to Magrathea.\n");
    
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("[User]: %s                                   [Execution Time]: %s\n", name, date);
    printf("=============================================================================================================\n");
}

int main() {
    char name[50];
    char date[11];
    
    printf("[Please enter the current date in the \"yyyy-mm-dd\" format]: ");
    scanf("%10s", date);
    
    printf("[Please enter your name]: ");
    scanf("%49s", name);
    
    printf("**The input has been processed successfully.**\n");
    
    sleep(3); // Wait for 3 seconds before clearing the screen
    display_splash_screen(name, date);
    
    return 0;
}
