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

    iCacheHits = 0;
    iCacheMisses = 0;
    dCacheHits = 0;
    dCacheMisses = 0;
    iTlbHits = 0;
    iTlbMisses = 0;
    dTlbHits = 0;
    dTlbMisses = 0;
    iPageTableHits = 0;
    iPageTalbeMisses = 0;
    dPageTableHits = 0;
    dPageTableMisses = 0;

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

unsigned char* MemoryManager::getIData(unsigned int virtualAddress, int cycle){
    if(iPageTable->getIsInMemory(virtualAddress) == 1){
        iPageTableHits++;
        unsigned int physicalAddress = iPageTable->getPhysicalAddress(virtualAddress);
        iMemory->updateLastRefCycle(physicalAddress, cycle);
        return iMemory->getMemoryPointer(physicalAddress);
    }
    else{
        iPageTalbeMisses++;
        unsigned int virtualPageHeadAddress = virtualAddress - virtualAddress % iMemoryPageSize;
        unsigned char* virtualPageHeadPointer = iDisk + virtualPageHeadAddress;
        unsigned int victimPageHeadPhysicalAddress = iMemory->getVictimPageHeadPhysicalAddress();
        iMemory->swapPages(virtualPageHeadPointer, victimPageHeadPhysicalAddress);
        iMemory->updateLastRefCycle(victimPageHeadPhysicalAddress, cycle);
        iPageTable->swapPages(victimPageHeadPhysicalAddress, virtualAddress);
        unsigned int physicalAddress = iPageTable->getPhysicalAddress(virtualAddress);
        return iMemory->getMemoryPointer(physicalAddress);
    }

}
void MemoryManager::displayReport(){
    printf("ICache :\n");
    printf("# hits: %u\n", iCacheHits);
    printf("# misses: %u\n\n",iCacheMisses);

    printf("DCache :\n");
    printf("# hits: %u\n", dCacheHits);
    printf("# misses: %u\n\n", dCacheMisses);

    printf("ITLB :\n");
    printf("# hits: %u\n", iTlbHits);
    printf("# misses: %u\n\n", iTlbMisses);

    printf("DTLB :\n");
    printf("# hits: %u\n", dTlbHits);
    printf("# misses: %u\n\n", dTlbHits);

    printf("IPageTable :\n");
    printf("# hits: %u\n", iPageTableHits);
    printf("# misses: %u\n\n", iPageTalbeMisses);

    printf("DPageTable :\n");
    printf("# hits: %u\n", dPageTableHits);
    printf("# misses: %u\n\n", dPageTableMisses);
}

