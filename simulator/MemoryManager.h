#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H
#include "Memory.h"


class MemoryManager
{
    public:
        MemoryManager();
        virtual ~MemoryManager();
        unsigned char readData(unsigned int address);
        unsigned char* getInsPointer(unsigned int pc);
        void writeData(unsigned int address);
    protected:
    private:
};

#endif // MEMORYMANAGER_H
