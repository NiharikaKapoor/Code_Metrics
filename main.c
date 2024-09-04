#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ERRORS 100
#define MAX_VARS 100

struct CheckResult {
    int printfCount;
    int scanfCount;
    int functionCount;
};

void initCheckResult(struct CheckResult *result) {
    result->printfCount = 0;
    result->scanfCount = 0;
    result->functionCount = 0;
}

int isWordBoundary(char c) {
    return !isalnum(c) && c != '_';
}

int isKeyword(const char *line, const char *keyword) {
    char *pos = strstr(line, keyword);
    while (pos != NULL) {
        char before = (pos == line) ? ' ' : *(pos - 1);
        char after = *(pos + strlen(keyword));
        if (isWordBoundary(before) && isWordBoundary(after)) {
            return 1;
        }
        pos = strstr(pos + 1, keyword);
    }
    return 0;
}

int isCommentLine(const char *line) {
    return strstr(line, "//") != NULL;
}

void processLine(char *line, struct CheckResult *result) {
    char cleanLine[MAX_LINE_LENGTH];
    strncpy(cleanLine, line, MAX_LINE_LENGTH);

    if (isCommentLine(line)) {
        char *commentPos = strstr(line, "//");
        if (commentPos != NULL) {
            *commentPos = '\0';
        }
    }

    if (isKeyword(cleanLine, "printf")) result->printfCount++;
    if (isKeyword(cleanLine, "scanf")) result->scanfCount++;
    if (isKeyword(cleanLine, "int") && isKeyword(cleanLine, "main")) result->functionCount++; // Count main as int function
}

void displayResult(struct CheckResult *result, int totalLines, int totalFuncs) {
    printf("BASIC INFORMATION ABOUT THE ABOVE CODE\n");
    printf("=========================================\n");
    printf("Total num of lines --> %d\n", totalLines);
    printf("Total built in functions used --> %d\n", result->printfCount + result->scanfCount);
    printf("Total num of functions used --> %d\n\n", totalFuncs);

    printf("INFORMATION ABOUT FUNCTIONS USED IN THE ABOVE CODE\n");
    printf("=====================================================\n");
    printf("Total number of \"void\" functions --> %d\n", 0); // Placeholder
    printf("Total number of \"integer\" functions --> %d\n", result->functionCount);
    printf("Total number of \"float\" functions --> %d\n", 0); // Placeholder
    printf("Total number of \"printf\" function --> %d\n", result->printfCount);
    printf("Total number of \"scanf\" function --> %d\n\n", result->scanfCount);
}

void analyzeFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open the file: %s\n", filename);
        return;
    }

    struct CheckResult result;
    initCheckResult(&result);

    char line[MAX_LINE_LENGTH];
    int lineNumber = 0;
    int totalFuncs = 0;

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        lineNumber++;
        processLine(line, &result);
    }

    totalFuncs = result.functionCount;

    displayResult(&result, lineNumber, totalFuncs);

    fclose(file);
}

int main(int argc, char *argv[]) {
    char filename[MAX_LINE_LENGTH];
    if (argc > 1) {
        strncpy(filename, argv[1], MAX_LINE_LENGTH);
    } else {
        printf("Enter the filename: ");
        fgets(filename, MAX_LINE_LENGTH, stdin);
        filename[strcspn(filename, "\n")] = '\0';  // Remove the newline character
    }

    analyzeFile(filename);

    return 0;
}
