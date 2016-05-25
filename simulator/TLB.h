#ifndef TLB_H
#define TLB_H


class TLB
{
    public:
        TLB(int pageTableSize);
        virtual ~TLB();
        bool* valid;
        unsigned int* lastRefCycle;
        unsigned int* physicalAddress;
        int pageSize;
        int numOfEntries;
    protected:
    private:
};

#endif // TLB_H
