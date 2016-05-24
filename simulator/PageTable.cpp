#include "PageTable.h"

PageTable::PageTable(int pageSize)
{
    //ctor
    this->pageSize = pageSize;
    valid = new bool[1024/pageSize];
    physicalAddresses = new unsigned int [1024/pageSize];

}

PageTable::~PageTable()
{
    //dtor
    delete valid;
    delete physicalAddresses;
}
