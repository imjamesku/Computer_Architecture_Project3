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

    this->iTLB = new TLB(iMemoryPageSize);
    this->iPageTable = new PageTable(iMemoryPageSize);
    this->iDisk = new unsigned char[1024];
    this->iMemory = new Memory(iMemorySize, iMemoryPageSize);
    this->iCache = new Cache(totalICacheSize, iCacheBlockSize, iCacheAssociativity);

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
    delete this->iTLB;
    delete this->iPageTable;
    delete [] this->iDisk;
    delete this->iMemory;
    delete this->iCache;
}
void MemoryManager::initializeDisk(unsigned char* iInput){
    for(int i=0; i<1024; i++){
        this->iDisk[i] = iInput[i];
    }
}

unsigned char* MemoryManager::getIData(unsigned int virtualAddress, int cycle){
    int iTlbIndex = iTLB->getIndexToTargetPhysicalAddress(virtualAddress);
    if(iTlbIndex != -1){
        //TLB hit
        iTlbHits++;
        unsigned int physicalAddress = iTLB->getPhysicalAddress(iTlbIndex, virtualAddress);
        iTLB->updateLastRefCycle(iTlbIndex, cycle);
        iMemory->updateLastRefCycle(physicalAddress, cycle);
        return iMemory->getMemoryPointer(physicalAddress);
    }
    //TLB missed
    iTlbMisses++;

    unsigned int physicalAddress;
    if(iPageTable->getIsInMemory(virtualAddress) == 1){
        //iPageTableHits
        iPageTableHits++;
        physicalAddress = iPageTable->getPhysicalAddress(virtualAddress);
        iMemory->updateLastRefCycle(physicalAddress, cycle);
        //return iMemory->getMemoryPointer(physicalAddress);
    }
    else{
        //page fault
        iPageTalbeMisses++;
        unsigned int virtualPageHeadAddress = virtualAddress - virtualAddress % iMemoryPageSize;
        //printf("virtualPageHeadAddress = %u\n", virtualPageHeadAddress);
        unsigned char* virtualPageHeadPointer = iDisk + virtualPageHeadAddress;
        unsigned int victimPageHeadPhysicalAddress = iMemory->getVictimPageHeadPhysicalAddress();
        //printf("victimPageHeadPhysicalAddress = %d\n", victimPageHeadPhysicalAddress);
        iMemory->swapPages(virtualPageHeadPointer, victimPageHeadPhysicalAddress);
        iMemory->updateLastRefCycle(victimPageHeadPhysicalAddress, cycle);
        iPageTable->swapPages(victimPageHeadPhysicalAddress, virtualAddress);
        physicalAddress = iPageTable->getPhysicalAddress(virtualAddress);
        //printf("physicalAddress = %d\n", physicalAddress);
        //return iMemory->getMemoryPointer(physicalAddress);
    }
    int victimIndex = iTLB->getVictimIndex();
    iTLB->replacePage(victimIndex, virtualAddress, physicalAddress);
    iTLB->updateLastRefCycle(victimIndex, cycle);
    return iMemory->getMemoryPointer(physicalAddress);

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

