all : main.c irq.c irq.h bootstrap.S
	riscv32-unknown-elf-gcc main.c irq.c bootstrap.S -o main -g3 -march=rv32ima -mabi=ilp32 -nostartfiles -Wl,--no-relax
	
sim: all
	~/ee6470/riscv-vp/vp/build/bin/riscv-vp --intercept-syscalls main
	
dump-elf: all
	riscv32-unknown-elf-readelf -a main
	
dump-code: all
	riscv32-unknown-elf-objdump -D main
	
dump-comment: all
	objdump -s --section .comment main
	
clean:
	rm -f main
