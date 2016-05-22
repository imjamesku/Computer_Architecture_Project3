#include "ProgramCounter.h"

ProgramCounter::ProgramCounter()
{
    //ctor
    PC = 0;
}
ProgramCounter::ProgramCounter(unsigned int pc){
    this->PC = pc;
}

ProgramCounter::~ProgramCounter()
{
    //dtor
}
void ProgramCounter::setValue(unsigned int rs){
    PC = rs;
}
