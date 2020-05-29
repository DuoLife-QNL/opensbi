#ifndef __SBI_MEM_TEST_H__
#define __SBI_MEM_TEST_H__

enum {
    MEM_READ,
    MEM_WRITE,
    MEM_EXEC,
    EXEC_FOO
};
void sbi_mem_test(unsigned op, unsigned long addr, unsigned long val);

#endif