#ifndef TLB_H
#define TLB_H


class TLB
{
    public:
        TLB(int pageSize);
        int getPhysicalAddress(unsigned int virtualAddress);
        virtual ~TLB();
        bool* valid;
        unsigned int* tag;
        unsigned int* lastRefCycle;
        unsigned int* physicalAddress;
        int pageSize;
        int numOfEntries;
    protected:
    private:
};

#endif // TLB_H
