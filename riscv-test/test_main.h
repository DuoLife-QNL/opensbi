#define SBI_EXT_0_1_CONSOLE_PUTCHAR 0x1
#define SBI_EXT_0_1_MEM_TEST 0x9
typedef unsigned long           uintptr_t;
struct sbiret {
	long error;
	long value;
};

struct sbiret sbi_ecall(int ext, int fid, unsigned long arg0,
                        unsigned long arg1, unsigned long arg2,
                        unsigned long arg3, unsigned long arg4,
                        unsigned long arg5)
{
	struct sbiret ret;

	register uintptr_t a0 asm ("a0") = (uintptr_t)(arg0);
	register uintptr_t a1 asm ("a1") = (uintptr_t)(arg1);
	register uintptr_t a2 asm ("a2") = (uintptr_t)(arg2);
	register uintptr_t a3 asm ("a3") = (uintptr_t)(arg3);
	register uintptr_t a4 asm ("a4") = (uintptr_t)(arg4);
	register uintptr_t a5 asm ("a5") = (uintptr_t)(arg5);
	register uintptr_t a6 asm ("a6") = (uintptr_t)(fid);
	register uintptr_t a7 asm ("a7") = (uintptr_t)(ext);
	asm volatile ("ecall"
					: "+r" (a0), "+r" (a1)
					: "r" (a2), "r" (a3), "r" (a4), "r" (a5), "r" (a6), "r" (a7)
					: "memory");
	ret.error = a0;
	ret.value = a1;

	return ret;
}
void sbi_console_putchar(int ch)
{
        sbi_ecall(SBI_EXT_0_1_CONSOLE_PUTCHAR, 0, ch, 0, 0, 0, 0, 0);
}
void sbi_console_putOK(){
	sbi_console_putchar('O');
	sbi_console_putchar('K');
	sbi_console_putchar('\n');
}
void sbi_console_puts(char *s){
	while(*s){
		sbi_console_putchar(*s);
		s ++;
	}
}
/**
 * len is length in hex format
 */
void sbi_console_putnum(unsigned long num, unsigned int len){
    if (len == 0){
        return;
    }
    if (len == 1){
        if(num < 10){
            sbi_console_putchar(num + '0');
        }else if (num < 16){
            sbi_console_putchar('a'+num-10);
        }
        return;
    }
    sbi_console_putnum(num >> (len/2)*4, len / 2);
    sbi_console_putnum(num & ((1<<(len/2)*4) -1), len/2);
}
#define CSR_PMPCFG0             0x3a0
#define CSR_PMPADDR0            0x3b0
#define CSR_STVAL               0x143
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
    ((void(*)())addr)();
    sbi_console_puts("end exec\n");
}


#define wfi()                                             \
	do {                                              \
			__asm__ __volatile__("wfi" ::: "memory"); \
	} while (0)
void sbi_mem_test(unsigned op, unsigned long addr, unsigned long val)
{
	sbi_ecall(9, 0, op, addr, val, 0, 0, 0);
}
unsigned long check_exception(){
	unsigned long r = read_csr(0x143);
	if (r){
			sbi_console_puts("exception detected! stvl\n");
	}else{
			sbi_console_puts("no exception: stvl:");
	}
	sbi_console_putnum(r, 8);
	sbi_console_putchar('\n');
	return r;
}
void foo(){
	sbi_console_puts("ex:foo\n");
}


