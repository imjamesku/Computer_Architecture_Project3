#include "TLB.h"

TLB::TLB(int pageTableSize)
{
    //ctor
    this->TLB_size = pageTableSize / 4;
    this->dirty = new bool[TLB_size];
    this->valid = new bool[TLB_size];
    this->lastRefCycle = new unsigned int[TLB_size];
    this->physicalAddress = new unsigned int[TLB_size];

}

TLB::~TLB()
{
    //dtor
    delete dirty;
    delete valid;
    delete lastRefCycle;
    delete physicalAddress;
}
