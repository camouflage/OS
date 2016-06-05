#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstdio>
#include<algorithm>
#include<sstream>
#include "windows.h"
#include <ctime>
using namespace std;

int main(int argc, char* argv[]) {
	clock_t begin = clock();
	
	if ( argc != 3 ) {
		cout << "Error! Correct usage: mmSort.exe [0/1] fileName" << endl;
    	return -1;
	}
	
	const int number = 10000;
	int a[number];
	
	if ( atoi(argv[1]) == 0 ) { // Ordinary File IO
		fstream fs(argv[2], fstream::in);
	
		for ( int i = 0; i < number; ++i ) {
			fs >> a[i];
		}
	
		sort(a, a + number);
	} else { // Memory Mapped File
		const int BUF_SIZE = 1024;
		HANDLE hFile, hMapFile;
		LPCTSTR pBuf;
		
		// https://msdn.microsoft.com/en-us/library/windows/desktop/aa363778(v=vs.85).aspx
		hFile = CreateFile(
				TEXT(argv[2]), 	  // open number.txt
	            GENERIC_READ,             // open for reading
	            0,          			  // no sharing
	            NULL,                     // no security
	            OPEN_EXISTING,            // existing file only
	            FILE_ATTRIBUTE_NORMAL,    // normal file
	            NULL);
	    
	    if ( hFile == NULL || hFile == INVALID_HANDLE_VALUE ) {
	    	cout << "Could not open file." << endl; 
	     	return -1;
	  	}
	    
	    // Get system granularity and file size.
	    SYSTEM_INFO sysInfo;
		GetSystemInfo(&sysInfo);
		DWORD granularity = sysInfo.dwAllocationGranularity;
		cout << "Granularity: " << granularity << endl;
		
		DWORD dwFileSize = GetFileSize(hFile,  NULL);
	    cout << "File size: " << dwFileSize << endl;
	    
	    // https://msdn.microsoft.com/en-us/library/windows/desktop/aa366551(v=vs.85).aspx
		hMapFile = CreateFileMapping(
	                 hFile,
	                 NULL,                    // default security
	                 PAGE_READONLY,           // read access
	                 0,                       // maximum object size (high-order DWORD)
	                 dwFileSize,              // maximum object size (low-order DWORD)
	                 "MMnumber"); 
	    
	    if ( hMapFile == NULL ) {
	    	cout << "Could not create file mapping object." << endl;
	      	return -1;
	   	}
	   	
	   	// https://msdn.microsoft.com/en-us/library/windows/desktop/aa366761(v=vs.85).aspx
	   	// Error code: 1132 ERROR_MAPPED_ALIGNMENT
	   	pBuf = (LPTSTR) MapViewOfFile(hMapFile,   // handle to map object
	                        FILE_MAP_READ, 		  // read permission(Error code: 5)
	                        0,
	                        0,
	                        dwFileSize);

	   	if ( pBuf == NULL ) {
			cout << "Could not map view of file. " << GetLastError() << endl;
	       	CloseHandle(hMapFile);
	      	return -1;
	   	}

	   	char* pEnd;
	   	a[0] = strtol(pBuf, &pEnd, 10);
	   	for ( int i = 1; i < number; ++i ) {
	   		a[i] = strtol(pEnd, &pEnd, 10);
	   		//cout << i << " " << a[i] << "; ";
		}
		
		sort(a, a + number);
		
		UnmapViewOfFile(pBuf);
	   	CloseHandle(hMapFile);
	}

	fstream fs("sorted.txt", fstream::out);
	for ( int i = 1; i < number; ++i ) {
	   	fs << a[i] << " ";
	}
	
	clock_t end = clock();
  	cout << "Elapsed time: " <<  double(end - begin) / CLOCKS_PER_SEC << endl;
      
	return 0;
}
