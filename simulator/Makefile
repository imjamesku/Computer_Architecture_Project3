single_cycle:main.o ControlUnit.o Decoder.o Memory.o ProgramCounter.o Register.o
	g++ -o single_cycle main.o ControlUnit.o Decoder.o Memory.o ProgramCounter.o Register.o
main.o:main.cpp ControlUnit.h Decoder.h Memory.h ProgramCounter.h MyRegister.h
	g++ main.cpp -c
ControlUnit.o:ControlUnit.cpp ControlUnit.h
	g++ ControlUnit.cpp -c
Decoder.o:Decoder.cpp Decoder.h
	g++ Decoder.cpp -c
Memory.o:Memory.cpp Memory.h
	g++ Memory.cpp -c
ProgramCounter.o:ProgramCounter.cpp ProgramCounter.h
	g++ ProgramCounter.cpp -c
Register.o:Register.cpp MyRegister.h
	g++ Register.cpp -c
clean:
	rm *.o