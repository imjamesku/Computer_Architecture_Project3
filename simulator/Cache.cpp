#include "Cache.h"

Cache::Cache(int cacheSize, int blockSize,int setAssociativity)
{
    //ctor
    this->cacheSize = cacheSize;
    this->blockSize = blockSize;
    this->setAssociativity = setAssociativity;
    this->numOfBlocks = cacheSize/blockSize;
    this->numOfSets = numOfBlocks/setAssociativity;
    this->sets = new std::vector<CacheSet>(numOfSets, CacheSet(this));
}

Cache::~Cache()
{
    //dtor
    delete sets;
}

Cache::CacheSet::CacheSet(Cache* cacheObject){
 //   this->blocks = new CacheBlock[setAssoviativity];
    this->blocks = new std::vector<CacheBlock>(cacheObject->numOfBlocks, CacheBlock(cacheObject));
}
Cache::CacheSet::~CacheSet(){
    delete blocks;
}
Cache::CacheBlock::CacheBlock(Cache* cacheObject){
    this->mru = 0;
    this->tag = 0;
    this->valid = 0;
    this->content = new unsigned char[cacheObject->blockSize];
}
Cache::CacheBlock::~CacheBlock(){
    delete content;
}
