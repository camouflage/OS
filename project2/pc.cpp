#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <psapi.h>
// Requires linker: -lpsapi

using namespace std;

// Global variables for buffer info.
const int BUFFER_SIZE = 50;
int buffer[BUFFER_SIZE];
int counter;
int in;
int out;

// Global variables for performance evaluation.
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

// size of KB
const int KB = 1024; 

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


// Initiate variables
void init() {
	counter = 0;
	in = 0;
	out = 0;
	producerRealOp = 0;
	producerTotalOp = 0;
	consumerRealOp = 0;
	consumerTotalOp = 0;
} 

// Ref: http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
void displayPerformance() {
	// Output buffer.
	cout << "Complete!" << endl;
	cout << counter << " elements remained in buffer: " << endl;
	for ( int i = 0; i < counter; ++i ) {
		cout << buffer[i] << " ";
	}
	cout << endl << endl;
	
	
	// Output # of operations perfromed.
	cout << "===========Operations Performed==========" << endl
		 << "# of operations performed by producers: " << producerRealOp << endl
		 << "# of operations tried by producers: " << producerTotalOp << endl
		 << "# of operations performed by consumers: " << consumerRealOp << endl
		 << "# of operations tried by consumers: " << consumerTotalOp << endl << endl;
	
	
	// Calculate memory usage.
	
	// Total virtual memory
	MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
    // Virtual memory being used
    DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
    // Virtual Memory used by current process
    PROCESS_MEMORY_COUNTERS_EX pmc;
    // Error solved by Ref: https://social.msdn.microsoft.com/Forums/en-US/720198c4-04a2-4737-9159-6e23a217d6b7/question-about-getprocessmemoryinfo?forum=Vsexpressvc 
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
    
    cout << "==========Virtual Memory Usage==========" << endl
		 << "There are " << totalVirtualMem / KB << " KBytes of virtual memory" << endl
		 << "Among which " << virtualMemUsed / KB << " KBytes are being used" << endl
		 << "And " << virtualMemUsedByMe / KB << " KBytes are used by current process" << endl
		 << "Usage rate is: " << virtualMemUsedByMe * 100.0 / virtualMemUsed << "%" << endl << endl;
		 
	// Total physical memory
	DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
    // Physical memory being used
    DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
    // Physical Memory used by current process
    SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
    
    cout << "==========Physical Memory Usage==========" << endl
		 << "There are " << totalPhysMem / KB << " KBytes of physical memory" << endl
		 << "Among which " << physMemUsed / KB << " KBytes are being used" << endl
		 << "And " << physMemUsedByMe / KB << " KBytes are used by current process" << endl
		 << "Usage rate is: " << physMemUsedByMe * 100.0 / physMemUsed << "%" << endl << endl;
}


int main(int argc, char* argv[]) {
    if ( argc != 5 ) {
        cout << "Error! Correct usage: pc.exe [#producers] [#consumers] [timeToSleep] [mode]" << endl;
    	return -1;
	}
	
	const int producers = atoi(argv[1]);
	const int consumers = atoi(argv[2]);
	const int timeToSleep = atoi(argv[3]);
	const int mode = atoi(argv[4]);
	
	// Deal with invalid input.
	if ( producers > 10 || producers <= 0 ) {
		cout << "Wrong number of producers, range: [1, 10]" << endl;
		return -1;
	}
	if ( consumers > 10 || consumers <= 0 ) {
		cout << "Wrong number of consumers, range: [1, 10]" << endl;
		return -1;
	}
	if ( timeToSleep < 1 || timeToSleep > 60 ) {
		cout << "Wrong waiting time, range: [1, 60] seconds" << endl;
		return -1;
	}
	if ( mode < 0 || mode > 2 ) {
		cout << "Wrong mode, mode should be 0, 1 or 2" << endl;
		return -1;
	}
	
	/* Only for test
	const int producers = 1;
	const int consumers = 1;
	const int timeToSleep = 10;
	const int mode = 2;
	*/
	
	init();
	
	// Thread info
	DWORD ProduderThreadId[10];
	HANDLE ProducerThreadHandle[10];
	DWORD ConsumerThreadId[10];
	HANDLE ConsumerThreadHandle[10];

	// Create thread according to the mode
	LPTHREAD_START_ROUTINE producer = producer;
	LPTHREAD_START_ROUTINE consumer = consumer;
	if ( mode == 0 ) { // mode0: Sleep for random amount of time
		producer = producer0;
		consumer = consumer0;
	} else if ( mode == 1 ) { // mode1: Peterson's solution
		producer = producer1;
		consumer = consumer1;
		
		// Init
		memset(flag, 0, sizeof(flag));
		turn = -1;
	} else if ( mode == 2 ) { // mode2: Using Mutex locks and Semaphores
		producer = producer2;
		consumer = consumer2;
		
		// Init
		Mutex = CreateMutex(NULL, FALSE, NULL);
		Full = CreateSemaphore(NULL, 0, BUFFER_SIZE, NULL);
		Empty = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL); 
	}
	
	// Number of producers and consumers created up till now.
	int pcount = 0;
	int ccount = 0;
	while ( 1 ) {
		int who = rand() % 2;
		// Randomly create producer or consumer thread.
		if ( who == 0 && pcount < producers ) {
			ProducerThreadHandle[pcount] = CreateThread(NULL, 0, producer, NULL, 0, &ProduderThreadId[pcount]);
			cout << "# Successfully created producer thread " << pcount << endl;
			++pcount;
		} else if ( who == 1 && ccount < consumers ) {
			ConsumerThreadHandle[ccount] = CreateThread(NULL, 0, consumer, NULL, 0, &ConsumerThreadId[ccount]);
			cout << "# Successfully created consumer thread " << ccount << endl;
			++ccount;
		} else if ( pcount == producers && ccount == consumers ) {
			break;
		} else if ( pcount == producers ) {
			for ( ; ccount < consumers; ++ccount ) {
				ConsumerThreadHandle[ccount] = CreateThread(NULL, 0, consumer, NULL, 0, &ConsumerThreadId[ccount]);
				cout << "# Successfully created consumer thread " << ccount << endl;
			}
		} else if ( ccount == consumers ) {
			for ( ; pcount < producers; ++pcount ) {
				ProducerThreadHandle[pcount] = CreateThread(NULL, 0, producer, NULL, 0, &ProduderThreadId[pcount]);
				cout << "# Successfully created producer thread " << pcount << endl;
			}
		}
	}
	
	// Sleep
	cout << endl << "Mode " << mode << ": Sleep for " << timeToSleep << " seconds" << endl << endl;
	Sleep(timeToSleep * 1000);
	
	// Output performance info
	displayPerformance();
	cout << "====================END====================" << endl << endl;
	
	return 0;
}

