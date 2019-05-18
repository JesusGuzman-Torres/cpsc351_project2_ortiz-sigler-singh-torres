#include "genericImports.h"
#include "ProcessManager.cpp"

int main() {
    /* Variable Definition */
    ifstream inputFile;
    string fileName;
    int memorySize, pageSize;

    /* Inputs into the system */
    cout << "Memory Size: ";
    cin >> memorySize;
    cout << "Page Size (1: 100, 2: 200, 3:400): ";
    cin >> pageSize;
    pageSize = pageSize == 3 ? 400 : pageSize*100; //Adjust for proper page size
    cout << "Please enter input file: ";
    cin.ignore(); //Stupid special char ignore
    getline(cin, fileName);
    inputFile.open(fileName);
    if (!inputFile.is_open()){
        cout << "Error opening file. Please try again." << endl;
        return 1;
    }

    /* Start the Process Manager & Simulator */
    ProcessManager processManager(inputFile, memorySize, pageSize);

    return 0;
}