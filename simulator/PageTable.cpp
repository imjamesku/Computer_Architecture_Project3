#include "PageTable.h"

PageTable::PageTable(int pageSize)
{
    //ctor
    this->pageSize = pageSize;
    this->numOfEntries = 1024/pageSize;
    this->isInMemory = new bool[numOfEntries];
    this->dirty = new bool[numOfEntries];
    this->lastRefCycle = new unsigned int[numOfEntries];
    physicalPageNumber = new unsigned int[numOfEntries];
    for(int i=0; i<numOfEntries; ++i){
        this->isInMemory[i] = 0;
        this->dirty[i] = 0;
        this->lastRefCycle[i] = 0;
        this->physicalPageNumber[i] = 0;
    }
}

PageTable::~PageTable()
{
    //dtor
    delete isInMemory;
    delete dirty;
    delete lastRefCycle;
    delete physicalPageNumber;
}

unsigned int PageTable::getPhysicalAddress(unsigned int virtualAddress, int cycle){
    unsigned int virtualPageNumber = virtualAddress / pageSize;
    unsigned int pageOffset = virtualAddress % pageSize;
    if(isInMemory[virtualPageNumber]){
        lastRefCycle[virtualPageNumber] = cycle;
        unsigned int physicalAddress = physicalPageNumber[virtualPageNumber] * pageSize + pageOffset;
        return physicalAddress;
    }
    return virtualAddress;
}
bool PageTable::getIsInMemory(unsigned int virtualAddress){
    unsigned int virtualPageNumber = virtualAddress / pageSize;
    return isInMemory[virtualPageNumber];
}

