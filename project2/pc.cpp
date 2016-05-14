#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
using namespace std;

const int BUFFER_SIZE = 50;
int buffer[BUFFER_SIZE];
int counter;
int in;
int out;
int producerSleep;
int consumerSleep;
int producerRealOp;
int consumerRealOp;
int ProducerTotalOp;
int consumerTotalOp;

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

// Producer: sleep for random amount of time befroe producing
void* producer(void* param) {
	int item;
	while ( true ) {
		int timeToSleep = rand() % 1000;
		producerSleep += timeToSleep;
		Sleep(timeToSleep);

		item = rand();
		if ( insert(item) == -1 ) {
			cout << "Failed to insert into the buffer" << endl;
		} else {
			++producerRealOp;
			cout << "Succeeded inserting " << item << endl;
		}
		++ProducerTotalOp;
	}
}

// Consumer: sleep for random amount of time befroe consuming
void* consumer(void* param) {
	int item;
	while ( true ) {
		int timeToSleep = rand() % 1000;
		consumerSleep += timeToSleep;
		Sleep(timeToSleep);

		if ( remove(item) == -1 ) {
			cout << "Failed to insert into the buffer" << endl;
		} else {
			++consumerRealOp;
			cout << "Succeeded inserting " << item << endl;
		}
		++consumerTotalOp;
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
	const int timeToSleep = 60 * 1000;

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

	// Thread
	DWORD ProduderThreadId[10];
	HANDLE ProducerThreadHandle[10];
	DWORD ConsumerThreadId[10];
	HANDLE ConsumerThreadHandle[10];

	// Create thread
	for ( int i = 0; i < producers; ++i ) {
		ProducerThreadHandle[i] = CreateThread(NULL, 0, producer, NULL, 0, ProduderThreadId[i]);
		cout << "# Successfully created producer thread " << i << endl << endl;
	}
	for ( int i = 0; i < consumers; ++i ) {
		ConsumerThreadHandle[i] = CreateThread(NULL, 0, consumer, NULL, 0, ConsumerThreadId[i]);
		cout << "# Successfully created consumer thread " << i << endl << endl;
	}

	// Wait
	/*
	if ( ProducerThreadHandle[0] != NULL ) {
		WaitForMultipleObjects(producers, ProducerThreadHandle, TRUE, INFINITE);
	}
	if ( ConsumerThreadHandle[0] != NULL ) {
		WaitForMultipleObjects(consumers, ConsumerThreadHandle, TRUE, INFINITE);
	}
	*/
	Sleep(timeToSleep);
	cout << "Complete!" << endl;
	
	return 0;
}
