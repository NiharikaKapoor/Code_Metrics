#include "./headers/complexity.h"

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
