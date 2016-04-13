#include <iostream>
#include <fstream>
#include <cstring>
//#include <windows.h>
using namespace std;
 
const int commandLength = 100;

void execute(char* command) {
    // Assume that the file is in the same directory.
    if ( strchr(command, '\\') == NULL ) {
        
    } else {
        
    }
}

int main(int argc, char* argv[]) {
    if ( argc != 2 ) {
        cout << "Error! Correct usage: minShell.exe fileName" << endl;
    } else {
        int pathLen = strlen(argv[1]);
        // Check extension
        if ( strncmp(argv[1] + pathLen - 4, ".txt", 4) == 0 ) {
            // Open that file
            ifstream ifs;
            ifs.open(argv[1], ios::in);

            // Fail to open the file
            if ( !ifs.is_open() ) {
                cout << "Error! Fail to open the file " << argv[1]
                     << endl << "Please make sure that the file exists!" << endl;
            } else {
                char line[commandLength];
                while ( !ifs.eof() ) {
                    ifs.getline(line, commandLength);
                    cout << line << endl;
                }

            }
        }
    }

    return 0;
}
