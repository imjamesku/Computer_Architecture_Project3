#include "MemoryManager.h"

MemoryManager::MemoryManager(int iMemorySize, int dMemorySize, int iMemoryPageSize, int dMemoryPageSize,
    int totalICacheSize, int iCacheBlockSize, int iCacheAssociativity,
    int totalDCacheSize, int dCacheBlockSize, int dCacheAssociativity){

    this->iMemorySize = iMemorySize;
    this->dMemorySize = dMemorySize;
    this->iMemoryPageSize = iMemoryPageSize;
    this->dMemoryPageSize = dMemoryPageSize;
    this->totalICacheSize = totalICacheSize;
    this->iCacheBlockSize = iCacheBlockSize;
    this->iCacheAssociativity = iCacheAssociativity;
    this->totalDCacheSize = totalDCacheSize;
    this->dCacheBlockSize = dCacheBlockSize;
    this->dCacheAssociativity = dCacheAssociativity;

    this->iPageTable = new PageTable(iMemoryPageSize);
    this->iDisk = new unsigned char[1024];
    this->iMemory = new Memory(iMemorySize, iMemoryPageSize);
}

MemoryManager::~MemoryManager()
{
    //dtor
    delete this->iPageTable;
    delete this->iDisk;
    delete this->iMemory;
}
void MemoryManager::initializeDisk(unsigned char* iInput){
    for(int i=0; i<1024; i++){
        this->iDisk[i] = iInput[i];
    }
}

unsigned char* MemoryManager::getData(unsigned int virtualAddress, int cycle){
    if(iPageTable->getIsInMemory(virtualAddress)){
        unsigned int physicalAddress = iPageTable->getPhysicalAddress(virtualAddress, cycle);
        iMemory->updateLastRefCycle(physicalAddress, cycle);
        return iMemory->getMemoryPointer(physicalAddress);
    }
    else{
        unsigned int virtualPageHeadAddress = virtualAddress - virtualAddress % iMemoryPageSize;
        unsigned char* virtualPageHeadPointer = iDisk + virtualPageHeadAddress;
        unsigned int victimPageHeadPhysicalAddress = iMemory->getVictimPageHeadPhysicalAddress();
    }
}

