//
// Created by Xander on 5/17/2019.
//
#include "genericImports.h"
#include "process.h"

/**
 * This process handles all of the memory. It first divides memory into the number of pages and uses an int
 * array based on the PID to determine who owns what. What a process is attempted to be added it sums up all
 * of the free spots (0) and sees if it is large enough to handle that process. If it has enough you add it and
 * return true or return false.
 */
class MemoryManager{
private:
    int memorySize,numberOfPages,pageSize;
    int *pages;
public:

    ~MemoryManager(){
        delete[] pages;
    }

    MemoryManager(int memorySize, int pageSize){
        this->memorySize = memorySize;
        this->pageSize = pageSize;
        this->numberOfPages = memorySize/pageSize;
        this->pages = new int[numberOfPages]();
    }

    /**
     * If it can be added, put the corresponding process's pID into the slot.
     *
     * @param process
     * @return Whether or not there is enough memory to add process to memory
     */
    bool addProcess(Process process){
        /* Needed to check to see if there are enough pages open. */
        int numOfPagesNeeded = ceil(process.getMemory()/((double)pageSize));
        if (numOfPagesNeeded > numOfFreePages())
            return false;

        /* Assign the pages to the open slots in memory */
        for (int i = 0; i < numberOfPages; i++){
            if (pages[i] == 0 && numOfPagesNeeded != 0) {
                pages[i] = process.getPId();
                numOfPagesNeeded--;
            }
        }
        cout << "    " << "MM moves Process " << process.getPId() << " to memory" << endl;

        return true;
    }

    /**
     * Remove the process from memory (if in it) based on pId
     * @param process
     */
    void removeProcess(Process process){
        for (int i = 0; i < numberOfPages; i++){
            if (pages[i] == process.getPId()) {
                pages[i] = 0;
            }
        }
    }


    /**
     *
     * @return number of free pages in memory
     */
    int numOfFreePages(){
        int open = 0;
        for (int i = 0; i < numberOfPages; i++){
            if (pages[i] == 0)
                open++;
        }
        return open;
    }

    /**
     * Print out the contents of memory along with the belong process.
     */
    void printMemoryMap(){
        map<int,int> tracker;
        cout << "    " << "Memory Map: " << endl;
        for (int i = 0; i < numberOfPages; i++){
            int pId = pages[i];
            /* Handle the free pages */
            if (pId == 0){

                int zero = i; //Account for continous zero pages
                for (int x = (i+1); x < numberOfPages; x++){
                    if (pages[x] == 0)
                        zero++;
                    else
                        break;
                }
                cout << "    " << "    " << (i*pageSize) << "-" << (zero*pageSize+pageSize-1) << ": Free frame(s)" << endl;
                i = zero; //Update 0 counter
                continue;
            }

            /* Sum up and calculated the pages and such for each process. */
            map<int,int>::iterator it = tracker.find(pId);
            if (it != tracker.end()){
                it->second = it->second+1;
            }else {
                tracker.insert(make_pair(pId, 1));
                it = tracker.find(pId);
            }
            cout << "    " << "    " << (i*pageSize) << "-" << (i*pageSize+pageSize-1) << ": Process " << pId << ", Page " << it->second << endl;
        }
    }
};

