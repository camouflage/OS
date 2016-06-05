#include<iostream>
#include<fstream>
#include<cstdlib>
using namespace std;

int main() {
	const int number = 10000;
	fstream fs("number.txt", fstream::out);
	for ( int i = 0; i < number; ++i ) {
		fs << rand() << " ";
	}
} 
