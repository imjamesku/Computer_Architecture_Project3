#include "Cache.h"
#include "stdio.h"

Cache::Cache(int cacheSize, int blockSize,int setAssociativity)
{
    //ctor
    this->cacheSize = cacheSize;
    this->blockSize = blockSize;
    this->setAssociativity = setAssociativity;
    this->numOfBlocks = cacheSize/blockSize;
    this->numOfSets = numOfBlocks/setAssociativity;
    this->valid = new bool[numOfBlocks];
    this->tag = new unsigned int[numOfBlocks];
    this->mru = new bool[numOfBlocks];
    this->content = new unsigned char[cacheSize];
   // this->sets = new std::vector<CacheSet>(numOfSets, CacheSet(this));
}

Cache::~Cache()
{
    //dtor
    delete [] valid;
    delete [] tag;
    delete [] mru;
    delete [] content;
}

