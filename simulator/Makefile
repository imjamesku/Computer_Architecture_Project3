CMP:main.o ControlUnit.o Decoder.o Memory.o ProgramCounter.o Register.o MemoryManager.o Cache.o TLB.o Pagetable.o
	g++ -std=c++11 -o CMP main.o ControlUnit.o Decoder.o Memory.o ProgramCounter.o Register.o MemoryManager.o Cache.o TLB.o Pagetable.o 
main.o:main.cpp ControlUnit.h Decoder.h Memory.h ProgramCounter.h MyRegister.h MemoryManager.h Cache.h TLB.h Pagetable.h
	g++ -std=c++11 main.cpp -c
ControlUnit.o:ControlUnit.cpp ControlUnit.h
	g++ -std=c++11 ControlUnit.cpp -c
Decoder.o:Decoder.cpp Decoder.h
	g++ -std=c++11 Decoder.cpp -c
Memory.o:Memory.cpp Memory.h
	g++ -std=c++11 Memory.cpp -c
ProgramCounter.o:ProgramCounter.cpp ProgramCounter.h
	g++ -std=c++11 ProgramCounter.cpp -c
Register.o:Register.cpp MyRegister.h
	g++ -std=c++11 Register.cpp -c
MemoryManager.o:MemoryManager.cpp MemoryManager.h
	g++ -std=c++11 MemoryManager.cpp -c
Cache.o:Cache.cpp Cache.h
	g++ -std=c++11 Cache.cpp Cache.h -c
TLB.o:TLB.cpp TLB.h
	g++ -std=c++11 TLB.cpp -c
Pagetable.o:Pagetable.cpp Pagetable.h
	g++ -std=c++11 Pagetable.cpp -c
clean:
	rm *.o