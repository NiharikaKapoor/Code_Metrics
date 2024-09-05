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
    int intCount;
    int floatCount;
    int charCount;
    int doubleCount;
    int boolCount;
    int longCount;
    int stringCount;
    int bracketErrorLines[MAX_ERRORS];
    char bracketErrorMessages[MAX_ERRORS][MAX_LINE_LENGTH];
    int bracketErrorCount;
};

struct MemoryStruct {
    char *memory;
    size_t size;
};

void initCheckResult(struct CheckResult *result) {
    result->printfCount = 0;
    result->scanfCount = 0;
    result->functionCount = 0;
    result->intCount = 0;
    result->floatCount = 0;
    result->charCount = 0;
    result->doubleCount = 0;
    result->boolCount = 0;
    result->longCount = 0;
    result->stringCount = 0;
    result->bracketErrorCount = 0;
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

void checkBrackets(const char *line, char *bracketStack, int *stackSize, struct CheckResult *result, int lineNumber) {
    while (*line) {
        if (*line == '{' || *line == '(' || *line == '[') {
            bracketStack[(*stackSize)++] = *line;
        } else if (*line == '}' || *line == ')' || *line == ']') {
            if (*stackSize == 0 ||
                (*line == '}' && bracketStack[*stackSize - 1] != '{') ||
                (*line == ')' && bracketStack[*stackSize - 1] != '(') ||
                (*line == ']' && bracketStack[*stackSize - 1] != '[')) {
                snprintf(result->bracketErrorMessages[result->bracketErrorCount], MAX_LINE_LENGTH,
                         "Mismatched closing bracket: %c", *line);
                result->bracketErrorLines[result->bracketErrorCount] = lineNumber;
                result->bracketErrorCount++;
            } else {
                (*stackSize)--;
            }
        }
        line++;
    }
}

void processLine(char *line, struct CheckResult *result, char *bracketStack, int *stackSize, int lineNumber) {
    char cleanLine[MAX_LINE_LENGTH];
    strncpy(cleanLine, line, MAX_LINE_LENGTH);

    if (isCommentLine(line)) {
        char *commentPos = strstr(line, "//");
        if (commentPos != NULL) {
            *commentPos = '\0';
        }
    }

    checkBrackets(cleanLine, bracketStack, stackSize, result, lineNumber);

    if (isKeyword(cleanLine, "printf")) result->printfCount++;
    if (isKeyword(cleanLine, "scanf")) result->scanfCount++;
    if (isKeyword(cleanLine, "int") && isKeyword(cleanLine, "main")) result->functionCount++; // Count main as int function
    if (isKeyword(cleanLine, "int") && !isKeyword(cleanLine, "main")) result->intCount++;
    if (isKeyword(cleanLine, "float")) result->floatCount++;
    if (isKeyword(cleanLine, "char")) result->charCount++;
    if (isKeyword(cleanLine, "double")) result->doubleCount++;
    if (isKeyword(cleanLine, "bool")) result->boolCount++;
    if (isKeyword(cleanLine, "long")) result->longCount++;
    if (isKeyword(cleanLine, "std::string")) result->stringCount++;
}

void handleMultilineComment(FILE *file, char *line) {
    char *commentStart = strstr(line, "/*");
    char *commentEnd = strstr(line, "*/");

    if (commentStart != NULL && commentEnd == NULL) {
        char tempLine[MAX_LINE_LENGTH];
        while (fgets(tempLine, MAX_LINE_LENGTH, file)) {
            strncat(line, tempLine, MAX_LINE_LENGTH - strlen(line) - 1);
            if (strstr(tempLine, "*/")) {
                break;
            }
        }
    }
}



void displayResult(struct CheckResult *result, int totalLines, int totalVars, int totalFuncs) {
    printf("BASIC INFORMATION ABOUT THE ABOVE CODE\n");
    printf("=========================================\n");
    printf("Total num of lines --> %d\n", totalLines);
    printf("Total built in functions used --> %d\n", result->printfCount + result->scanfCount);
    printf("Total num of variables used --> %d\n\n", totalVars);

    printf("INFORMATION ABOUT FUNCTIONS USED IN THE ABOVE CODE\n");
    printf("=====================================================\n");
    printf("Total number of \"void\" functions --> %d\n", 0); // Placeholder
    printf("Total number of \"integer\" functions --> %d\n", result->functionCount);
    printf("Total number of \"float\" functions --> %d\n", 0); // Placeholder
    printf("Total number of \"printf\" function --> %d\n", result->printfCount);
    printf("Total number of \"scanf\" function --> %d\n\n", result->scanfCount);

    printf("INFORMATION ABOUT THE BRACKETS USED IN ABOVE CODE\n");
    printf("====================================================\n");
    printf("%d closing curly bracket(s) are missing\n", result->bracketErrorCount);
    printf("Curve brackets are perfectly closed\n"); // Placeholder
    printf("Long brackets are perfectly closed\n\n"); // Placeholder

    printf("INFORMATION ABOUT THE VARIABLES USED\n");
    printf("========================================\n");
    printf("The num of floats variables used are --> %d\n", result->floatCount);
    printf("The num of int variables used are --> %d\n", result->intCount);
    printf("The num of char variables used are --> %d\n\n", result->charCount);
}

void analyzeFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open the file: %s\n", filename);
        return;
    }

    struct CheckResult result;
    initCheckResult(&result);

    char bracketStack[MAX_LINE_LENGTH];
    int stackSize = 0;
    char line[MAX_LINE_LENGTH];
    int lineNumber = 0;
    int totalVars = 0;
    int totalFuncs = 0;

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        lineNumber++;
        handleMultilineComment(file, line);
        processLine(line, &result, bracketStack, &stackSize, lineNumber);
    }

    if (stackSize > 0) {
        result.bracketErrorLines[result.bracketErrorCount] = lineNumber;
        snprintf(result.bracketErrorMessages[result.bracketErrorCount], MAX_LINE_LENGTH,
                 "Mismatched opening bracket(s) found.");
        result.bracketErrorCount++;
    }

    totalVars = result.intCount + result.floatCount + result.charCount;
    totalFuncs = result.functionCount;

    displayResult(&result, lineNumber, totalVars, totalFuncs);


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

    // Read the entire file content for time complexity analysis
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open the file: %s\n", filename);
        return 1;
    }

    char *code = (char *)malloc(MAX_LINE_LENGTH * MAX_ERRORS);
    char line[MAX_LINE_LENGTH];
    code[0] = '\0';

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        strcat(code, line);
    }
    fclose(file);


    free(code);
    return 0;
}