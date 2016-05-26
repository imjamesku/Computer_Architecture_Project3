#ifndef CACHE_H
#define CACHE_H
#include <vector>


class Cache
{
    public:
        Cache(int cacheSize, int blockSize,int setAssociativity);
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
