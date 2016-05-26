#ifndef CACHE_H
#define CACHE_H
#include <vector>


class Cache
{
    public:
        Cache(int cacheSize, int blockSize,int setAssociativity);
        int getBlockIndex(unsigned int physicalAddress);
        bool isValid(unsigned int physicalAddress);
        void updateMru(int index);
        int getVictimBlockIndex(unsigned int physicalAddress);
        void replaceBlock(int victimBlockIndex, unsigned int physicalAddress, unsigned char* content);
        unsigned char* getData(int storedBlockIndex, unsigned int physicalAddress);
        virtual ~Cache();

        int cacheSize;
        int blockSize;
        int setAssociativity;
        int numOfBlocks;
        int numOfSets;
        bool* mru;
        bool* valid;
        unsigned char* content;
        unsigned int* tag;

    protected:
    private:
};

#endif // CACHE_H
