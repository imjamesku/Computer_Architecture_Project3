#include "Memory.h"

Memory::Memory()
{
    //ctor
    for(int i=0; i<1024; i++){
        memory[i] = 0;
    }
}


Memory::Memory(FILE *image, unsigned int startFrom)
{
    //ctor
     for(int i=0; i<1024; i++){
        memory[i] = 0;
    }
    size_t result;
    unsigned char readArray[4];
    result = fread(readArray, 4, 1, image);
    words = readArray[0] << 24 | readArray[1] << 16 | readArray[2] << 8 | readArray[3];
    result = fread(memory+startFrom, 4, words, image);
}
Memory::Memory(int memorySize, int pageSize){
    setMemorySize(memorySize);
    this->pageSize = pageSize;
    this->numberOfPages = memorySize/pageSize;
    isOccypied = new bool[numberOfPages];
    lastRefCycle = new int[numberOfPages];
    setMemoryToZero();
    for(int i=0; i<numberOfPages; i++){
        isOccypied[i] = 0;
        lastRefCycle[i] = 0;
    }
}
Memory::~Memory()
{
    //dtor
}

void Memory::setMemorySize(int size){
    this->memorySize = size;
}

int Memory::getMemorySize(){
    return memorySize;
}

unsigned char* Memory::getMemoryPointer(int offset){
    return (memory+offset);
}
void Memory::setMemoryToZero(){
    for(int i=0; i<this->memorySize; i++)
        memory[i] = 0;
}
void Memory::updateLastRefCycle(unsigned int physicalAddress, int cycle){
    unsigned int physicalPageNumber = physicalAddress/pageSize;
    this->lastRefCycle[physicalPageNumber] = cycle;
}
unsigned int Memory::getVictimPageHeadPhysicalAddress(){
    unsigned int victimAddress = 0;
    int smallestRefCycle = lastRefCycle[0];
    for(int i=0; i<numberOfPages; i++){
        if(isOccypied[i] == 0)
            return i*pageSize;
        if(lastRefCycle[i] < smallestRefCycle){
            victimAddress = i*pageSize;
            smallestRefCycle = lastRefCycle[i];
        }
    }
    return victimAddress;
}

void Memory::swapPages(unsigned char* virtualPageHeadPointer, unsigned int targetPageHeadPhysicalAddress){
    for(int i=0; i<pageSize; i++){
        memory[targetPageHeadPhysicalAddress+i] = virtualPageHeadPointer[i];
    }
    isOccypied[targetPageHeadPhysicalAddress/pageSize] = 1;
}
