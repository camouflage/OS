#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>

int value = 5;

int main() {
    pid_t pid;
    pid = fork();

    if ( pid == 0 ) {
        value += 15;
        printf("Child value = %d\n", value);

        pid_t pid2;
        pid2 = fork();
        if ( pid2 == 0 ) {
            value += 30;
            printf("Another child value = %d\n", value);
        } else if ( pid2 > 0 ) {
            // wait(NULL);
            printf("Another parent value = %d\n", value);
        }

    } else if ( pid > 0 ) {
        /* If there is wait(NULL), child process will be executed first, then parent process.
         * But "value" in parent process does not change even though there is "value += 15" in child process.
         * When wait(NULL) is commented out, parent process will come first.
         */
        wait(NULL);
        printf("Parent value = %d\n", value);
    }

    return 0;
}
