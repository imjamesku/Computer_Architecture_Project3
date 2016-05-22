#include "ControlUnit.h"

ControlUnit::ControlUnit(MyRegister* reg, ProgramCounter* pc, Memory* dMemory, FILE* errorFile)
{
    //ctor
    this->reg = reg;
    this->pc = pc;
    this->dMemory = dMemory;
    this->errorFile = errorFile;
}

ControlUnit::~ControlUnit()
{
    //dtor
}
void ControlUnit::printError(int cycle, int errorType){
    switch(errorType){
        case WriteTo0:
            fprintf(errorFile, "In cycle %d: Write $0 Error\n", cycle);
            break;
        case NumOverFlow:
            fprintf(errorFile, "In cycle %d: Number Overflow\n", cycle);
            break;
        case AddressOverFlow:
            fprintf(errorFile, "In cycle %d: Address Overflow\n", cycle);
            break;
        case Misalignment:
            fprintf(errorFile, "In cycle %d: Misalignment Error\n", cycle);
            break;
    }
}
int ControlUnit::execute(Decoder* d1, int cycle){
    int doHalt = 0;
    int doInstruction = 1;
    if(d1->instructionType == R){//write to r0 error
        if( (d1->rd == 0) && (d1->instructionName != "jr") &&
        !(d1->instructionName == "sll" && d1->rt == 0 && d1->rd == 0 && d1->shamt == 0) ){
            printError(cycle, WriteTo0);
            doInstruction = 0;
        }
    }
    else if(d1->instructionType == I){
        if(d1->instructionName != "sw" && d1->instructionName != "sh" && d1->instructionName != "sb" &&
           d1->instructionName != "beq" && d1->instructionName != "bne" && d1->instructionName != "bgtz"){
                if(d1->rt == 0){
                    printError(cycle, WriteTo0);
                    doInstruction = 0;
                }
           }
    }

    if(d1->instructionName == "add"){
       unsigned int rsSign = (reg->reg[d1->rs]) >> 31;
       unsigned int rtSign = (reg->reg[d1->rt]) >> 31;
       unsigned int rdValue = reg->reg[d1->rs] + reg->reg[d1->rt];
       unsigned int rdSign = (rdValue) >> 31;
       if(doInstruction)
            reg->add(d1->rs, d1->rt, d1->rd);
        if( rsSign == rtSign && rsSign != rdSign )
            printError(cycle, NumOverFlow);
    }
    else if(d1->instructionName == "addu"){
        if(doInstruction)
            reg->addu(d1->rs, d1->rt, d1->rd);
    }
    else if(d1->instructionName == "sub"){
        int rsSign = (reg->reg[d1->rs]) >> 31;
        //int rtSign = (reg->reg[d1->rt]) >> 31;
        int rt2sComplementSign = (~reg->reg[d1->rt] + 1) >> 31;
        unsigned int rdValue = reg->reg[d1->rs] + ~(reg->reg[d1->rt]) + 1;
        int rdSign = (rdValue) >> 31;
        if(doInstruction)
            reg->sub(d1->rs, d1->rt, d1->rd);
        
        //if( rsSign != rtSign && rsSign != rdSign)
        if(rsSign == rt2sComplementSign && rsSign != rdSign)
            printError(cycle, NumOverFlow);
    }
    else if(d1->instructionName == "and"){
        if(doInstruction)
            reg->andFun(d1->rs, d1->rt, d1->rd);
    }
    else if(d1->instructionName == "or"){
        if(doInstruction)
            reg->orFun(d1->rs, d1->rt, d1->rd);
    }
    else if(d1->instructionName == "xor"){
        if(doInstruction)
            reg->xorFun(d1->rs, d1->rt, d1->rd);
    }
    else if(d1->instructionName == "nor"){
        if(doInstruction)
            reg->nor(d1->rs, d1->rt, d1->rd);
    }
    else if(d1->instructionName == "nand"){
        if(doInstruction)
            reg->nand(d1->rs, d1->rt, d1->rd);
    }
    else if(d1->instructionName == "slt"){
        if(doInstruction)
            reg->slt(d1->rs, d1->rt, d1->rd);
    }
    else if(d1->instructionName == "sll"){
        if(doInstruction)
            reg->sll(d1->rt, d1->rd, d1->shamt);
    }
    else if(d1->instructionName == "srl"){
        if(doInstruction)
            reg->srl(d1->rt, d1->rd, d1->shamt);
    }
    else if(d1->instructionName == "sra"){
        if(doInstruction)
            reg->sra(d1->rt, d1->rd, d1->shamt);
    }
    else if(d1->instructionName == "jr")
        reg->jr(d1->rs, pc);
    // I-type
    else if(d1->instructionName == "addi"){
        unsigned int rsSign = (reg->reg[d1->rs]) >> 31;
        unsigned int immediateSign = (d1->immediate) >> 31;
        unsigned int rtValue = reg->reg[d1->rs] + d1->immediate;
        unsigned int rtSign = (rtValue) >> 31;
        if(doInstruction)
            reg->addi(d1->rs, d1->rt, d1->immediate);
        
        if( rsSign == immediateSign && rsSign != rtSign )
            printError(cycle, NumOverFlow);
    }
    else if(d1->instructionName == "addiu"){
        if(doInstruction)
            reg->addiu(d1->rs, d1->rt, d1->immediate);
    }

    else if(d1->instructionName == "lw"){
        unsigned int offset = reg->reg[d1->rs] + d1->immediate;
        if( (reg->reg[d1->rs] >> 31) == (d1->immediate >> 31) && (d1->immediate >> 31) !=  (offset>>31) )
            printError(cycle, NumOverFlow);
        if(offset + 3 >= 1024 || offset >= 1024){
            printError(cycle, AddressOverFlow);
            doInstruction = 0;
            doHalt = 1;
        }
        if(offset%4 != 0){
            printError(cycle, Misalignment);
            doInstruction = 0;
            doHalt = 1;
        }
        if(doInstruction)
            reg->lw(d1->rs, d1->rt, d1->immediate, dMemory);
    }
    else if(d1->instructionName == "lh"){
        unsigned int offset = reg->reg[d1->rs] + d1->immediate;
        if( (reg->reg[d1->rs] >> 31) == (d1->immediate >> 31) && (d1->immediate >> 31) !=  (offset>>31) )
            printError(cycle, NumOverFlow);
        if(offset + 1 >= 1024 || offset >= 1024){
            printError(cycle, AddressOverFlow);
            doInstruction = 0;
            doHalt = 1;
        }
        if(offset%2 != 0){
            printError(cycle, Misalignment);
            doInstruction = 0;
            doHalt = 1;
        }
        if(doInstruction)
            reg->lh(d1->rs, d1->rt, d1->immediate, dMemory);
    }
    else if(d1->instructionName == "lhu"){
        unsigned int offset = reg->reg[d1->rs] + d1->immediate;
        if( (reg->reg[d1->rs] >> 31) == (d1->immediate >> 31) && (d1->immediate >> 31) !=  (offset>>31) )
            printError(cycle, NumOverFlow);
        if(offset + 1 >= 1024 || offset >= 1024){
            printError(cycle, AddressOverFlow);
            doInstruction = 0;
            doHalt = 1;
        }
        if(offset%2 != 0){
            printError(cycle, Misalignment);
            doInstruction = 0;
            doHalt = 1;
        }
        if(doInstruction)
            reg->lhu(d1->rs, d1->rt, d1->immediate, dMemory);
    }
    else if(d1->instructionName == "lb"){
        unsigned int offset = reg->reg[d1->rs] + d1->immediate;
        if( (reg->reg[d1->rs] >> 31) == (d1->immediate >> 31) && (d1->immediate >> 31) !=  (offset>>31) )
            printError(cycle, NumOverFlow);
        if(offset  >= 1024 ){
            printError(cycle, AddressOverFlow);
            doInstruction = 0;
            doHalt = 1;
        }
        if(doInstruction)
            reg->lb(d1->rs, d1->rt, d1->immediate, dMemory);
    }
    else if(d1->instructionName == "lbu"){
        unsigned int offset = reg->reg[d1->rs] + d1->immediate;
        if( (reg->reg[d1->rs] >> 31) == (d1->immediate >> 31) && (d1->immediate >> 31) !=  (offset>>31) )
            printError(cycle, NumOverFlow);
        if(offset  >= 1024){
            printError(cycle, AddressOverFlow);
            doInstruction = 0;
            doHalt = 1;
        }
        if(doInstruction)
            reg->lbu(d1->rs, d1->rt, d1->immediate, dMemory);
    }
    else if(d1->instructionName == "sw"){
        unsigned int offset = reg->reg[d1->rs] + d1->immediate;
        if( (reg->reg[d1->rs] >> 31) == (d1->immediate >> 31) && (d1->immediate >> 31) !=  (offset>>31) )
            printError(cycle, NumOverFlow);
        if(offset + 3 >= 1024 || offset >= 1024){
            printError(cycle, AddressOverFlow);
            doInstruction = 0;
            doHalt = 1;
        }
        if(offset%4 != 0){
            printError(cycle, Misalignment);
            doInstruction = 0;
            doHalt = 1;
        }
        if(doInstruction)
            reg->sw(d1->rs, d1->rt, d1->immediate, dMemory);
    }
    else if(d1->instructionName == "sh"){
        unsigned int offset = reg->reg[d1->rs] + d1->immediate;
        if( (reg->reg[d1->rs] >> 31) == (d1->immediate >> 31) && (d1->immediate >> 31) !=  (offset>>31) )
            printError(cycle, NumOverFlow);
        if(offset + 1 >= 1024 || offset >= 1024){
            printError(cycle, AddressOverFlow);
            doInstruction = 0;
            doHalt = 1;
        }
        if(offset%2 != 0){
            printError(cycle, Misalignment);
            doInstruction = 0;
            doHalt = 1;
        }
        if(doInstruction)
            reg->sh(d1->rs, d1->rt, d1->immediate, dMemory);
    }
    else if(d1->instructionName == "sb"){
        unsigned int offset = reg->reg[d1->rs] + d1->immediate;
        if( (reg->reg[d1->rs] >> 31) == (d1->immediate >> 31) && (d1->immediate >> 31) !=  (offset>>31) )
            printError(cycle, NumOverFlow);
        if(offset >= 1024){
            printError(cycle, AddressOverFlow);
            doInstruction = 0;
            doHalt = 1;
        }
        if(doInstruction)
            reg->sb(d1->rs, d1->rt, d1->immediate, dMemory);
    }
    else if(d1->instructionName == "lui"){
        if(doInstruction)
            reg->lui(d1->rt, d1->immediate);
    }
    else if(d1->instructionName == "andi"){
        if(doInstruction)
            reg->andi(d1->rs, d1->rt, d1->immediate);
    }
    else if(d1->instructionName == "ori"){
        if(doInstruction)
            reg->ori(d1->rs, d1->rt, d1->immediate);
    }
    else if(d1->instructionName == "nori"){
        if(doInstruction)
            reg->nori(d1->rs, d1->rt, d1->immediate);
    }
    else if(d1->instructionName == "slti"){
        if(doInstruction)
            reg->slti(d1->rs, d1->rt, d1->immediate);
    }
    else if(d1->instructionName == "beq"){
        unsigned int pcSign = pc->PC >> 31;
        unsigned int cTimes4Sign = d1->immediate << 2;
        cTimes4Sign >>= 31;
        reg->beq(d1->rs, d1->rt, d1->immediate, pc);
        unsigned int pcAfterSign = pc->PC >> 31;
        if(pcSign == cTimes4Sign && pcSign != pcAfterSign)
            printError(cycle, NumOverFlow);
    }
    else if(d1->instructionName == "bne"){
        unsigned int pcSign = pc->PC >> 31;
        unsigned int cTimes4Sign = d1->immediate << 2;
        cTimes4Sign >>= 31;
        reg->bne(d1->rs, d1->rt, d1->immediate, pc);
            unsigned int pcAfterSign = pc->PC >> 31;
        if(pcSign == cTimes4Sign && pcSign != pcAfterSign)
            printError(cycle, NumOverFlow);
        }
    else if(d1->instructionName == "bgtz"){
        reg->bgtz(d1->rs, d1->immediate, pc);
    }
    // J-type
    else if(d1->instructionName == "j"){
        reg->jump(d1->address, pc);
    }
    else if(d1->instructionName == "jal"){
        reg->jal(d1->address, pc);
    }

    return doHalt;
}
