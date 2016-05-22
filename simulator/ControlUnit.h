#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H
#include <cstdio>
#include "MyRegister.h"
#include "ProgramCounter.h"
#include "Memory.h"
#include "Decoder.h"
#include <string>
#define WriteTo0 0
#define NumOverFlow 1
#define AddressOverFlow 2
#define Misalignment 3


class ControlUnit
{
    public:
        ControlUnit(MyRegister* reg, ProgramCounter* pc, Memory* dMemory, FILE* errorFile);
        virtual ~ControlUnit();
        void printError(int cycle, int errorType);
        int execute(Decoder* d1, int cycle);
        MyRegister* reg;
        ProgramCounter* pc;
        Memory* dMemory;
        FILE* errorFile;
    protected:
    private:
};

#endif // CONTROLUNIT_H
