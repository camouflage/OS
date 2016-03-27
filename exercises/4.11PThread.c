#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int a[1000];
struct paramsStruct {
    int num;
    int valid;
};

void* fib(void* param) {
    a[0] = 0;
    a[1] = 1;
    int num = ((struct paramsStruct*) param)->num;

    for ( int i = 2; i < num; ++i ) {
        a[i] = a[i - 1] + a[i - 2];
    }

    int* ret = (int*) malloc(sizeof(int));
    *ret = a[num - 1];

    pthread_exit(ret);
}

int main(int argc, char* argv[]) {
    pthread_t tid;

    if ( argc != 2 ) {
        printf("Error! Usage: ./a.out <integer>\n");
        return -1;
    }

    int num = atoi(argv[1]);
    if ( num < 0 ) {
        printf("Error! Int value should be >= 0\n");
        return -1;
    }

    struct paramsStruct ps;
    ps.num = num;
    pthread_create(&tid, NULL, fib, (void*) &ps);

    void* vptr_ret;
    pthread_join(tid, &vptr_ret);
    int ret = *((int*) vptr_ret);
    free(vptr_ret);

    printf("Result: %d\n", ret);

    for ( int i = 0; i < num; ++i ) {
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}
