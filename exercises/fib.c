#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int num = atoi(argv[1]);
    int fib0 = 0;
    int fib1 = 1;
    int i;
    for ( i = 0; i < num; ++i ) {
        printf("%d\n", fib0);
        int temp;
        temp = fib0 + fib1;
        fib0 = fib1;
        fib1 = temp;
    }

    return 0;
}