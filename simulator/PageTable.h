#ifndef PAGETABLE_H
#define PAGETABLE_H


class PageTable
{
    public:
        PageTable(int pageSize);
        virtual ~PageTable();
        bool* isInMemory;
        bool* dirty;
        unsigned int* lastRefCycle;
        unsigned int* physicalAddresses;
        int pageSize;
        int numOfEntries;

    protected:
    private:
};

#endif // PAGETABLE_H
