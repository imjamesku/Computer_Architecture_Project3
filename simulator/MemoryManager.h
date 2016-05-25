#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H
#include "Memory.h"
#include "PageTable.h"


class MemoryManager
{
    public:
        int iMemorySize, dMemorySize, iMemoryPageSize, dMemoryPageSize;
        int totalICacheSize, iCacheBlockSize, iCacheAssociativity;
        int totalDCacheSize, dCacheBlockSize, dCacheAssociativity;
        PageTable* iPageTable;
        Memory* iMemory;
        unsigned char* iDisk;
        MemoryManager(int iMemorySize, int dMemorySize, int iMemoryPageSize, int dMemoryPageSize,
                      int totalICacheSize, int iCacheBlockSize, int iCacheAssociativity,
                      int totalDCacheSize, int dCacheBlockSize, int dCacheAssociativity);
        void initializeDisk(unsigned char* iInput);
        unsigned int getPhysicalAddressViaPageTable(unsigned int vertualAddress);
        unsigned char* getData(unsigned int vertualAddress, int cycle);
        unsigned char* getDataFromMemory(unsigned int vertualAddress);
        unsigned char* swapPages(unsigned int virtualAddress);
        virtual ~MemoryManager();
       // unsigned char readData(unsigned int address);
       // unsigned char* getInsPointer(unsigned int pc);
       // void writeData(unsigned int address);
    protected:
    private:
};

#endif // MEMORYMANAGER_H
