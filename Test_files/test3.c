#include <stdio.h>
#include <stdbool.h>

int factorial(int n) {
    if (n == 0 || n == 1) return 1;
    return n * factorial(n - 1);
}

int main() {
    int num = 5;
    long result = factorial(num);

    printf("Factorial of %d is %ld\n", num, result);

    bool isEven = (num % 2 == 0);
    if (isEven) {
        printf("%d is even\n", num);
    } else {
        printf("%d is odd\n", num);
    }

    return 0;
}
