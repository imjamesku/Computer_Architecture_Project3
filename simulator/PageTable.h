#ifndef PAGETABLE_H
#define PAGETABLE_H


class PageTable
{
    public:
        PageTable(int pageSize);
        virtual ~PageTable();
        bool* valid;
        unsigned int* physicalAddresses;
        int pageSize;

    protected:
    private:
};

#endif // PAGETABLE_H
