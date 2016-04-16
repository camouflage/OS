#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <windows.h>
using namespace std;
 
const int commandLength = 100;
const int BUFSIZE = 4096;
#define ENVVAR TEXT("Path")

void execute(char* command) {
	STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    
    // Assume that the file is in the same directory.
	cout << "--- Executing command: " << command << " ---\n";
    if ( !CreateProcess(NULL, command,
         NULL, NULL, FALSE, 0, NULL, NULL,
         &si, &pi) ) {
    	/*
    	// GetEnvironmentVariable function
    	// Ref: https://msdn.microsoft.com/en-us/library/windows/desktop/ms683188(v=vs.85).aspx
    	// Ref: https://msdn.microsoft.com/en-us/library/windows/desktop/ms682009(v=vs.85).aspx
    	LPSTR envVarPath;
    	envVarPath = (LPTSTR) malloc(BUFSIZE * sizeof(TCHAR));
    	if ( envVarPath == NULL ) {
    		cout << "Failed to get environment variables!" << endl
    		     << "Create Process Failed! Please check that the executable "
			     << command << " exists!" << endl; 
			     return;
		}
    	
    	DWORD number = GetEnvironmentVariable(ENVVAR, envVarPath, BUFSIZE);
    	
    	// STRTOK Ref: http://www.cplusplus.com/reference/cstring/strtok/ 
    	if ( number > 0 ) {
    		char* pch;
    		pch = strtok(envVarPath, ";");
    		while ( pch != NULL ) {
    			char extendedName[commandLength];
    			strcpy(extendedName, pch);
    			// Concatenate \ to the end of the path if it does not exist.
    			if ( extendedName[strlen(extendedName) - 1] != '\\' ) {
    				strcat(extendedName, "\\");
    			}
				strcat(extendedName, command);
    			cout << "--- We have tried: " << extendedName << " ---" << endl;
    			
    			if ( !CreateProcess(NULL, extendedName,
    				 NULL, NULL, FALSE, 0, NULL, NULL,
    				 &si, &pi) ) {
    				 	cout << "Failed at current location!" << endl;
    			} else {
    				cout << "Succeeded executing!" << endl;
    				
    				WaitForSingleObject(pi.hProcess, INFINITE);

					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);
					break;
				}
						  	
				pch = strtok(NULL, ";");
    			
			}
		}
		
		free(envVarPath);
		*/
    	
    	cout << "Create Process Failed! Please check that the executable "
			 << command << " exists!" << endl; 
             
	} else {
		WaitForSingleObject(pi.hProcess, INFINITE);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
    
    cout << "---------------------------------------------------------------------------\n\n";
}


int main(int argc, char* argv[]) {
    if ( argc != 2 ) {
        cout << "Error! Correct usage: minShell.exe fileName" << endl;
    } else {
        int pathLen = strlen(argv[1]);
        // Check extension
        if ( strncmp(argv[1] + pathLen - 4, ".exe", 4) == 0 ) {
        	execute(argv[1]);
		} else {
            // Open that file
            ifstream ifs;
            ifs.open(argv[1], ios::in);

            // Fail to open the file
            if ( !ifs.is_open() ) {
                cout << "Error! Failed to open the file " << argv[1] << endl;
            } else {
                char line[commandLength];
                while ( !ifs.eof() ) {
                    ifs.getline(line, commandLength);
                    execute(line);
                }

            }
        }
    }

    return 0;
}

