#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H
#include "Memory.h"
#include "PageTable.h"
#include "TLB.h"
#include "Cache.h"


class MemoryManager
{
    public:
        int iMemorySize, dMemorySize, iMemoryPageSize, dMemoryPageSize;
        int totalICacheSize, iCacheBlockSize, iCacheAssociativity;
        int totalDCacheSize, dCacheBlockSize, dCacheAssociativity;

        TLB* iTLB;
        PageTable* iPageTable;
        Cache* iCache;
        Memory* iMemory;
        unsigned char* iDisk;
        MemoryManager(int iMemorySize, int dMemorySize, int iMemoryPageSize, int dMemoryPageSize,
                      int totalICacheSize, int iCacheBlockSize, int iCacheAssociativity,
                      int totalDCacheSize, int dCacheBlockSize, int dCacheAssociativity);
        void initializeDisk(unsigned char* iInput);
        unsigned int getPhysicalAddressViaPageTable(unsigned int vertualAddress);
        unsigned char* getIData(unsigned int vertualAddress, int cycle);
        unsigned char* swapPages(unsigned int virtualAddress);
        void printReport();
        void displayReport();
        virtual ~MemoryManager();
       // unsigned char readData(unsigned int address);
       // unsigned char* getInsPointer(unsigned int pc);
       // void writeData(unsigned int address);
    protected:
    private:
        int iCacheHits, iCacheMisses;
        int dCacheHits, dCacheMisses;
        int iTlbHits, iTlbMisses;
        int dTlbHits, dTlbMisses;
        int iPageTableHits, iPageTalbeMisses;
        int dPageTableHits, dPageTableMisses;
};

#endif // MEMORYMANAGER_H
