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

Memory::~Memory()
{
    //dtor
}

unsigned char* Memory::getMemoryPointer(int offset){
    return (memory+offset);
}
