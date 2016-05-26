#ifndef CACHE_H
#define CACHE_H
#include <vector>


class Cache
{
    public:
        class CacheBlock{
            public:
                bool mru;
                bool valid;
                unsigned char* content;
                unsigned int tag;
                CacheBlock(Cache* cacheObject);
                virtual ~CacheBlock();
        };
        class CacheSet{
            public:
            std::vector<CacheBlock>* blocks;
            CacheSet(Cache* cacheObject);
            virtual ~CacheSet();
        };
        Cache(int cacheSize, int blockSize,int setAssociativity);
        virtual ~Cache();

        int cacheSize;
        int blockSize;
        int setAssociativity;
        int numOfBlocks;
        int numOfSets;
        std::vector<CacheSet>* sets;



    protected:
    private:
};

#endif // CACHE_H
