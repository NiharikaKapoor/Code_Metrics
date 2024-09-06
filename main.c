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
// Callback function for handling the response from the API
size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        fprintf(stderr, "Not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void escapeJsonString(char *escaped, const char *input) {
    while (*input) {
        switch (*input) {
            case '"': strcat(escaped, "\\\""); break;
            case '\\': strcat(escaped, "\\\\"); break;
            case '\b': strcat(escaped, "\\b"); break;
            case '\f': strcat(escaped, "\\f"); break;
            case '\n': strcat(escaped, "\\n"); break;
            case '\r': strcat(escaped, "\\r"); break;
            case '\t': strcat(escaped, "\\t"); break;
            default:
                strncat(escaped, input, 1);
                break;
        }
        input++;
    }
}

void extractComplexitiesFromApiResponse(const char *apiResponse, char *extractedText) {
    // Locate the start of the "text": " field
    const char *startText = strstr(apiResponse, "\"text\": \"");

    if (startText != NULL) {
        startText += 9; // Move past the "\"text\": \"" part
        const char *endText = strstr(startText, "\""); // Find the closing quote for the "text" value

        if (endText != NULL) {
            size_t length = endText - startText;
            strncpy(extractedText, startText, length-2); // Copy the extracted content
            extractedText[length] = '\0'; // Null-terminate the extracted text
        } else {
            strcpy(extractedText, "Text end quote not found");
        }
    } else {
        strcpy(extractedText, "Text field not found");
    }
}


void getComplexities(const char *code) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    struct MemoryStruct chunk = {0};
    char escapedCode[8192] = "";  // Buffer for the escaped code

    // Retrieve the API key from the environment variable
    const char *api_key = getenv("GEMINI_API_KEY");
    if (!api_key) {
        fprintf(stderr, "Error: GEMINI_API_KEY environment variable not set.\n");
        return;
    }

    // Escape the code to be JSON safe
    escapeJsonString(escapedCode, code);

    // Construct the JSON payload
    char postData[8192];
    snprintf(postData, sizeof(postData),
        "{\"contents\": [{\"role\": \"user\", \"parts\": [{\"text\": \"Analyze the time and space complexity of the following code and give me just the time and space complexity of it without any explanation in the same line separated by comma in format Time Complexity - __ , Space Complexity - __: %s\"}]}],"
        "\"generationConfig\": {\"temperature\": 1, \"topK\": 64, \"topP\": 0.95, \"maxOutputTokens\": 8192, \"responseMimeType\": \"text/plain\"}}",
        escapedCode);

    curl = curl_easy_init();
    if(curl) {
        // Set the URL for the API request
        char url[512];
        snprintf(url, sizeof(url),
            "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-pro:generateContent?key=%s",
            api_key);
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set the headers for the API request
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);

        // Setup write callback to capture response
        chunk.memory = malloc(1);
        chunk.size = 0;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // Perform the request
        res = curl_easy_perform(curl);
        //printf("%s\n", chunk.memory);
        // Check for errors
        char extractedText[200] = "";
        extractComplexitiesFromApiResponse(chunk.memory, extractedText);

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("\n%s \n", extractedText); // Print the API response
        }

        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        free(chunk.memory);
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
    printf("The num of double variables used are --> %d\n\n", result->doubleCount);
    printf("The num of bool variables used are --> %d\n\n", result->boolCount);
    printf("The num of long variables used are --> %d\n\n", result->longCount);
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
    // Fetch and display the time complexity
    getComplexities(code);

    free(code);
    return 0;
}