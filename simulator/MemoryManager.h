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
        unsigned char* iMemory;
        unsigned char* iDisk;
        MemoryManager(int iMemorySize, int dMemorySize, int iMemoryPageSize, int dMemoryPageSize,
                      int totalICacheSize, int iCacheBlockSize, int iCacheAssociativity,
                      int totalDCacheSize, int dCacheBlockSize, int dCacheAssociativity);
        void initializeDisk(unsigned char* iInput);
        virtual ~MemoryManager();
       // unsigned char readData(unsigned int address);
       // unsigned char* getInsPointer(unsigned int pc);
       // void writeData(unsigned int address);
    protected:
    private:
};

#endif // MEMORYMANAGER_H
