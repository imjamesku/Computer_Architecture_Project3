#include "TLB.h"

TLB::TLB(int pageSize)
{
    //ctor

    this->pageSize = pageSize;
    this->numOfEntries = 1024/pageSize/4;
    this->lastRefCycle = new unsigned int[numOfEntries];
    this->physicalPageNumber = new unsigned int[numOfEntries];
    this->tag = new unsigned int[numOfEntries];
    this->valid = new bool[numOfEntries];
    for(int i=0; i<numOfEntries; ++i){
        this->tag[i] = 0;
        this->lastRefCycle[i] = 0;
        this->physicalPageNumber[i] = 0;
        this->valid[i] = 0;
    }

}

TLB::~TLB()
{
    //dtor
    delete tag;
    delete lastRefCycle;
    delete physicalPageNumber;
    delete valid;
}

unsigned int TLB::getPhysicalAddress(int index, unsigned int virtualAddress){
    unsigned int pageOffset = virtualAddress%pageSize;
    if(valid[index])
        return physicalPageNumber[index]*pageSize + pageOffset;
    return 999999;
}
int TLB::getIndexToTargetPhysicalAddress(unsigned int virtualAddress){
    unsigned int virtualPageNumber = virtualAddress/pageSize;
    for(int i=0; i<numOfEntries; i++){
        if(valid[i] && tag[i] == virtualPageNumber){
            return i;
        }
    }
    return -1;
}
void TLB::updateLastRefCycle(int index, int cycle){
    lastRefCycle[index] = cycle;
}
int TLB::getVictimIndex()const{
    unsigned int victimIndex = 0;
    unsigned int smallestRefCycle = lastRefCycle[0];
    for(int i=0; i<numOfEntries; i++){
        if(valid[i] == 0)
            return i;
        if(lastRefCycle[i] < smallestRefCycle){
            victimIndex = i;
            smallestRefCycle = lastRefCycle[i];
        }
    }
    return victimIndex;
}
void TLB::replacePage(int index, unsigned int virtualAddress, unsigned int physicalAddress){
    tag[index] = virtualAddress/pageSize;
    physicalPageNumber[index] = physicalAddress/pageSize;
    valid[index] = 1;
}
