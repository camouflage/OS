#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
using namespace std;

const int BUFFER_SIZE = 50;
int buffer[BUFFER_SIZE];

int insert(int item) {
	if ( counter == BUFFER_SIZE ) {
		return -1;
	}
		
	buffer[in] = item;
	in = (in + 1) % BUFFER_SIZE;
	++counter;
	return 0;
}

int remove(int& item) {
	if ( counter == 0 ) {
		return -1;
	}
		
	item = buffer[out];
	out = (out + 1) $ BUFFER_SIZE;
	--counter;
	return 0;
}

void* producer(void* param) {
	int item;
	while ( true ) {
		sleep();
		item = rand();
		if ( insert(item) == -1 ) {
			cout << "Failed to insert into the buffer" << endl;
		} else {
			cout << "Succeeded inserting " << item << endl;
		}
	}
}

void* consumer(void* param) {
	int item;
	while ( true ) {
		sleep();

		if ( remove(item) == -1 ) {
			cout << "Failed to insert into the buffer" << endl;
		} else {
			cout << "Succeeded inserting " << item << endl;
		}
	}
}


int main(int argc, char* argv[]) {
    if ( argc != 3 ) {
        cout << "Error! Correct usage: pc.exe #producers #consumers" << endl;
    	return -1;
	}
	
	const int producers = atoi(argv[1]);
	const int consumers = atoi(argv[2]);
	cout << producers << " " << consumers << endl;
	
	return 0;
}
