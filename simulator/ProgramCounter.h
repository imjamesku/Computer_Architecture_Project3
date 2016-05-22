#ifndef PROGRAMCOUNTER_H
#define PROGRAMCOUNTER_H


class ProgramCounter
{
    public:
        ProgramCounter();
        ProgramCounter(unsigned int pc);
        virtual ~ProgramCounter();
        void setValue(unsigned int rs);
        unsigned int PC;
    protected:
    private:
};

#endif // PROGRAMCOUNTER_H
