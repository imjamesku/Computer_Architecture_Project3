#include "TLB.h"

TLB::TLB(int pageSize)
{
    //ctor

    this->pageSize = pageSize;
    this->numOfEntries = 1024/pageSize/4;
    this->lastRefCycle = new unsigned int[numOfEntries];
    this->physicalAddress = new unsigned int[numOfEntries];
    this->tag = new unsigned int[numOfEntries];
    this->valid = new bool[numOfEntries];
    for(int i=0; i<numOfEntries; ++i){
        this->tag[i] = 0;
        this->lastRefCycle[i] = 0;
        this->physicalAddress[i] = 0;
        this->valid[i] = 0;
    }

}

TLB::~TLB()
{
    //dtor
    delete tag;
    delete lastRefCycle;
    delete physicalAddress;
    delete valid;
}

int TLB::getPhysicalAddress(unsigned int virtualAddress){
    unsigned int virtualPageNumber = virtualAddress/pageSize;
    unsigned int pageOffset = virtualAddress%pageSize;
    for(int i=0; i<numOfEntries; i++){
        if(tag[i] == virtualPageNumber){
            return physicalAddress[i]*pageSize + pageOffset;
        }
    }
    return -1;
}
