#include "./headers/complexity.h"
#include "./headers/metrics.h"

void analyzeCode(const char *filename) {
    analyzeFile(filename);

    // Load code contents from file
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file: %s\n", filename);
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *code = (char *)malloc(fileSize + 1);
    fread(code, 1, fileSize, file);
    code[fileSize] = '\0';
    fclose(file);

    getComplexities(code);

    free(code);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    analyzeCode(filename);

    return 0;
}