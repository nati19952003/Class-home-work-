#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CANDIDATES 6
#define STR_LEN 100


struct Candidate {
    char name[STR_LEN];
    char dob[11];
    char gender;
    char email[STR_LEN];
    char nationality[STR_LEN];
    float bmi;
    char primary_skill[STR_LEN];
    char secondary_skill[STR_LEN];
    int topik;
    char mbti[6];
    char intro[STR_LEN];
};


int calculate_age(char *dob) {
    int year, month, day;
    sscanf(dob, "%d/%d/%d", &year, &month, &day);
    int age = 2025 - year;
    if (month > 4 || (month == 4 && day > 9)) {
        age--;
    }
    return age;
}


void get_group_name(char *group_name) {
    printf("Enter the audition group name: ");
    int i = 0;
    char ch;
    while ((ch = getchar()) != '\n' && i < STR_LEN - 1) {
        group_name[i++] = ch;
    }
    group_name[i] = '\0';
}

int main() {
    struct Candidate member_info[MAX_CANDIDATES];
    char group_name[STR_LEN];

    get_group_name(group_name);

    printf("\n####################################\n");
    printf("[%s] Audition Candidate Data Entry\n", group_name);
    printf("####################################\n");

    int count = 0;
    while (count < MAX_CANDIDATES) {
        printf("\nEntering information for candidate #%d:\n", count + 1);
        printf("---------------------------------------\n");

        printf("1. Name: ");
        fgets(member_info[count].name, STR_LEN, stdin);
        member_info[count].name[strcspn(member_info[count].name, "\n")] = '\0';

        printf("2. Date of Birth (YYYY/MM/DD): ");
        fgets(member_info[count].dob, 11, stdin);
        while (getchar() != '\n');  // clear buffer

        printf("3. Gender (M/F): ");
        scanf(" %c", &member_info[count].gender);
        while (getchar() != '\n');

        printf("4. Email: ");
        fgets(member_info[count].email, STR_LEN, stdin);
        member_info[count].email[strcspn(member_info[count].email, "\n")] = '\0';

        printf("5. Nationality: ");
        fgets(member_info[count].nationality, STR_LEN, stdin);
        member_info[count].nationality[strcspn(member_info[count].nationality, "\n")] = '\0';

        printf("6. BMI: ");
        scanf("%f", &member_info[count].bmi);
        while (getchar() != '\n');

        printf("7. Primary Skill: ");
        fgets(member_info[count].primary_skill, STR_LEN, stdin);
        member_info[count].primary_skill[strcspn(member_info[count].primary_skill, "\n")] = '\0';

        printf("8. Secondary Skill: ");
        fgets(member_info[count].secondary_skill, STR_LEN, stdin);
        member_info[count].secondary_skill[strcspn(member_info[count].secondary_skill, "\n")] = '\0';

        printf("9. Korean Proficiency Level (TOPIK): ");
        scanf("%d", &member_info[count].topik);
        while (getchar() != '\n');

        printf("10. MBTI: ");
        fgets(member_info[count].mbti, 6, stdin);
        member_info[count].mbti[strcspn(member_info[count].mbti, "\n")] = '\0';

        printf("11. Introduction: ");
        fgets(member_info[count].intro, STR_LEN, stdin);
        member_info[count].intro[strcspn(member_info[count].intro, "\n")] = '\0';

        count++;
    }

    // Output screen
    printf("\n####################################\n");
    printf("[%s] Audition Candidate Data Review\n", group_name);
    printf("####################################\n");

    printf("===============================================================================================================\n");
    printf("Name                 | Age | DOB       | Gender | Email                 | Nationality   | BMI  | Primary Skill | Secondary Skill       | TOPIK   | MBTI |\n");
    printf("===============================================================================================================\n");

    for (int i = 0; i < MAX_CANDIDATES; i++) {
        int age = calculate_age(member_info[i].dob);
        printf("%-20s (%2d) | %10s |   %c    | %-22s | %-13s | %.1f | %-13s | %-20s | ",
            member_info[i].name,
            age,
            member_info[i].dob,
            member_info[i].gender,
            member_info[i].email,
            member_info[i].nationality,
            member_info[i].bmi,
            member_info[i].primary_skill,
            member_info[i].secondary_skill);

        
        if (member_info[i].topik == 0) printf("Native   ");
        else printf("Level %d  ", member_info[i].topik);

        printf("| %-4s |\n", member_info[i].mbti);
        printf("---------------------------------------------------------------------------------------------\n");
        printf("%s\n", member_info[i].intro);
        printf("---------------------------------------------------------------------------------------------\n");
    }

    return 0;
}
