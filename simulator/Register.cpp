#include "MyRegister.h"
MyRegister::MyRegister(){
    for(int i=0; i<32; i++){
        reg[i] = 0;
    }
}
MyRegister::MyRegister(FILE* dImage)
{
    //ctor
    for(int i=0; i<32; i++){
        reg[i] = 0;
    }
    unsigned char temp[4] = {0};
    fread(temp, 4, 1, dImage);
    reg[sp] = temp[3] | (temp[2] << 8) | (temp[1] << 16) | (temp[0] << 24);
   // fread(&reg[sp*4], 4, 1, dImage);
}
MyRegister::~MyRegister()
{
    //dtor
}
int MyRegister::setReg(int rt, unsigned int value){
    if(rt == 0)
        return 1;
    else{
        reg[rt] = value;
        return 0;
    }
}
void MyRegister::print(){
    for(int i=0; i<32; i++){
        printf("$%02d: 0x%08X\n", i, reg[i]);
    }
}
void MyRegister::printSnapShot(FILE *fp){
    for(int i=0; i<32; i++){
        fprintf(fp,"$%02d: 0x%08X\n", i, reg[i]);
    }
}
// R_type ins

void MyRegister::add(unsigned char rs, unsigned char rt, unsigned char rd){
    reg[rd] = reg[rs] + reg[rt];
}
void MyRegister::addu(unsigned char rs, unsigned char rt, unsigned char rd){
    reg[rd] = reg[rs] + reg[rt];
}
void MyRegister::sub(unsigned char rs, unsigned char rt, unsigned char rd){
    reg[rd] = reg[rs] + ~reg[rt] + 1;
}
void MyRegister::andFun(unsigned char rs, unsigned char rt, unsigned char rd){
    reg[rd] = reg[rs] & reg[rt];
}
void MyRegister::orFun(unsigned char rs, unsigned char rt, unsigned char rd){
    reg[rd] = reg[rs] | reg[rt];
}
void MyRegister::xorFun(unsigned char rs, unsigned char rt, unsigned char rd){
    reg[rd] = reg[rs] ^ reg[rt];
}
void MyRegister::nor(unsigned char rs, unsigned char rt, unsigned char rd){
    reg[rd] = ~(reg[rs] | reg[rt]);
}
void MyRegister::nand(unsigned char rs, unsigned char rt, unsigned char rd){
    reg[rd] = ~(reg[rs] & reg[rt]);
}
void MyRegister::slt(unsigned char rs, unsigned char rt, unsigned char rd){
   /* if(reg[rs]>>31 == reg[rt]>>31 == 0)
        reg[rd] = reg[rs] < reg[rt];
    else if(reg[rs]>>31 == reg[rt]>>31 == 1)
        reg[rd] = reg[rs] > reg[rt];
    else if(reg[rs]>>31 == 1)
        reg[rd] = 1;
    else
        reg[rd] = 0;*/
    int ts = (int)reg[rs];
    int tt = (int) reg[rt];
    reg[rd] = ts < tt;
}
void MyRegister::sll(unsigned char rt, unsigned char rd, unsigned char shamt){
    reg[rd] = reg[rt] << shamt;
}
void MyRegister::srl(unsigned char rt, unsigned char rd, unsigned char shamt){
    reg[rd] = reg[rt] >> shamt;
}
void MyRegister::sra(unsigned char rt, unsigned char rd, unsigned char shamt){
    /*reg[rd] = reg[rt] >> shamt;
    unsigned int sum = 1;
    for(int i=1; i<shamt; i++){
        sum << 1;
        sum += 1;
    }
    sum <<= 32-shamt;
    reg[rd] = reg[rd] | sum;*/
    int t = (int)reg[rt];
    reg[rd] = t >> shamt;
}
void MyRegister::jr(unsigned char rs, ProgramCounter *programCounter){
    programCounter -> setValue(reg[rs]);
}

//I-type instructions
void MyRegister::addi(unsigned char rs, unsigned char rt, unsigned int immediate){
    reg[rt] = reg[rs] + immediate;
}
void MyRegister::addiu(unsigned char rs, unsigned char rt, unsigned int immediate){
    reg[rt] = reg[rs] + immediate;
}
void MyRegister::lw(unsigned char rs, unsigned char rt, unsigned int immediate, Memory *dMemory){
    unsigned int offset = reg[rs] + immediate;
    reg[rt] = dMemory->memory[offset] << 24 | dMemory->memory[offset+1] << 16 | dMemory->memory[offset+2] << 8 | dMemory->memory[offset+3];
}
void MyRegister::lh(unsigned char rs, unsigned char rt, unsigned int immediate, Memory *dMemory){
    unsigned int offset = reg[rs] + immediate;
    if( (dMemory->memory[offset] >> 7) == 0 ){
        reg[rt] = dMemory->memory[offset] << 8 | dMemory->memory[offset+1];
    }
    else{
        reg[rt] = dMemory->memory[offset] << 8 | dMemory->memory[offset+1];
        unsigned int t = 0xFFFF0000;
        reg[rt] = reg[rt] | t;
    }

}
void MyRegister::lhu(unsigned char rs, unsigned char rt, unsigned int immediate, Memory *dMemory){
    unsigned int offset = reg[rs] + immediate;
    reg[rt] = dMemory->memory[offset] << 8 | dMemory->memory[offset+1];

}
void MyRegister::lb(unsigned char rs, unsigned char rt, unsigned int immediate, Memory *dMemory){
    unsigned int offset = reg[rs] + immediate;
    if( (dMemory->memory[offset] >> 7) == 0 ){
        reg[rt] = dMemory->memory[offset];
    }
    else{
        reg[rt] = dMemory->memory[offset];
        unsigned int t = 0xFFFFFF00;
        reg[rt] = reg[rt] | t;
    }

}
void MyRegister::lbu(unsigned char rs, unsigned char rt, unsigned int immediate, Memory *dMemory){
    unsigned int offset = reg[rs] + immediate;
    reg[rt] = dMemory->memory[offset];
}
void MyRegister::sw(unsigned char rs, unsigned char rt, unsigned int immediate, Memory *dMemory){
    unsigned int offset = reg[rs] + immediate;
    dMemory->memory[offset] = reg[rt] >> 24;
    dMemory->memory[offset+1] = reg[rt] >> 16;
    dMemory->memory[offset+2] = reg[rt] >> 8;
    dMemory->memory[offset+3] = reg[rt];
}
void MyRegister::sh(unsigned char rs, unsigned char rt, unsigned int immediate, Memory *dMemory){
    unsigned int offset = reg[rs] + immediate;
    dMemory->memory[offset] = reg[rt] >> 8;
    dMemory->memory[offset+1] = reg[rt];
}
void MyRegister::sb(unsigned char rs, unsigned char rt, unsigned int immediate, Memory *dMemory){
    unsigned int offset = reg[rs] + immediate;
    dMemory->memory[offset] = reg[rt];
}
void MyRegister::lui(unsigned char rt, unsigned int immediate){
    reg[rt] = immediate << 16;
}
void MyRegister::andi(unsigned char rs, unsigned char rt, unsigned int immediate){
    //immediate = immediate | 0xFFFF0000;wait for reply
    reg[rt] = reg[rs] & immediate;
}
void MyRegister::ori(unsigned char rs, unsigned char rt, unsigned int immediate){
    reg[rt] = reg[rs] | immediate;
}
void MyRegister::nori(unsigned char rs, unsigned char rt, unsigned int immediate){
    reg[rt] = ~(reg[rs] | immediate);
}
void MyRegister::slti(unsigned char rs, unsigned char rt, unsigned int immediate){
    int signedRs = (int)reg[rs];
    int signedC = (int)immediate;
    reg[rt] = signedRs < signedC;
}
void MyRegister::beq(unsigned char rs, unsigned char rt, unsigned int immediate, ProgramCounter *pc){
    if(reg[rs] == reg[rt])
        pc->PC += 4*immediate;
}
void MyRegister::bne(unsigned char rs, unsigned char rt, unsigned int immediate, ProgramCounter *pc){
    if(reg[rs] != reg[rt])
        pc->PC += 4*immediate;
}
void MyRegister::bgtz(unsigned char rs, unsigned int immediate, ProgramCounter *pc){
    unsigned int signBit = reg[rs] >> 31;
    if(signBit == 0 && reg[rs] != 0)
        pc->PC += 4*immediate;
}
//J-type ins
void MyRegister::jump(unsigned int address, ProgramCounter *pc){
    unsigned int pcFrom31To28;
    pcFrom31To28 = pc->PC & 0xF0000000;
    pc->PC = pcFrom31To28 | (4*address);
}
void MyRegister::jal(unsigned int address, ProgramCounter *pc){
    reg[31] = pc->PC;
    unsigned int pcFrom31To28 = pc->PC & 0xF0000000;
    pc->PC = pcFrom31To28 | (address <<2 );
}

void MyRegister::halt(){
    return;
}

