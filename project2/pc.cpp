#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
using namespace std;

// Global variables for buffer info.
const int BUFFER_SIZE = 50;
int buffer[BUFFER_SIZE];
int counter;
int in;
int out;

// Global variables for performance evaluation.
int producerSleep;
int consumerSleep;
int producerRealOp;
int consumerRealOp;
int producerTotalOp;
int consumerTotalOp;

// Global variables for Peterson's solution.
// 0 stands for producer while 1 stands for consumer
bool flag[2];
int turn;

// Global variables for Mutex locks and Semaphores
HANDLE Mutex;
HANDLE Empty;
HANDLE Full;


// Insert function
int insert(int item) {
	if ( counter == BUFFER_SIZE ) {
		return -1;
	}
		
	buffer[in] = item;
	in = (in + 1) % BUFFER_SIZE;
	++counter;
	return 0;
}

// Remove function
int remove(int& item) {
	if ( counter == 0 ) {
		return -1;
	}
		
	item = buffer[out];
	out = (out + 1) % BUFFER_SIZE;
	--counter;
	return 0;
}

// Insert function in critical section
inline void insertCriticalSection() {
	int item = rand();
	if ( insert(item) == -1 ) {
		//cout << "Failed to insert into the buffer" << endl;
	} else {
		++producerRealOp;
		//cout << "Succeeded inserting " << item << endl;
	}
	
	++producerTotalOp;
}

// Remove function in critical section
inline void removeCriticalSection() {
	int item;
	if ( remove(item) == -1 ) {
		//cout << "Failed to consume the buffer" << endl;
	} else {
		++consumerRealOp;
		//cout << "Succeeded consuming " << item << endl;
	}
	
	++consumerTotalOp;
}


// Producer0: sleep for random amount of time befroe producing
DWORD WINAPI producer0(LPVOID param) {
	while ( true ) {
		int timeToSleep = rand() % 1000;
		producerSleep += timeToSleep;
		Sleep(timeToSleep);

		insertCriticalSection();
	}
}

// Producer1: Peterson's solution.
DWORD WINAPI producer1(LPVOID param) {
	while ( true ) {
		flag[0] = true;
		turn = 1;
		while ( flag[1] && turn == 1 );
		
		insertCriticalSection();
		
		flag[0] = false;
	}
}

// Producer2: Using Mutex locks and Semaphores.
DWORD WINAPI producer2(LPVOID param) {
	while ( true ) {
		WaitForSingleObject(Empty, INFINITE);
		WaitForSingleObject(Mutex, INFINITE);
		
		insertCriticalSection();
		
		ReleaseMutex(Mutex);
		ReleaseSemaphore(Full, 1, NULL);
	}
}


// Consumer0: sleep for random amount of time befroe consuming
DWORD WINAPI consumer0(LPVOID param) {
	while ( true ) {
		int timeToSleep = rand() % 1000;
		consumerSleep += timeToSleep;
		Sleep(timeToSleep);

		removeCriticalSection();
	}
}

// Consumer1: Peterson's solution
DWORD WINAPI consumer1(LPVOID param) {
	while ( true ) {
		flag[1] = true;
		turn = 0;
		while ( flag[0] && turn == 0 );
		
		removeCriticalSection();
		
		flag[1] = false;
	}
}

// Consumer2: Using Mutex locks and Semaphores.
DWORD WINAPI consumer2(LPVOID param) {
	while ( true ) {
		WaitForSingleObject(Full, INFINITE);
		WaitForSingleObject(Mutex, INFINITE);
		
		removeCriticalSection();
		
		ReleaseMutex(Mutex);
		ReleaseSemaphore(Empty, 1, NULL);
	}
}


int main(int argc, char* argv[]) {
	/*
    if ( argc != 3 ) {
        cout << "Error! Correct usage: pc.exe #producers #consumers" << endl;
    	return -1;
	}
	
	const int producers = atoi(argv[1]);
	const int consumers = atoi(argv[2]);
	//cout << producers << " " << consumers << endl;
	*/
	const int producers = 1;
	const int consumers = 1;
	const int timeToSleep = 10;

	// Init
	counter = 0;
	in = 0;
	out = 0;
	producerSleep = 0;
	consumerSleep = 0;
	producerRealOp = 0;
	producerTotalOp = 0;
	consumerRealOp = 0;
	consumerTotalOp = 0;
	
	memset(flag, 0, sizeof(flag));
	turn = -1;
	
	Mutex = CreateMutex(NULL, FALSE, NULL);
	Full = CreateSemaphore(NULL, 0, BUFFER_SIZE, NULL);
	Empty = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL); 

	// Thread
	DWORD ProduderThreadId[10];
	HANDLE ProducerThreadHandle[10];
	DWORD ConsumerThreadId[10];
	HANDLE ConsumerThreadHandle[10];

	// Create thread
	for ( int i = 0; i < producers; ++i ) {
		ProducerThreadHandle[i] = CreateThread(NULL, 0, producer2, NULL, 0, &ProduderThreadId[i]);
		cout << "# Successfully created producer thread " << i << endl;
	}
	
	for ( int i = 0; i < consumers; ++i ) {
		ConsumerThreadHandle[i] = CreateThread(NULL, 0, consumer2, NULL, 0, &ConsumerThreadId[i]);
		cout << "# Successfully created consumer thread " << i << endl;
	}
	
	// Sleep
	cout << endl << "Sleep for " << timeToSleep << " seconds" << endl << endl;
	Sleep(timeToSleep * 1000);
	
	// Output buffer
	cout << "Complete!" << endl;
	cout << counter << " elements remained in buffer: " << endl;
	for ( int i = 0; i < counter; ++i ) {
		cout << buffer[i] << " ";
	}
	cout << endl;
	
	// Output performance info.
	cout << "# of operations performed by producers: " << producerRealOp << endl
		 << "# of operations tried by producers: " << producerTotalOp << endl
		 << "# of operations performed by consumers: " << consumerRealOp << endl
		 << "# of operations tried by consumers: " << consumerTotalOp << endl;
	
	return 0;
}

