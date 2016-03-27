#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

DWORD a[1000];

void fib(LPVOID Param) {
	DWORD number = *((DWORD*) Param);
	a[0] = 0;
	a[1] = 1;
	DWORD i;
	for ( i = 2; i < number; ++i ) {
		a[i] = a[i - 1] + a[i - 2];
	}
}

int main(int argc, char* argv[]) {
	int param = atoi(argv[1]);
	DWORD ThreadId;
	HANDLE ThreadHandle;

	ThreadHandle = CreateThread(NULL, 0, fib, &param, 0, &ThreadId);

	if ( ThreadHandle != NULL ) {
		WaitForSingleObject(ThreadHandle, INFINITE);
		CloseHandle(ThreadHandle);

		int i;
		for ( i = 0; i < param; ++i ) {
			printf("%d ", a[i]);
		}

		printf("\n");
	}

	return 0;
}
