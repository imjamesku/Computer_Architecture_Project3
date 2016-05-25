#ifndef TLB_H
#define TLB_H


class TLB
{
    public:
        TLB(int pageTableSize);
        virtual ~TLB();
        bool* valid;
        bool* dirty;
        unsigned int* lastRefCycle;
        unsigned int* physicalAddress;
        unsigned int TLB_size;
    protected:
    private:
};

#endif // TLB_H
