#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TRAINEES 4
#define MAX_QUESTIONS 10
#define QUIZ_SIZE 5

typedef struct {
    int id;
    char question[100];
    char answer[100];
} KoreanQuiz;

typedef struct {
    char name[50];
    char nickname[30];
    char nationality[30];
    int score;
    char pass;
} Trainee;

typedef struct {
    int qid;
    char result; // 'O' or 'X'
} QuizAnswer;

// Question bank (10 questions)
KoreanQuiz questionBank[MAX_QUESTIONS] = {
    {1, "Hello", "안녕하세요"},
    {2, "Thank you", "감사합니다"},
    {3, "Sorry", "죄송합니다"},
    {4, "Yes", "네"},
    {5, "No", "아니요"},
    {6, "Delicious", "맛있어요"},
    {7, "Where is the bathroom?", "화장실 어디에요?"},
    {8, "I don’t know", "모르겠어요"},
    {9, "Help me", "도와주세요"},
    {10, "Goodbye", "안녕히 가세요"}
};

// Final Millieway trainees (1 Korean + 3 others)
Trainee trainees[MAX_TRAINEES] = {
    {"Park Ji-yeon", "Jiyeon", "Korean", 0, 'X'},
    {"Liam Chen", "Liam", "American", 0, 'X'},
    {"Sakura Tanaka", "Sakura", "Japanese", 0, 'X'
