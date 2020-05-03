#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')



void printBinary(size_t m){
    printf(BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN "\n"
           , BYTE_TO_BINARY(m>>24)
           , BYTE_TO_BINARY(m>>16)
           , BYTE_TO_BINARY(m>>8)
           , BYTE_TO_BINARY(m));
}

void *our_memset(void *s, int c, size_t n){
    uint32_t *t_s32 = (uint32_t *)s;    /* Get 32 bit pointer to memory */
    uint8_t *t_s8;                      /* Get 8 bit pointer to memory */
    uint8_t c8 = (uint8_t)c;            /* Cast int to uint8 cccccccc */
    uint32_t c32 = c8;                  /* c32: 0000000000000000000000000cccccccc */
    size_t j, rem;
    
    c32 = (c32 << 24) | (c32 << 16) | (c32 << 8) | c32;
    /*
        cccccccc000000000000000000000000    (c32 << 24)
        00000000cccccccc0000000000000000    (c32 << 16)
        0000000000000000cccccccc00000000    (c32 << 8)
        000000000000000000000000cccccccc     c32
        cccccccccccccccccccccccccccccccc     res
     
     */
    
    rem = n & 3ul;
    printf("%ld bytes to set\n", n);
    printBinary(c8);
    printBinary(c32);
    printBinary(rem);
    printBinary(n);
    n >>= 2;
    printBinary(n); printf("\n");
    printf("%ld * 32 bits to set \n", n);
    printf("Rem %ld bytes to set left or 24 bits\n", rem);
    
    for(j = 0ul; j < n; j++)
        t_s32[j] = c32;
    
    if(rem){
        t_s8 = (uint8_t *)(t_s32 + n);   /* Get 8 bit pointer to point to the last memory location we have set */
        for(j = 0ul;j < rem; j++)
            t_s8[j] = c8;
    }
    
    return s;
}

int main(void){
    
    size_t *s = (size_t*)malloc(sizeof(size_t) * 2);

    if(!s) {
        fprintf(stderr, "Cannot allocate memory\n");
	return -1;
    }
    
    s[0] = 0xFFFFFFFFF;
    s[1] = 0xFFFFFFFFF;
    printf("Our s pointer to usigned long 64 has all bits set to 1 initialy\n");
    printBinary(s[0]);
    printBinary(s[1]);
    
    printf("----- Our memset start\n");
    our_memset((void*)s, 254u, sizeof(size_t) + 3);
    printf("----- Our memset end\n");
    
    printBinary(s[0]);
    printBinary(s[1]);
    printf("-----\n");
    
   if(s)
     free(s);

    return 0;
}

