#include "PageTable.h"

PageTable::PageTable(int pageSize)
{
    //ctor
    this->pageSize = pageSize;
    this->numOfEntries = 1024/pageSize;
    this->isInMemory = new bool[numOfEntries];
    this->dirty = new bool[numOfEntries];
    this->lastRefCycle = new unsigned int[numOfEntries];
    physicalAddresses = new unsigned int[numOfEntries];
    for(int i=0; i<numOfEntries; ++i){
        this->isInMemory[i] = 0;
        this->dirty[i] = 0;
        this->lastRefCycle[i] = 0;
        this->physicalAddresses[i] = 0;
    }
}

PageTable::~PageTable()
{
    //dtor
    delete isInMemory;
    delete dirty;
    delete lastRefCycle;
    delete physicalAddresses;
}
