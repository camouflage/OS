#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char* argv[]) {
    int num = atoi(argv[1]);
    int fib0 = 0;
    int fib1 = 1;
    
    int i, temp;
    for ( i = 0; i < num; ++i ) {
        pid_t pid = fork();
        if ( pid == 0 ) {
            printf("%d\n", fib0);
            temp = fib0 + fib1;
            fib0 = fib1;
            fib1 = temp;
        } else {
            wait(NULL);
        }
    }
    return 0;
}
