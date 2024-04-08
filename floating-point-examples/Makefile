
all: xmm fpu

xmm: xmm.o
	ld -o xmm.elf xmm.o

xmm.o: xmm_float.s
	as -o xmm.o xmm_float.s

fpu: fpu.o
	ld -o fpu.elf fpu.o

fpu.o: fpu_float.s
	as -o fpu.o fpu_float.s