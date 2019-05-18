//
// Created by Xander on 5/17/2019.
//
#pragma once
#include "genericImports.h"

/**
 *
 * This class represents a process object. Contains all relevant information about
 * the process once it has been loaded from the file as well as keeps tracks of its
 * various stages in memory.
 *
 */
class Process{
private:
    int pId;
    int arrivalTime;
    int timeInMemory;
    int lifeTime;
    int memory;
    bool arrived;

public:
    Process(int pId, int arrival, int life, int mem) {
        this->pId = pId;
        this->arrivalTime = arrival;
        this->lifeTime = life;
        this->memory = mem;
        this->timeInMemory = 0;
        this->arrived = false;
    }

    /**
     * Whether or not the process arrived at current time
     * @return
     */
    bool isArrived() const {
        return arrived;
    }

    /**
     * Acknowledges that the process arrived
     */
    void hasArrived() {
        arrived = true;
    }

    /**
     *
     * @return The unique process ID
     */
    const int getPId() const {
        return pId;
    }

    /**
     *
     * @return The time the process is arrived
     */
    int getArrivalTime() const {
        return arrivalTime;
    }

    /**
     *
     * @return the life of process in memory
     */
    int getLifeTime() const {
        return lifeTime;
    }

    /**
     *
     * @return int the memory used by process
     */
    int getMemory() const {
        return memory;
    }

    /**
     * Increase the time it has been loaded in memory
     */
    void updateTimeInMemory(){
        this->timeInMemory = this->timeInMemory+1;
    }

    /**
     * Return if the process is completed
     * @return
     */
    bool isCompleted(){
        return timeInMemory >= lifeTime;
    }

    /**
     * Print out information about each process.
     */
    void print(){
        cout << "PID: " << pId << "  AT: " << arrivalTime << "  Life: " << lifeTime << " Mem: " << memory << " TIM:" << timeInMemory << " Arriv?: " << arrived << endl;
    }


};



