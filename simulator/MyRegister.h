#ifndef REGISTER_H
#define REGISTER_H
#include <cstdio>
#include "ProgramCounter.h"
#include "Memory.h"
#define sp 29

class MyRegister
{
    public:
        MyRegister();
        MyRegister(FILE *dImage);
        int setReg(int rt, unsigned int value);
        //R-type ins
        void add(unsigned char rs, unsigned char rt, unsigned char rd);
        void addu(unsigned char rs, unsigned char rt, unsigned char rd);
        void sub(unsigned char rs, unsigned char rt, unsigned char rd);
        void andFun(unsigned char rs, unsigned char rt, unsigned char rd);
        void orFun(unsigned char rs, unsigned char rt, unsigned char rd);
        void xorFun(unsigned char rs, unsigned char rt, unsigned char rd);
        void nor(unsigned char rs, unsigned char rt, unsigned char rd);
        void nand(unsigned char rs, unsigned char rt, unsigned char rd);
        void slt(unsigned char rs, unsigned char rt, unsigned char rd);
        void sll(unsigned char rt, unsigned char rd, unsigned char shamt);
        void srl(unsigned char rt, unsigned char rd, unsigned char shamt);
        void sra(unsigned char rt, unsigned char rd, unsigned char shamt);
        void jr(unsigned char rs, ProgramCounter *programCounter);
        // I-type ins
        void addi(unsigned char rs, unsigned char rt, unsigned int immediate);
        void addiu(unsigned char rs, unsigned char rt, unsigned int immediate);
        void lw(unsigned char rs, unsigned char rt, unsigned int immediate, Memory *dMemory);
        void lh(unsigned char rs, unsigned char rt, unsigned int immediate, Memory *dMemory);
        void lhu(unsigned char rs, unsigned char rt, unsigned int immediate, Memory *dMemory);
        void lb(unsigned char rs, unsigned char rt, unsigned int immediate, Memory *dMemory);
        void lbu(unsigned char rs, unsigned char rt, unsigned int immediate, Memory *dMemory);
        void sw(unsigned char rs, unsigned char rt, unsigned int immediate, Memory *dMemory);
        void sh(unsigned char rs, unsigned char rt, unsigned int immediate, Memory *dMemory);
        void sb(unsigned char rs, unsigned char rt, unsigned int immediate, Memory *dMemory);
        void lui(unsigned char rt, unsigned int immediate);
        void andi(unsigned char rs, unsigned char rt, unsigned int immediate);
        void ori(unsigned char rs, unsigned char rt, unsigned int immediate);
        void nori(unsigned char rs, unsigned char rt, unsigned int immediate);
        void slti(unsigned char rs, unsigned char rt, unsigned int immediate);
        void beq(unsigned char rs, unsigned char rt, unsigned int immediate, ProgramCounter *pc);
        void bne(unsigned char rs, unsigned char rt, unsigned int immediate, ProgramCounter *pc);
        void bgtz(unsigned char rs, unsigned int immediate, ProgramCounter *pc);
        //J-type ins
        void jump(unsigned int address, ProgramCounter *pc);
        void jal(unsigned int address, ProgramCounter *pc);
        //S-type ins
        void halt();

        void print();
        void printSnapShot(FILE *fp);
        virtual ~MyRegister();
        unsigned int reg[32];

    protected:
    private:
};

#endif // REGISTER_H
