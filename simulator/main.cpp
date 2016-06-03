#include <iostream>
#include <cstdio>
#include "MyRegister.h"
#include "ProgramCounter.h"
#include "Memory.h"
#include "Decoder.h"
#include "ControlUnit.h"
#include "MemoryManager.h"
#include <cstdlib>
#define sp 29
using namespace std;
void printSnapShot(FILE* snapShot, int cycle, MyRegister* reg, ProgramCounter* pc);
void printDebugFile(FILE* debug, int cycle, MyRegister* reg, ProgramCounter* pc);
int main(int argc, char* argv[])
{
    int iMemorySize = 64, dMemorySize = 32, iMemoryPageSize = 8, dMemoryPageSize = 16;
    int totalICacheSize = 16, iCacheBlockSize = 4, iCacheAssociativity = 4;
    int totalDCacheSize = 16, dCacheBlockSize = 4, dCacheAssociativity = 1;
    /*for(int i=0; i<argc; i++){
        printf("%s\n", argv[i]);
    }*/
    if(argc == 11){
        iMemorySize = atoi(argv[1]);
        dMemorySize = atoi(argv[2]);
        iMemoryPageSize = atoi(argv[3]);
        dMemoryPageSize = atoi(argv[4]);
        totalICacheSize = atoi(argv[5]);
        iCacheBlockSize = atoi(argv[6]);
        iCacheAssociativity = atoi(argv[7]);
        totalDCacheSize = atoi(argv[8]);
        dCacheBlockSize = atoi(argv[9]);
        dCacheAssociativity = atoi(argv[10]);
    }
    MemoryManager* memoryManager = new MemoryManager(iMemorySize, dMemorySize, iMemoryPageSize, dMemoryPageSize,
                                                     totalICacheSize, iCacheBlockSize, iCacheAssociativity, totalDCacheSize,
                                                    dCacheBlockSize, dCacheAssociativity);

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
    FILE *snapShot;
    FILE *errorFile;
    FILE* reportFile = fopen("report.rpt", "w");

    FILE* debug;

    dImage = fopen("dimage.bin", "rb");
    iImage = fopen("iimage.bin", "rb");
    snapShot = fopen("snapshot.rpt", "w");
    errorFile = fopen("error_dump.rpt", "w");
    debug = fopen("debug.rpt", "w");

    //read iimage
    result = fread(readArray, 4, 1, iImage);
    readProgramCounter = readArray[0] << 24 | readArray[1] << 16 | readArray[2] << 8 | readArray[3];
    pc = new ProgramCounter(readProgramCounter);
    iMemory = new Memory(iImage, pc->PC);

    //read dimage
    reg = new MyRegister(dImage, memoryManager);
   // reg->print();
    dMemory = new Memory(dImage, 0);

    memoryManager->initializeDisk(iMemory->memory, dMemory->memory);
    //Decoder d1(iMemory->memory + pc->PC);
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
    printDebugFile(debug, cycle, reg, pc);
    while(1){
        Decoder d3(iMemory->getMemoryPointer(pc->PC));
        Decoder testMemory(memoryManager->getIData(pc->PC, cycle));
       // printf("0x%x\n", testMemory.instruction);
        //testMemory.print();
        pc->PC += 4;
        fprintf(debug, "instruction = %x\n", d3.instruction);

        d3.printDebug(debug);

        cycle++;
        //printf("%d\n", cycle);
       // memoryManager->displayReport();
        shutDown = controlUnit->execute(&d3, cycle);//run instruction
        if(d3.instructionName == "halt" || shutDown)
            break;

        printSnapShot(snapShot, cycle, reg, pc);
        printDebugFile(debug, cycle, reg, pc);
       // reg->print();



      // system("PAUSE");
    }
   // memoryManager->displayReport();
    memoryManager->printReport(reportFile);
    delete dMemory;
    delete iMemory;
    delete controlUnit;
    delete pc;
    delete reg;
    delete memoryManager;
    fclose(iImage);
    fclose(dImage);
    fclose(snapShot);
    fclose(errorFile);
    fclose(debug);
    fclose(reportFile);
    return 0;
}
void printSnapShot(FILE* snapShot, int cycle, MyRegister* reg, ProgramCounter* pc){
    fprintf(snapShot, "cycle %d\n", cycle);
    reg->printSnapShot(snapShot);
    fprintf(snapShot, "PC: 0x%08X", pc->PC);
    fprintf(snapShot,"\n\n\n");
}
void printDebugFile(FILE* debug, int cycle, MyRegister* reg, ProgramCounter* pc){
    fprintf(debug, "cycle %d\n", cycle);
    reg->printSnapShot(debug);
    fprintf(debug, "PC: 0x%08X", pc->PC);
    fprintf(debug, "\n\n\n");
}
