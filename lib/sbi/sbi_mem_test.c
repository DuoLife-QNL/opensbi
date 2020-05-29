#include "sbi/sbi_console.h"
#include "sbi/sbi_mem_test.h"
void read_mem(unsigned long addr){
	sbi_printf("M-mode read mem @%lx\n", addr); 
    sbi_printf("ret value:0x%lx\n",*(unsigned long *)addr);
	return;
}
void write_mem(unsigned long addr, unsigned long val){
	sbi_printf("M-mode write mem @0x%lx with value %lx\n",addr,val);
    *(unsigned long *)addr = val;
}
void exec_mem(unsigned long addr){
	sbi_printf("M-mode start exec:0x%lx\n",addr);
    ((void(*)())addr)();
 	sbi_printf("\nend exec\n");
}

void sbi_mem_test(unsigned op, unsigned long addr, unsigned long val){
    switch (op)
    {
    case MEM_READ:
        read_mem(addr);
        break;
    case MEM_WRITE:
        write_mem(addr, val);
        break;
    case MEM_EXEC:
        exec_mem(addr);
        break;
    default:
        break;
    }
    return;
}