#ifndef MEMORY_H
#define MEMORY_H
#include <cstdio>


class Memory
{
    public:
        Memory();
        Memory(FILE *image,unsigned int startFrom);
        unsigned char* getMemoryPointer(int offset);
        virtual ~Memory();
        unsigned char memory[1024];
        unsigned int words;
    protected:
    private:
};

#endif // MEMORY_H
