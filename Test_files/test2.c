#include <stdio.h>

int main() {
    int a = 5;
    int b = 10;
    int sum = a + b;

    printf("Sum: %d\n", sum);

    if (a > b) {
        printf("A is greater than B\n");
    } else {
        printf("B is greater than or equal to A\n");
    }

    for (int i = 0; i < 5; i++) {
        printf("Loop iteration: %d\n", i);
    }

    return 0;
}
