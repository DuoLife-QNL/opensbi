#include "test_main.h"
#define CSR_PMPCFG0		0x3a0
#define CSR_PMPADDR0		0x3b0
#define CSR_STVAL		0x143
#define read_csr(reg) ({ unsigned long __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); __tmp; })
#define write_csr(reg, val) ({ \
  asm volatile ("csrw " #reg ", %0" :: "rK"(val)); })
unsigned long read_mem(unsigned long addr){
	sbi_console_puts("S/U mode read mem @");
	sbi_console_putnum(addr, 8);
	sbi_console_putchar('\n');
	return *(unsigned long *)addr; 
}
void write_mem(unsigned long addr, unsigned long val){
	sbi_console_puts("S/U mode write mem @");
    sbi_console_putnum(addr, 8);
	sbi_console_puts(" with value ");
    sbi_console_putnum(val, 8);
	sbi_console_putchar('\n');
	*(unsigned long *)addr = val;
	sbi_console_puts("end write");
}
void exec_mem(unsigned long addr){
	sbi_console_puts("S/U mode start exec:");
	sbi_console_putnum(addr, 8);
    sbi_console_putchar('\n');
    int(*p)();
    p = addr;
    p();
 	sbi_console_puts("end exec\n");
}


#define wfi()                                             \
	do {                                              \
		__asm__ __volatile__("wfi" ::: "memory"); \
	} while (0)
void test_1(){
	// csr R/W
	unsigned long a = read_csr(0x100);

        sbi_console_putnum(a, 4);
        sbi_console_putchar('\n');
        a |= 0x7;
        write_csr(0x100, a);
        a = read_csr(0x100);
        sbi_console_putnum(a, 8);
	sbi_console_puts("\ntest1 pass\n");
	return ;
}
int check_no_exception(){
        unsigned long r = read_csr(0x143);
	if (r == 0){
		sbi_console_puts("no exception: stvl:");
	}else{
                sbi_console_puts("exception detected!\n");
        }
	sbi_console_putnum(r, 8);
	sbi_console_putchar('\n');
	return r == 0;
}

void foo(){
	sbi_console_puts("ex:foo\n");
}

void test_2(){
	// mem R/W
	unsigned long value = 0x00012345;
	write_mem(0x80300000, value);
    	sbi_console_putnum(read_mem(0x80300000), 8);
	
	sbi_console_puts("\ntest2 pass\n");
	return;
}
void test_3(){
	// exec mem
	unsigned long addr = &foo;
        exec_mem(&foo);
	return;
}
void sbi_mem_test(unsigned op, unsigned long addr, unsigned long val)
{
        sbi_ecall(9, 0, op, addr, val, 0, 0, 0);
}
int test_main(){
	if(!check_no_exception()){
		return 0;
	}
	sbi_mem_test(0,0x80800000,0);
	sbi_mem_test(1,0x80800000,0x12345);
	sbi_mem_test(0,0x80800000,0);
	
//	sbi_console_putnum(read_mem(0x80800000),8);
	write_mem(0x80800000,0x54321);
	sbi_console_putnum(read_mem(0x80800000),8);
	return 0;
}
