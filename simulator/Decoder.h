#ifndef DECODER_H
#define DECODER_H
#include <string>
#include <cstdio>
#include <iostream>
enum type{R, I, J, S};

class Decoder
{
    public:
        Decoder(unsigned char *instruction);
        virtual ~Decoder();
    void print(){
        std::cout << instructionName;
      //  printf("\topcode = %x", op);
        if(instructionType == R){
            printf(" rs = %d, rt = %d, rd = %d, shamt = %x, funct = %x", rs, rt, rd, shamt, funct);
        }
        else if(instructionType == I){
            printf(" rs = %d, rt = %d, C = %x", rs, rt, immediate);
        }
        else if(instructionType == J){
            printf(" address = %x", address);
        }
        printf("\n");
    }
    void printDebug(FILE* debug){
        //std::cout << instructionName;
        fprintf(debug, "%s", instructionName.c_str());
      //  printf("\topcode = %x", op);
        if(instructionType == R){
            fprintf(debug, " rs = %d, rt = %d, rd = %d, shamt = %x, funct = %x", rs, rt, rd, shamt, funct);
        }
        else if(instructionType == I){
            fprintf(debug, " rs = %d, rt = %d, C = %x", rs, rt, immediate);
        }
        else if(instructionType == J){
            fprintf(debug, " address = %x", address);
        }
        fprintf(debug, "\n");
    }
    void fprint(FILE* out){
        //std::cout << instructionName;
        fprintf(out, "%s", instructionName.data());
      //  printf("\topcode = %x", op);
        if(instructionType == R){
            fprintf(out," rs = %d, rt = %d, rd = %d, shamt = %x, funct = %x", rs, rt, rd, shamt, funct);
        }
        else if(instructionType == I){
            fprintf(out," rs = %d, rt = %d, C = %x", rs, rt, immediate);
        }
        else if(instructionType == J){
            fprintf(out," address = %x", address);
        }
        fprintf(out,"\n");
    }
    unsigned char op, rs, rt, rd, shamt, funct;
    unsigned int immediate;
    unsigned int address;
    unsigned int instruction;
    std::string instructionName;
    type instructionType;
    protected:
    private:
};

#endif // DECODER_H
