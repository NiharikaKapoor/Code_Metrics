#include <stdio.h>

int main() {
    int a = 5;
    float b = 3.14;
    char c = 'x';
    
    printf("Hello, World!\n");
    scanf("%d", &a);
    
    if (a > 0) {
        printf("Positive number\n");
    }
    
    return 0;
}