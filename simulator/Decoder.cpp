#include "Decoder.h"

Decoder::Decoder(unsigned char *instruction)
{
    op = 0;
    instructionType = R;
    rs = 0;
    rt = 0;
    rd = 0;
    shamt = 0;
    funct = 0;
    immediate = 0;
    address = 0;
    this->instruction = (instruction[0]<<24) | (instruction[1]<<16) | (instruction[2]<<8) | instruction[3];
    op = (instruction[0] >> 2);
    if(op == 0){
        instructionType = R;

        funct = instruction[3] & 63;
        switch (funct){
            case 32:
                instructionName = "add";
                break;
            case 33:
                instructionName = "addu";
                break;
            case 34:
                instructionName = "sub";
                break;
            case 36:
                instructionName = "and";
                break;
            case 37:
                instructionName = "or";
                break;
            case 38:
                instructionName = "xor";
                break;
            case 39:
                instructionName = "nor";
                break;
            case 40:
                instructionName = "nand";
                break;
            case 42:
                instructionName = "slt";
                break;
            case 0:
                instructionName = "sll";
                break;
            case 2:
                instructionName = "srl";
                break;
            case 3:
                instructionName = "sra";
                break;
            case 8:
                instructionName = "jr";
                break;
        }
        if(funct != 0 && funct != 2 && funct != 3)
            rs = ((instruction[0] & 3)<<3) + (instruction[1]>>5);
        if(funct != 8){
            rt = instruction[1] & 31;
            rd = instruction[2] >>3;
        }
        if(funct == 0 || funct == 2 || funct == 3)
            shamt = (instruction[2] & 7) << 2 | (instruction[3] >> 6);


    }
    else if(op == 2){
        instructionType = J;
        instructionName = "j";
        address = instruction[3] | (instruction[2] << 8) | (instruction[1] << 16) | ((instruction[0] & 3) << 24);
    }
    else if(op == 3){
        instructionType = J;
        instructionName = "jal";
        address = instruction[3] | (instruction[2] << 8) | (instruction[1] << 16) | ((instruction[0] & 3) << 24);
    }
    else if(op == 63){
        instructionType = S;
        instructionName = "halt";
    }
    else{
        instructionType = I;
        rs = ((instruction[0] & 3) << 3) + (instruction[1] >> 5);
        rt = instruction[1] & 31;
        /*char temp2 = instruction[2];
        char temp3 = instruction[3];
        immediate = (temp2 << 8) + temp3;*/
       immediate = ((char)instruction[2] << 8) | instruction[3]; //sign-bit只看前面
       //immediate = (instruction[2] << 8) + (instruction[3]);//減查括號
        switch(op){
        case 8:
            instructionName = "addi";
            break;
        case 9:
            instructionName = "addiu";
            //immediate = (instruction[2] << 8) + (instruction[3]);
            break;
        case 35:
            instructionName = "lw";
            break;
        case 33:
            instructionName = "lh";
            break;
        case 37:
            instructionName = "lhu";
            break;
        case 32:
            instructionName = "lb";
            break;
        case 36:
            instructionName = "lbu";
            break;
        case 43:
            instructionName = "sw";
            break;
        case 41:
            instructionName = "sh";
            break;
        case 40:
            instructionName = "sb";
            break;
        case 15:
            instructionName = "lui";
            break;
        case 12:
            instructionName = "andi";
            immediate = (instruction[2] << 8) + (instruction[3]);
            break;
        case 13:
            instructionName = "ori";
            immediate = (instruction[2] << 8) + (instruction[3]);
            break;
        case 14:
            instructionName = "nori";
            immediate = (instruction[2] << 8) + (instruction[3]);
            break;
        case 10:
            instructionName = "slti";
            break;
        case 4:
            instructionName = "beq";
            break;
        case 5:
            instructionName = "bne";
            break;
        case 7:
            instructionName = "bgtz";
            break;
        }
    }
}

Decoder::~Decoder()
{
    //dtor
}
