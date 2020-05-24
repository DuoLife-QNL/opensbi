void sbi_console_putnum(int num, int len){
    if (len == 0){
        return;
    }
    printf("\nsize=%d,num=%x\n",len,num);
    if (len == 1){
        if(num<9){
            printf("%c",num + '0');
        }else if (num < 16){
            printf("%c",'a'+num-10);
        }
        return;
    }
    sbi_console_putnum(num >> (len/2)*4, len / 2);
    sbi_console_putnum(num & ((1<<(len/2)*4) -1), len/2);
}
int main(){
    sbi_console_putnum(0x1234,4);
    return 0;
}

