#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ERRORS 100
#define MAX_VARS 100

struct CheckResult {
    int printfCount;
    int scanfCount;
    int functionCount;
    int intCount;
    int floatCount;
    int charCount;
    int doubleCount;
    int boolCount;
    int longCount;
    int bracketErrorLines[MAX_ERRORS];
    char bracketErrorMessages[MAX_ERRORS][MAX_LINE_LENGTH];
    int bracketErrorCount;
};


void analyzeFile(const char *filename);