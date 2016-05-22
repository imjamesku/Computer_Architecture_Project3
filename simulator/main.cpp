#include <iostream>
#include <cstdio>
#include "MyRegister.h"
#include "ProgramCounter.h"
#include "Memory.h"
#include "Decoder.h"
#include "ControlUnit.h"
#include <cstdlib>
#define sp 29
using namespace std;
void printSnapShot(FILE* snapShot, int cycle, MyRegister* reg, ProgramCounter* pc);
void print(FILE* debug, int cycle, MyRegister* reg, ProgramCounter* pc);
int main()
{

    Memory* iMemory;
    Memory* dMemory;
    ControlUnit* controlUnit;
    MyRegister *reg;
    ProgramCounter *pc;
   // unsigned int readSp;
    size_t result;

    unsigned char readArray[4];
    unsigned int readProgramCounter;
    FILE *dImage;
    FILE *iImage;
    FILE *insOut;
    FILE *snapShot;
    FILE *errorFile;

    FILE* debug;
    
    dImage = fopen("dimage.bin", "rb");
    iImage = fopen("iimage.bin", "rb");
    insOut = fopen("ins1.txt", "w");
    snapShot = fopen("snapshot.rpt", "w");
    errorFile = fopen("error_dump.rpt", "w");
    debug = fopen("debug.rpt", "w");

    //read iimage
    result = fread(readArray, 4, 1, iImage);
    readProgramCounter = readArray[0] << 24 | readArray[1] << 16 | readArray[2] << 8 | readArray[3];
    pc = new ProgramCounter(readProgramCounter);
    iMemory = new Memory(iImage, pc->PC);

    //read dimage
    reg = new MyRegister(dImage);
   // reg->print();
    dMemory = new Memory(dImage, 0);
    Decoder d1(iMemory->memory + pc->PC);
    //d1.print();
    //printf("words = %d\n", iMemory->words);

   /* for(int i=0; i<iMemory->words; i++){
        Decoder d2(iMemory->memory + pc->PC + i*4);
        d2.print();
        d2.fprint(insOut);
    }*/

    int cycle = 0, shutDown = 0;
    controlUnit = new ControlUnit(reg, pc, dMemory, errorFile);

    printSnapShot(snapShot, cycle, reg, pc);
    print(debug, cycle, reg, pc);
    while(1){
        Decoder d3(iMemory->getMemoryPointer(pc->PC));
        pc->PC += 4;
        fprintf(debug, "instruction = %x\n", d3.instruction);

        d3.printDebug(debug);

        cycle++;

        shutDown = controlUnit->execute(&d3, cycle);//run instruction
        if(d3.instructionName == "halt" || shutDown)
            break;

        printSnapShot(snapShot, cycle, reg, pc);
        print(debug, cycle, reg, pc);
       // reg->print();
        
        

       //system("PAUSE");
    }
    delete dMemory;
    delete iMemory;
    delete controlUnit;
    delete pc;
    delete reg;
    fclose(iImage);
    fclose(dImage);
    fclose(insOut);
    fclose(snapShot);
    fclose(errorFile);
    fclose(debug);
    return 0;
}
void printSnapShot(FILE* snapShot, int cycle, MyRegister* reg, ProgramCounter* pc){
    fprintf(snapShot, "cycle %d\n", cycle);
    reg->printSnapShot(snapShot);
    fprintf(snapShot, "PC: 0x%08X", pc->PC);
    fprintf(snapShot,"\n\n\n");
}
void print(FILE* debug, int cycle, MyRegister* reg, ProgramCounter* pc){
    fprintf(debug, "cycle %d\n", cycle);
    reg->printSnapShot(debug);
    fprintf(debug, "PC: 0x%08X", pc->PC);
    fprintf(debug, "\n\n\n");
}
