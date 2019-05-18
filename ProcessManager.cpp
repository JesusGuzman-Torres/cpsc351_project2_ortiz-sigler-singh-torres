//
// Created by Xander on 5/17/2019.
//


#include "genericImports.h"
#include "process.h"
#include "MemoryManager.cpp"

/**
 * This is a manager to handle the adding of all processes and simulation of them.
 * totalProcess = All processes loaded from memory
 * unloadedProceses = Processes who arrival time passed are added to this queue
 * inProgress = Processes that are currently in memory
 * clockTIme = Simulated System CLock Time
 * Process Count = total amount of processes in system
 * MemoryManager - handles all memory allocation operation
 * printOUtMessages - whether current time requires a printout.
 */
class ProcessManager{
private:
    int clockTime, processCount;
    vector<Process> totalProcesses;
    vector<Process> unloadedProcesses;
    vector<Process> inProgressProcesses;
    MemoryManager *mm;
    bool printOutMessages;
    double totalTime = 0;
public:

    /**
     * Deconstructor, delete the newly created memory manager.
     */
    ~ProcessManager(){
        delete mm;
    }

    /**
     * Constructor, creates and allocates everything.
     * @param inputFile The in1.txt, file that is loaded
     * @param memorySize The total memory (2K)
     * @param pageSize The size of the page (100, 200 ,400)
     */
    ProcessManager(ifstream &inputFile, int memorySize, int pageSize) {
        /* Initialize the memory manager and break memory into pages */
        this->mm = new MemoryManager(memorySize, pageSize);

        clockTime = 0; //Initial clock time

        /* Load all of the process from the file */
        inputFile >> processCount; //Load the total # of processes
        int pId,arrivalTime, lifeTime,memorySegments,totalMemoryNeeded;
        while (inputFile >> pId &&
               inputFile >>arrivalTime &&
               inputFile >> lifeTime &&
               inputFile >> memorySegments){

            totalMemoryNeeded = 0;
            for (int i = 0; i < memorySegments; ++i)
            {
                int tempMem;
                inputFile >> tempMem;
                totalMemoryNeeded += tempMem;
            }

            /* Create the process then add it to our total list */
            Process process(pId, arrivalTime, lifeTime, totalMemoryNeeded);
            totalProcesses.push_back(process); //Total Process list
        }

        /* Run the simulation */
        while (clockTime < MAX_CLOCK_TIME)
            //while (clockTime < 10)
            run();


        for (auto x : totalProcesses){
            totalTime += x.getLifeTime();
        }
        cout << endl << endl << "Average Turnaround time: " << fixed << setprecision(2) << (totalTime/processCount);
    }

    /**
     *
     * The runner to execute and simulate each clock cycle. It will
     * increase the clock cycle, compare the different times, and figure out what is occuring.
     *
     */
    void run(){
        Process *process;
        printOutMessages = false;

        /* Handle the process's arrival time. */
        for (int i = 0; i < totalProcesses.size(); i++){
            process = &totalProcesses[i];
            if (process->getArrivalTime() == clockTime) {
                if (!process->isArrived()) { //Make sure it marked as arrived.
                    process->hasArrived();
                    unloadedProcesses.push_back(*process); //Means it has arrived.
                    printArrivalMessage(process->getPId());
                    printInputQueue();
                }
            }
        }

        /* Handle the processes that are completed. */
        for (int i = 0; i < inProgressProcesses.size(); i++) {
            process = &inProgressProcesses[i];  //Variable definition
            /* Process succesfully completed. */
            if (process->isCompleted()){
                mm->removeProcess(*process); //Remove process from memory
                printOutCompletionMessage(process->getPId());
                printOutMemoryMap();
                inProgressProcesses.erase(inProgressProcesses.begin()+i); //Remove it from vector
                i -=1; //Adjust the counter
            }
        }


        /* Handle the starting of the processes into memory */
        for (int i = 0; i < unloadedProcesses.size(); i++){
            process = &unloadedProcesses[i]; //Variable definition
            if (mm->addProcess(*process)) { //Try to add process to memory (true if it did, increase totaltime if not)
                inProgressProcesses.push_back(*process);//Add to local inorigress vector

                /* Handle the erasure of that process in the unloaded processes */
                unloadedProcesses.erase(unloadedProcesses.begin() + i);
                i -= 1; //Adjust counter in loop since we removed a value.
                printInputQueue();
                printOutMemoryMap();
            }else
                totalTime++; //Need this to account for time waiting without free memory
        }

        /* Update the time in memory for each process */
        for (auto process = inProgressProcesses.begin(); process != inProgressProcesses.end(); process++){
            process->updateTimeInMemory(); //Increase the life in memory of ecah process by one
        }

        clockTime++; //Increase the clocktime to the next cycle.
    }

    /* Print out the memory map */
    void printOutMemoryMap(){
        mm->printMemoryMap();
    }

    /* Have it print out time and allow other messages to be printed */
    void enablePrintOutMessage(){
        if (!printOutMessages){
            cout << endl << "t = " << clockTime << ": " << endl;
            printOutMessages = true;
        }
    }

    /* Format and print the input queue */
    void printInputQueue(){
        if (!printOutMessages)
            return;
        cout << "    " << "Input Queue [ ";
        for (auto x : unloadedProcesses)
            cout << x.getPId() << " ";
        cout << "]" << endl;
    }

    /* Print out the process arriving */
    void printArrivalMessage(int pId){
        enablePrintOutMessage();
        cout << "    " << "Process " << pId << " arrives" << endl;
    }

    /* Print out process completing. */
    void printOutCompletionMessage(int pId){
        enablePrintOutMessage();
        cout << "    " << "Process " << pId << " completes" << endl;
    }
};
