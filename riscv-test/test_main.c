#define BASE_ADDR 		0x80000000
#define FOO_FN_OFFSET 	0x000fff00
#define MEM_READ  		0 
#define MEM_WRITE 		1
#define MEM_EXEC  		2
#define EXEC_FOO  		3

#define PMP_ADDR(x)	BASE_ADDR + 0x100000 * (x + (x!=0))
#define PMP_R(x) PMP_ADDR(x)
#define PMP_W(x) PMP_ADDR(x) + 0xffff8UL
#define PMP_X(x) PMP_ADDR(x) + FOO_FN_OFFSET

#include "test_main.h"
#include "mtest_no.h"
#define TEST(x) test_##x()
#define TESTMR(x) test_m(x,MEM_READ)
#define TESTMW(x) test_m(x,MEM_WRITE)
#define TESTMX(x) test_m(x,MEM_EXEC)
#define TESTSR(x) test_s(x,MEM_READ)
#define TESTSW(x) test_s(x,MEM_WRITE)
#define TESTSX(x) test_s(x,MEM_EXEC)

#define NEGSR(x) TESTSR(x); break; case PMP_R(x):
#define NEGSW(x) TESTSW(x); break; case PMP_W(x):
#define NEGSX(x) TESTSX(x); break; case PMP_X(x):
#define NEGMR(x) TESTMR(x); break; case PMP_R(x):
#define NEGMW(x) TESTMW(x); break; case PMP_W(x):
#define NEGMX(x) TESTMX(x); break; case PMP_X(x):

void test_csr(){
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
void test_0(){
	unsigned long addr = BASE_ADDR + 0x100000 * 0;
	sbi_mem_test(EXEC_FOO, 0, 0);
	sbi_mem_test(MEM_READ, addr + 0x1ffe0, 0);
	sbi_mem_test(MEM_WRITE, addr + 0x1fff8, 0);
	sbi_console_puts("test 0 pass");
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
void test_4(){
	sbi_mem_test(0,0x80800000,0);
	sbi_mem_test(1,0x80800000,0x12345);
	sbi_mem_test(0,0x80800000,0);
	
	sbi_console_putnum(read_mem(0x80800000),8);
	write_mem(0x80800000,0x54321);
	sbi_console_putnum(read_mem(0x80800000),8);
}
void test_m(int x, unsigned op){
	unsigned long addr = PMP_ADDR(x);
	addr += (op == MEM_WRITE) * 0x1fff8UL 
		 + (op == MEM_EXEC) * FOO_FN_OFFSET;
	sbi_mem_test(op, addr, 0);
	sbi_console_puts("test ");
	sbi_console_putnum(x,1);
	if (op == MEM_READ){
		sbi_console_puts("R");
	}else if (op == MEM_WRITE){
		sbi_console_puts("W");
	}else if (op == MEM_EXEC){
		sbi_console_puts("X");
	}
	sbi_console_puts(" pass\n");
}
void test_s(int x, unsigned op){
	unsigned long addr = PMP_ADDR(x);
	if (op == MEM_READ){
		unsigned long r = read_mem(addr);
		sbi_console_puts("ret value:");
		sbi_console_putnum(r, 8);
		sbi_console_puts("\ntest ");
		sbi_console_putnum(x,1);
		sbi_console_puts("R");
		sbi_console_puts(" pass\n");
	}else if (op == MEM_WRITE){
		addr += 0xffff8UL;
		write_mem(addr, 0);
		sbi_console_puts("\ntest ");
		sbi_console_putnum(x,1);
		sbi_console_puts("W");
		sbi_console_puts(" pass\n");
	}else if (op == MEM_EXEC){
		addr += FOO_FN_OFFSET;
		exec_mem(addr);
		sbi_console_puts("test ");
		sbi_console_putnum(x,1);
		sbi_console_puts("X");
		sbi_console_puts(" pass\n");
	}
}
void MML_Pos(){
	if(check_exception()){
		return;
	}
	// MML = 1 positive
	// M mode
	TESTMR(0);
	TESTMR(2);
	TESTMR(4);
	TESTMR(6);

	TESTMW(0);
	TESTMW(6);

	// TESTMX(0); Not support & definitely pass
	TESTMX(4);
	TESTMX(10);

	// S mode 
	TESTSR(1);
	TESTSR(3);
	TESTSR(5);
	TESTSR(7);

	TESTSW(1);
	TESTSW(7);

	TESTSX(1); 
	TESTSX(5);
	TESTSX(11);

	// Shared Region
	TESTMR(14);
	TESTMX(14);
	TESTMX(12);
	TESTSX(12);
	TESTSX(14);

	TESTSR(13);
	TESTSR(15);
	TESTSW(15);
	TESTMR(13);
	TESTMR(15);
	TESTMW(13);
	TESTMW(15);
}
void MML_Neg_S(){
	switch (check_exception())
	{
	case 0:
	// L = 1 S/U
	NEGSR(0)
	NEGSW(0)
	NEGSX(0)
	NEGSR(2)
	NEGSW(2)
	NEGSX(2)
	NEGSR(4)
	NEGSW(4)
	NEGSX(4)
	NEGSR(6)
	NEGSW(6)
	NEGSX(6)
	NEGSR(8)
	NEGSW(8)
	NEGSX(8)
	NEGSR(10)
	NEGSW(10)
	NEGSX(10)
	NEGSR(12)
	NEGSW(12)
	NEGSR(14)
	NEGSW(14)
	
	default:
		break;
	}
	sbi_console_puts("if NO case passes, NEG test passes");
}
void MML_Neg_M(){
	// Usage: ./mtest_start.sh
	test_m(TEST_NO, TEST_OP);
}
/**
 * PMP_NO MAP
 * 		MML = 0	MML = 1
 * R--		3		2	
 * R--		5		4
 * RW-		7		6
 * RWX		1		0
 * ---		9		8
 * --X		b		a
 * -W-		d		c
 * -WX		f		e
 * 
 */
int test_main(){
	MML_Neg_S();
	return 0;
}
