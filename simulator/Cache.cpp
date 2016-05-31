#include "Cache.h"
//#include "stdio.h"

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
    for(int i=0; i<numOfBlocks; i++){
        valid[i] = 0;
        tag[i] = 0;
        mru[i] = 0;
    }
    for(int i=0; i<cacheSize; i++){
        content[i] = 0;
    }
}

Cache::~Cache()
{
    //dtor

    delete [] valid;
    delete [] tag;
    delete [] mru;
    delete [] content;
}
int Cache::getBlockIndex(unsigned int physicalAddress){
    //corresponding block index
    unsigned int blockIndex = physicalAddress/blockSize;
    unsigned int setIndex = (blockIndex/setAssociativity)%numOfSets;
    unsigned int startBlockIndex = setIndex*setAssociativity;
    for(int i=0; i<setAssociativity; i++){
        if(valid[i+startBlockIndex] == 1 && tag[i+startBlockIndex] == blockIndex){
            //stored block index. different than corresponding block index because of set associativity
            return startBlockIndex + i;
        }
    }
    return -1;
}
bool Cache::isValid(int blockIndex){
    //needs to be updated before used
    return valid[blockIndex];
}
void Cache::updateMru(int index){
    mru[index] = 1;
    int setIndex = (index/setAssociativity)%numOfSets;
    int startBlockIndex = setIndex*setAssociativity;
    int sum = 0;
    for(int i=0; i<setAssociativity; i++){
        if(valid[startBlockIndex + i] && mru[startBlockIndex + i] == 1)
            sum++;
    }
    if(sum == setAssociativity){
        for(int i=0; i<setAssociativity; i++){
            mru[startBlockIndex + i] = 0;
        }
        mru[index] = 1;
    }
}
int Cache::getVictimBlockIndex(unsigned int physicalAddress){
    unsigned int setIndex = (physicalAddress/blockSize/setAssociativity)%numOfSets;
    unsigned int startBlockIndex = setIndex*setAssociativity;
  //  printf("startBlcokIndex = %d\n", startBlockIndex);
    //check if there are any invalid bits
    for(int i=0; i<setAssociativity; i++){
        if(valid[startBlockIndex + i] == 0)
            return startBlockIndex + i;
    }
    //no invalid bits, check mru
    for(int i=0; i<setAssociativity; i++){
        if(mru[startBlockIndex + i] == 0)
            return startBlockIndex + i;
    }
   // printf("no space!!\n");
    return startBlockIndex;
}
void Cache::replaceBlock(int victimBlockndex, unsigned int physicalAddress, unsigned char* content){
    valid[victimBlockndex] = 1;
    if(victimBlockndex >= numOfBlocks){
      //  printf("ououtoutoutoutoutoutoutoutoutoutoutoutt\n");
    }
    tag[victimBlockndex] = physicalAddress/blockSize;
    int victimByteIndex = victimBlockndex * blockSize;
    for(int i=0; i<blockSize; i++){
        this->content[victimByteIndex + i] = content[i];
    }
}

unsigned char* Cache::getData(int storedBlockIndex, unsigned int physicalAddress){
    unsigned int blockOffset = physicalAddress % blockSize;
    return content + (blockSize*storedBlockIndex + blockOffset);
}

void Cache::writeData(int storedBlcokIndex, unsigned int physicalAddrss, unsigned char* data, int lengthInBytes){
    unsigned int startByteIndex = storedBlcokIndex*blockSize + physicalAddrss % blockSize;
    for(int i=0; i<lengthInBytes; i++){
        content[startByteIndex + i] = data[i];
    }
}

unsigned int Cache::getBlockHeadAddress(unsigned int physicalAddress){
    return physicalAddress - physicalAddress%blockSize;
}
void Cache::deleteEntirePage(unsigned int pageHeadAddress, int pageSize){
    int numOfBlocksPerPage = pageSize/blockSize;
    int pageNeedToBeDeleted = pageHeadAddress/pageSize;
    for(int i=0; i<numOfBlocks; i++){
        if(tag[i]/numOfBlocksPerPage == pageNeedToBeDeleted){
            valid[i] = 0;
            mru[i] = 0;
        }
    }
}
