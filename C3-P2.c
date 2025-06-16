// stage2.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MEMBERS 8
#define MAX_QUESTIONS 5
#define MAX_RESPONSES 3
#define MAX_RESPONSE_LEN 101

typedef struct {
    char nickname[50];
    int age;
    char trauma[200];
    int hasTrauma;
} Member;

typedef struct {
    i
