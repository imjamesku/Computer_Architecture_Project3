#ifndef TLB_H
#define TLB_H


class TLB
{
    public:
        //fuctions
        TLB(int pageSize);
        unsigned int getPhysicalAddress(int index, unsigned int virtualAddress);
        int getIndexToTargetPhysicalAddress(unsigned int virtualAddress);
        void updateLastRefCycle(int index, int cycle);
        int getVictimIndex()const;
        void replacePage(int index, unsigned int virtualAddress, unsigned int physicalAddress);
        virtual ~TLB();
        //variables
        bool* valid;
        unsigned int* tag;
        unsigned int* lastRefCycle;
        unsigned int* physicalPageNumber;
        int pageSize;
        int numOfEntries;
    protected:
    private:
};

#endif // TLB_H
