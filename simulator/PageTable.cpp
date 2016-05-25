#include "PageTable.h"

PageTable::PageTable(int pageSize)
{
    //ctor
    this->pageSize = pageSize;
    this->numOfEntries = 1024/pageSize;
    this->isInMemory = new bool[numOfEntries];
  //  this->dirty = new bool[numOfEntries];
  //  this->lastRefCycle = new unsigned int[numOfEntries];
    physicalPageNumber = new unsigned int[numOfEntries];
    for(int i=0; i<numOfEntries; ++i){
        this->isInMemory[i] = 0;
        this->physicalPageNumber[i] = 0;
    }
}

PageTable::~PageTable()
{
    //dtor
    if(isInMemory != nullptr)
        delete isInMemory;
    if(physicalPageNumber != nullptr)
    delete physicalPageNumber;
}

unsigned int PageTable::getPhysicalAddress(unsigned int virtualAddress){
    unsigned int virtualPageNumber = virtualAddress / pageSize;
    unsigned int pageOffset = virtualAddress % pageSize;
    if(isInMemory[virtualPageNumber]){
        unsigned int physicalAddress = physicalPageNumber[virtualPageNumber] * pageSize + pageOffset;
        return physicalAddress;
    }
    return -1;
}
bool PageTable::getIsInMemory(unsigned int virtualAddress){
    unsigned int virtualPageNumber = virtualAddress / pageSize;
    return isInMemory[virtualPageNumber];
}
void PageTable::swapPages(unsigned int victimPagePhysicalAddress, unsigned int virtualAddressOfDataIntoMem){
    unsigned int victimPageNumber = victimPagePhysicalAddress / pageSize;
    unsigned int virtualPageNumberOfDataIntoMem = virtualAddressOfDataIntoMem / pageSize;

    for(int i=0; i<numOfEntries; i++){
        if(isInMemory[i] == 1 && physicalPageNumber[i] == victimPageNumber){
            isInMemory[i] = 0;
            physicalPageNumber[i] = -1;
            break;
        }
    }
    isInMemory[virtualPageNumberOfDataIntoMem] = 1;
    physicalPageNumber[virtualPageNumberOfDataIntoMem] = victimPageNumber;
}


