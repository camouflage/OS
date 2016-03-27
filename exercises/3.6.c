#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {
    int num = atoi(argv[1]);
    pid_t pid = fork();
    if ( pid < 0 ) {
        printf("Fork Failed\n");
        return -1;
    } else if ( pid == 0 ) {
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
    } else {
        wait(NULL);
    }
    return 0;
}
