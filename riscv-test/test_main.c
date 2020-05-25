#include "test_main.h"
#define CSR_PMPCFG0		0x3a0
#define CSR_PMPADDR0		0x3b0
#define read_csr(reg) ({ unsigned long __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); __tmp; })
#define write_csr(reg, val) ({ \
  asm volatile ("csrw " #reg ", %0" :: "rK"(val)); })
unsigned long read_mem(unsigned long addr){
	return *(unsigned long *)addr; 
}
void write_mem(unsigned long addr, unsigned long val){
	*(unsigned long *)addr = val;
}
#define wfi()                                             \
	do {                                              \
		__asm__ __volatile__("wfi" ::: "memory"); \
	} while (0)
int test_1(){
	// csr R/W
	unsigned long a = read_csr(0x100);

        sbi_console_putnum(a, 4);
        sbi_console_putchar('\n');
        a |= 0x7;
        write_csr(0x100, a);
        a = read_csr(0x100);
        sbi_console_putnum(a, 8);
	sbi_console_puts("\ntest1 pass\n");
}
void foo(){
	sbi_console_puts("ex:foo\n");
}

void exec_mem(unsigned long addr){
	int(*p)();
	p = addr;
	p();
}
void test_2(){
	// mem R/W
	unsigned long value = 0x12345;
	sbi_console_putnum(value, 8);
	write_mem(0x80300000, value);
        sbi_console_putnum(read_mem(0x80300000), 8);
	sbi_console_puts("test2 pass");
}
void test_3(){
	// exec mem
	unsigned long addr = &foo;
	sbi_console_puts("start exec, exec: ");
	sbi_console_putnum(addr, 8);
	sbi_console_putchar('\n');
        exec_mem(&foo);
        sbi_console_puts("end exec\n");
}
int test_main(){
	
	test_2();
	wfi();
	return 0;
}
