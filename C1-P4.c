#include <stdio.h>
#include <stdlib.h>

#define MAX_JUDGES 10


struct Judge {
    char name[100];
    char gender[10];
    char affiliation[100];
    char title[100];
    char specialty[100];
    char email[100];
    char phone[30];
};

int main() {
    struct Judge judges[MAX_JUDGES];
    char project[100];
    int totalJudges, selectedMembers;
    char check;

    printf("####################################\n");
    printf("#      Judge List Data Entry      #\n");
    printf("####################################\n");

    printf("Participating Project: ");
    fgets(project, sizeof(project), stdin);

    printf("Total Number of Judges: ");
    scanf("%d", &totalJudges);
    getchar(); // To consume newline character

    printf("Number of Selected Members: ");
    scanf("%d", &selectedMembers);
    getchar(); // To consume newline

    printf("++++++++++++++++++++++++++++++++++++\n");
    printf("Starting to input information for %d judges.\n", totalJudges);
    printf("++++++++++++++++++++++++++++++++++++\n");

    for (int i = 0; i < totalJudges; i++) {
        printf("\nJudge %d:\n", i + 1);

        printf("Name: ");
        fgets(judges[i].name, sizeof(judges[i].name), stdin);

        printf("Gender: ");
        fgets(judges[i].gender, sizeof(judges[i].gender), stdin);

        printf("Affiliation: ");
        fgets(judges[i].affiliation, sizeof(judges[i].affiliation), stdin);

        printf("Title: ");
        fgets(judges[i].title, sizeof(judges[i].title), stdin);

        printf("Specialty: ");
        fgets(judges[i].specialty, sizeof(judges[i].specialty), stdin);

        printf("Email: ");
        fgets(judges[i].email, sizeof(judges[i].email), stdin);

        printf("Phone: ");
        fgets(judges[i].phone, sizeof(judges[i].phone), stdin);
    }

    printf("++++++++++++++++++++++++++++++++++++\n");
    printf("Judge information entry is complete.\n");
    printf("++++++++++++++++++++++++++++++++++++\n");

    printf("Should we check the judge information? (Y/N): ");
    scanf(" %c", &check);

    if (check == 'Y') {
        printf("\n####################################\n");
        printf("#        Display Judge Data        #\n");
        printf("####################################\n");

        for (int i = 0; i < totalJudges; i++) {
            printf("[Judge %d]\n", i + 1);
            printf("Name: %s", judges[i].name);
            printf("Gender: %s", judges[i].gender);
            printf("Affiliation: %s", judges[i].affiliation);
            printf("Title: %s", judges[i].title);
            printf("Expertise: %s", judges[i].specialty);
            printf("Email: %s", judges[i].email);
            printf("Phone: %s", judges[i].phone);
            printf("-----------------------------------\n");
        }
    } else {
        printf("Program terminated.\n");
    }

    return 0;
}
