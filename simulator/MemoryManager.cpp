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

    this->dTLB = new TLB(dMemoryPageSize);
    this->dPageTable = new PageTable(dMemoryPageSize);
    this->dDisk = new unsigned char[1024];
    this->dMemory = new Memory(dMemorySize, dMemoryPageSize);
    this->dCache = new Cache(totalDCacheSize, dCacheBlockSize, dCacheAssociativity);

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

    delete this->dTLB;
    delete this->dPageTable;
    delete [] this->dDisk;
    delete this->dMemory;
    delete this->dCache;
}
void MemoryManager::initializeDisk(unsigned char* iInput, unsigned char* dInput){
    for(int i=0; i<1024; i++){
        this->iDisk[i] = iInput[i];
        this->dDisk[i] = dInput[i];
    }
}

unsigned char* MemoryManager::getIData(unsigned int virtualAddress, int cycle){
    int iTlbIndex = iTLB->getIndexToTargetPhysicalAddress(virtualAddress);
    if(iTlbIndex != -1){
        //TLB hit
        iTlbHits++;
        unsigned int physicalAddress = iTLB->getPhysicalAddress(iTlbIndex, virtualAddress);
        iTLB->updateLastRefCycle(iTlbIndex, cycle);
        //try to access cache first
        int iCacheBlockIndex = iCache->getBlockIndex(physicalAddress);
        if(iCacheBlockIndex != -1){
            //cache hit
            iCacheHits++;
            iCache->updateMru(iCacheBlockIndex);
            return iCache->getData(iCacheBlockIndex, physicalAddress);
        }
        //cache miss
        iCacheMisses++;
        unsigned int blockHeadAddress = iCache->getBlockHeadAddress(physicalAddress);
       // iMemory->updateLastRefCycle(blockHeadAddress, cycle);//may be different than golden
        unsigned char* blockHeadDataPointer = iMemory->getMemoryPointer(blockHeadAddress);
        int victimBlockIndex = iCache->getVictimBlockIndex(blockHeadAddress);
        iCache->replaceBlock(victimBlockIndex, blockHeadAddress, blockHeadDataPointer);
        iCache->updateMru(victimBlockIndex);
        return iCache->getData(victimBlockIndex, physicalAddress);
    }
    //TLB missed
    iTlbMisses++;
    unsigned char* returnDataPointer;
    unsigned int physicalAddress;
    if(iPageTable->getIsInMemory(virtualAddress) == 1){
        //Page table hit
        iPageTableHits++;
        physicalAddress = iPageTable->getPhysicalAddress(virtualAddress);
        iMemory->updateLastRefCycle(physicalAddress, cycle);
        //try to access cache first
        int iCacheBlockIndex = iCache->getBlockIndex(physicalAddress);
        if(iCacheBlockIndex != -1){
            //cache hit
            iCacheHits++;
            iCache->updateMru(iCacheBlockIndex);
            returnDataPointer = iCache->getData(iCacheBlockIndex, physicalAddress);
        }
        else{
            //cache missed
            iCacheMisses++;
            unsigned int blockHeadAddress = iCache->getBlockHeadAddress(physicalAddress);
          //  iMemory->updateLastRefCycle(physicalAddress, cycle);//may be different than golden
            unsigned char* blockHeadDataPointer = iMemory->getMemoryPointer(blockHeadAddress);
            int victimBlockIndex = iCache->getVictimBlockIndex(blockHeadAddress);
            iCache->replaceBlock(victimBlockIndex, blockHeadAddress, blockHeadDataPointer);
            iCache->updateMru(victimBlockIndex);
            returnDataPointer = iCache->getData(victimBlockIndex, physicalAddress);
        }

        //return iMemory->getMemoryPointer(physicalAddress);
    }
    else{
        //page fault
        iPageTalbeMisses++;
        iCacheMisses++;
        unsigned int virtualPageHeadAddress = virtualAddress - virtualAddress % iMemoryPageSize;
        //printf("virtualPageHeadAddress = %u\n", virtualPageHeadAddress);
        unsigned char* virtualPageHeadPointer = iDisk + virtualPageHeadAddress;
        unsigned int victimPageHeadPhysicalAddress = iMemory->getVictimPageHeadPhysicalAddress();
        //printf("victimPageHeadPhysicalAddress = %d\n", victimPageHeadPhysicalAddress);
        iMemory->swapPages(virtualPageHeadPointer, victimPageHeadPhysicalAddress);
        iCache->deleteEntirePage(victimPageHeadPhysicalAddress, iMemoryPageSize);
        iTLB->setToInvalid(victimPageHeadPhysicalAddress);//if the data is swapped to disk, we need to set TLB's valid to 0
        iMemory->updateLastRefCycle(victimPageHeadPhysicalAddress, cycle);
        iPageTable->swapPages(victimPageHeadPhysicalAddress, virtualAddress);
        //re-translate address
        physicalAddress = iPageTable->getPhysicalAddress(virtualAddress);
        //printf("virtualAddress = %d\n", virtualAddress);
        //printf("physicalAddress = %d\n", physicalAddress);
        //load to cache
        unsigned int blockHeadAddress = iCache->getBlockHeadAddress(physicalAddress);
        //printf("block head Add = %d\n", blockHeadAddress);
        iMemory->updateLastRefCycle(physicalAddress, cycle);
        unsigned char* blockHeadDataPointer = iMemory->getMemoryPointer(blockHeadAddress);
        int victimBlockIndex = iCache->getVictimBlockIndex(blockHeadAddress);
        //printf("victim block index = %d\n", victimBlockIndex);
        iCache->replaceBlock(victimBlockIndex, blockHeadAddress, blockHeadDataPointer);
        iCache->updateMru(victimBlockIndex);
        returnDataPointer = iCache->getData(victimBlockIndex, physicalAddress);
        //printf("physicalAddress = %d\n", physicalAddress);
        //return iMemory->getMemoryPointer(physicalAddress);
    }
    //update TLB
    int victimIndex = iTLB->getVictimIndex();
    iTLB->replacePage(victimIndex, virtualAddress, physicalAddress);
    iTLB->updateLastRefCycle(victimIndex, cycle);
    return returnDataPointer;


}
unsigned char* MemoryManager::getDData(unsigned int virtualAddress, int cycle){
    int dTlbIndex = dTLB->getIndexToTargetPhysicalAddress(virtualAddress);
    if(dTlbIndex != -1){
        //TLB hit
        dTlbHits++;
        unsigned int physicalAddress = dTLB->getPhysicalAddress(dTlbIndex, virtualAddress);
        dTLB->updateLastRefCycle(dTlbIndex, cycle);
        //try to access cache first
        int dCacheBlockIndex = dCache->getBlockIndex(physicalAddress);
        if(dCacheBlockIndex != -1){
            //cache hit
            dCacheHits++;
            dCache->updateMru(dCacheBlockIndex);
            return dCache->getData(dCacheBlockIndex, physicalAddress);
        }
        //cache miss
        dCacheMisses++;
        unsigned int blockHeadAddress = dCache->getBlockHeadAddress(physicalAddress);
       // dMemory->updateLastRefCycle(blockHeadAddress, cycle);//may be different than golden
        unsigned char* blockHeadDataPointer = dMemory->getMemoryPointer(blockHeadAddress);
        int victimBlockIndex = dCache->getVictimBlockIndex(blockHeadAddress);
        dCache->replaceBlock(victimBlockIndex, blockHeadAddress, blockHeadDataPointer);
        dCache->updateMru(victimBlockIndex);
        return dCache->getData(victimBlockIndex, physicalAddress);
    }
    //TLB missed
    dTlbMisses++;
    unsigned char* returnDataPointer;
    unsigned int physicalAddress;
    if(dPageTable->getIsInMemory(virtualAddress) == 1){
        //Page table hit
        dPageTableHits++;
        physicalAddress = dPageTable->getPhysicalAddress(virtualAddress);
        dMemory->updateLastRefCycle(physicalAddress, cycle);
        //try to access cache first
        int dCacheBlockIndex = dCache->getBlockIndex(physicalAddress);
        if(dCacheBlockIndex != -1){
            //cache hit
            dCacheHits++;
            dCache->updateMru(dCacheBlockIndex);
            returnDataPointer = dCache->getData(dCacheBlockIndex, physicalAddress);
        }
        else{
            //cache missed
            dCacheMisses++;
            unsigned int blockHeadAddress = dCache->getBlockHeadAddress(physicalAddress);
        //    dMemory->updateLastRefCycle(physicalAddress, cycle);//may be different than golden
            unsigned char* blockHeadDataPointer = dMemory->getMemoryPointer(blockHeadAddress);
            int victimBlockIndex = dCache->getVictimBlockIndex(blockHeadAddress);
            dCache->replaceBlock(victimBlockIndex, blockHeadAddress, blockHeadDataPointer);
            dCache->updateMru(victimBlockIndex);
            returnDataPointer = dCache->getData(victimBlockIndex, physicalAddress);
        }

        //return iMemory->getMemoryPointer(physicalAddress);
    }
    else{
        //page fault
        dPageTableMisses++;
        dCacheMisses++;
        unsigned int virtualPageHeadAddress = virtualAddress - virtualAddress % dMemoryPageSize;
        //printf("virtualPageHeadAddress = %u\n", virtualPageHeadAddress);
        unsigned char* virtualPageHeadPointer = dDisk + virtualPageHeadAddress;
        unsigned int victimPageHeadPhysicalAddress = dMemory->getVictimPageHeadPhysicalAddress();
        //printf("victimPageHeadPhysicalAddress = %d\n", victimPageHeadPhysicalAddress);
        dMemory->swapPages(virtualPageHeadPointer, victimPageHeadPhysicalAddress);
        dCache->deleteEntirePage(victimPageHeadPhysicalAddress, dMemoryPageSize);
        dTLB->setToInvalid(victimPageHeadPhysicalAddress);
        dMemory->updateLastRefCycle(victimPageHeadPhysicalAddress, cycle);
        dPageTable->swapPages(victimPageHeadPhysicalAddress, virtualAddress);
        //re-translate address
        physicalAddress = dPageTable->getPhysicalAddress(virtualAddress);
        //printf("virtualAddress = %d\n", virtualAddress);
        //printf("physicalAddress = %d\n", physicalAddress);
        //load to cache
        unsigned int blockHeadAddress = dCache->getBlockHeadAddress(physicalAddress);
        //printf("block head Add = %d\n", blockHeadAddress);
        dMemory->updateLastRefCycle(physicalAddress, cycle);
        unsigned char* blockHeadDataPointer = dMemory->getMemoryPointer(blockHeadAddress);
        int victimBlockIndex = dCache->getVictimBlockIndex(blockHeadAddress);
        //printf("victim block index = %d\n", victimBlockIndex);
        dCache->replaceBlock(victimBlockIndex, blockHeadAddress, blockHeadDataPointer);
        dCache->updateMru(victimBlockIndex);
        returnDataPointer = dCache->getData(victimBlockIndex, physicalAddress);
        //printf("physicalAddress = %d\n", physicalAddress);
        //return iMemory->getMemoryPointer(physicalAddress);
    }
    //update TLB
    int victimIndex = dTLB->getVictimIndex();
    dTLB->replacePage(victimIndex, virtualAddress, physicalAddress);
    dTLB->updateLastRefCycle(victimIndex, cycle);
    return returnDataPointer;
}
void MemoryManager::writeDData(unsigned int virtualAddress, unsigned char* data, int lengthInbytes, int cycle){
    int dTlbIndex = dTLB->getIndexToTargetPhysicalAddress(virtualAddress);
    if(dTlbIndex != -1){
        //TLB hit
        dTlbHits++;
        unsigned int physicalAddress = dTLB->getPhysicalAddress(dTlbIndex, virtualAddress);
        dTLB->updateLastRefCycle(dTlbIndex, cycle);
        //try to access cache first
        int dCacheBlockIndex = dCache->getBlockIndex(physicalAddress);
        if(dCacheBlockIndex != -1){
            //cache hit
            dCacheHits++;
            dCache->updateMru(dCacheBlockIndex);
            //return dCache->getData(dCacheBlockIndex, physicalAddress);
            dCache->writeData(dCacheBlockIndex, physicalAddress, data, lengthInbytes);
            dMemory->writeData(physicalAddress, data, lengthInbytes);
            this->writeDataToDDisk(virtualAddress, data, lengthInbytes);
            return;
        }
        //cache miss
        dCacheMisses++;
        unsigned int blockHeadAddress = dCache->getBlockHeadAddress(physicalAddress);
       // dMemory->updateLastRefCycle(blockHeadAddress, cycle);//may be different than golden
        unsigned char* blockHeadDataPointer = dMemory->getMemoryPointer(blockHeadAddress);
        int victimBlockIndex = dCache->getVictimBlockIndex(blockHeadAddress);
        dCache->replaceBlock(victimBlockIndex, blockHeadAddress, blockHeadDataPointer);
        dCache->updateMru(victimBlockIndex);

        dCache->writeData(victimBlockIndex, physicalAddress, data, lengthInbytes);
        dMemory->writeData(physicalAddress, data, lengthInbytes);
        this->writeDataToDDisk(virtualAddress, data, lengthInbytes);
        //return dCache->getData(victimBlockIndex, physicalAddress);
        return;
    }
    //TLB missed
    dTlbMisses++;
  //  unsigned char* returnDataPointer;
    unsigned int physicalAddress;
    if(dPageTable->getIsInMemory(virtualAddress) == 1){
        //Page table hit
        dPageTableHits++;
        physicalAddress = dPageTable->getPhysicalAddress(virtualAddress);
        dMemory->updateLastRefCycle(physicalAddress, cycle);
        //try to access cache first
        int dCacheBlockIndex = dCache->getBlockIndex(physicalAddress);
        if(dCacheBlockIndex != -1){
            //cache hit
            dCacheHits++;
            dCache->updateMru(dCacheBlockIndex);

            dCache->writeData(dCacheBlockIndex, physicalAddress, data, lengthInbytes);
            dMemory->writeData(physicalAddress, data, lengthInbytes);
            this->writeDataToDDisk(virtualAddress, data, lengthInbytes);
            //returnDataPointer = dCache->getData(dCacheBlockIndex, physicalAddress);
        }
        else{
            //cache missed
            dCacheMisses++;
            unsigned int blockHeadAddress = dCache->getBlockHeadAddress(physicalAddress);
        //    dMemory->updateLastRefCycle(physicalAddress, cycle);//may be different than golden
            unsigned char* blockHeadDataPointer = dMemory->getMemoryPointer(blockHeadAddress);
            int victimBlockIndex = dCache->getVictimBlockIndex(blockHeadAddress);
            dCache->replaceBlock(victimBlockIndex, blockHeadAddress, blockHeadDataPointer);
            dCache->updateMru(victimBlockIndex);

            dCache->writeData(victimBlockIndex, physicalAddress, data, lengthInbytes);
            dMemory->writeData(physicalAddress, data, lengthInbytes);
            this->writeDataToDDisk(virtualAddress, data, lengthInbytes);
            //returnDataPointer = dCache->getData(victimBlockIndex, physicalAddress);
        }

        //return iMemory->getMemoryPointer(physicalAddress);
    }
    else{
        //page fault
        dPageTableMisses++;
        dCacheMisses++;
        unsigned int virtualPageHeadAddress = virtualAddress - virtualAddress % dMemoryPageSize;
        //printf("virtualPageHeadAddress = %u\n", virtualPageHeadAddress);
        unsigned char* virtualPageHeadPointer = dDisk + virtualPageHeadAddress;
        unsigned int victimPageHeadPhysicalAddress = dMemory->getVictimPageHeadPhysicalAddress();
        /*r***************remember to add setToInvalid**************/
        //printf("victimPageHeadPhysicalAddress = %d\n", victimPageHeadPhysicalAddress);
        dMemory->swapPages(virtualPageHeadPointer, victimPageHeadPhysicalAddress);
        dCache->deleteEntirePage(victimPageHeadPhysicalAddress, dMemoryPageSize);
        dTLB->setToInvalid(victimPageHeadPhysicalAddress);
        dMemory->updateLastRefCycle(victimPageHeadPhysicalAddress, cycle);
        dPageTable->swapPages(victimPageHeadPhysicalAddress, virtualAddress);
        //re-translate address
        physicalAddress = dPageTable->getPhysicalAddress(virtualAddress);
        //printf("virtualAddress = %d\n", virtualAddress);
        //printf("physicalAddress = %d\n", physicalAddress);
        //load to cache
        unsigned int blockHeadAddress = dCache->getBlockHeadAddress(physicalAddress);
        //printf("block head Add = %d\n", blockHeadAddress);
        dMemory->updateLastRefCycle(physicalAddress, cycle);
        unsigned char* blockHeadDataPointer = dMemory->getMemoryPointer(blockHeadAddress);
        int victimBlockIndex = dCache->getVictimBlockIndex(blockHeadAddress);
        //printf("victim block index = %d\n", victimBlockIndex);
        dCache->replaceBlock(victimBlockIndex, blockHeadAddress, blockHeadDataPointer);
        dCache->updateMru(victimBlockIndex);
        dCache->writeData(victimBlockIndex, physicalAddress, data, lengthInbytes);
        dMemory->writeData(physicalAddress, data, lengthInbytes);
        this->writeDataToDDisk(virtualAddress, data, lengthInbytes);
        //returnDataPointer = dCache->getData(victimBlockIndex, physicalAddress);
        //printf("physicalAddress = %d\n", physicalAddress);
        //return iMemory->getMemoryPointer(physicalAddress);
    }
    //update TLB
    int victimIndex = dTLB->getVictimIndex();
    dTLB->replacePage(victimIndex, virtualAddress, physicalAddress);
    dTLB->updateLastRefCycle(victimIndex, cycle);
   // return returnDataPointer;
}
void MemoryManager::writeDataToDDisk(unsigned int virtualAddress, unsigned char* data, int lengthInBytes){
    for(int i=0; i<lengthInBytes; i++){
        dDisk[virtualAddress + i] = data[i];
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
    printf("# misses: %u\n\n", dTlbMisses);

    printf("IPageTable :\n");
    printf("# hits: %u\n", iPageTableHits);
    printf("# misses: %u\n\n", iPageTalbeMisses);

    printf("DPageTable :\n");
    printf("# hits: %u\n", dPageTableHits);
    printf("# misses: %u\n\n", dPageTableMisses);
}

void MemoryManager::printReport(FILE* reportFile){
    fprintf(reportFile, "ICache :\n");
    fprintf(reportFile, "# hits: %u\n", iCacheHits);
    fprintf(reportFile,"# misses: %u\n\n",iCacheMisses);

    fprintf(reportFile,"DCache :\n");
    fprintf(reportFile,"# hits: %u\n", dCacheHits);
    fprintf(reportFile,"# misses: %u\n\n", dCacheMisses);

    fprintf(reportFile,"ITLB :\n");
    fprintf(reportFile,"# hits: %u\n", iTlbHits);
    fprintf(reportFile,"# misses: %u\n\n", iTlbMisses);

    fprintf(reportFile,"DTLB :\n");
    fprintf(reportFile,"# hits: %u\n", dTlbHits);
    fprintf(reportFile,"# misses: %u\n\n", dTlbMisses);

    fprintf(reportFile,"IPageTable :\n");
    fprintf(reportFile,"# hits: %u\n", iPageTableHits);
    fprintf(reportFile,"# misses: %u\n\n", iPageTalbeMisses);

    fprintf(reportFile,"DPageTable :\n");
    fprintf(reportFile,"# hits: %u\n", dPageTableHits);
    fprintf(reportFile,"# misses: %u\n\n", dPageTableMisses);
}

