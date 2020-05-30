echo '#define TEST_NO '$1 > mtest_no.h
echo '#define TEST_OP '$2 >> mtest_no.h
make bin
make qemu
