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
    printf(BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN "\n"
           , BYTE_TO_BINARY(m>>56)
           , BYTE_TO_BINARY(m>>48)
           , BYTE_TO_BINARY(m>>40)
           , BYTE_TO_BINARY(m>>32)
           , BYTE_TO_BINARY(m>>24)
           , BYTE_TO_BINARY(m>>16)
           , BYTE_TO_BINARY(m>>8)
           , BYTE_TO_BINARY(m));
}

void *our_memcpy(void *src, void *dst, size_t n){
    uint32_t *t_dst32 = (uint32_t *)dst;    /* Get 32 bit pointer to memory */
    uint32_t *t_src32 = (uint32_t *)src;    /* Get 32 bit pointer to memory */
    uint8_t *t_src8, *t_dst8;
    size_t j, rem = 0;
    
    rem = n & 3lu;
    n >>= 2;
    
    printf("Iterations by 32 bits:          %ld\nRem bytes to iterate by 8 bits: %ld\n", n, rem);
    for(j = 0lu; j < n; j++)
        t_dst32[j] = t_src32[j];
    
    if(rem){
        t_dst8 = (uint8_t *)(t_dst32 + n); /* Get 8 bit dst pointer to last memory we have set */
        t_src8 = (uint8_t *)(t_src32 + n); /* Get 8 bit src pointer to last memory we have read */
        /* Set rem bytes */
        for(j = 0lu; j < rem; j++)
            t_dst8[j] = t_src8[j];
    }
    
    return dst;
}

int main(void){
    
    size_t *src = (size_t*)malloc(sizeof(size_t) * 2);
    if(!src){
        fprintf(stderr, "Cannot allocate src memory");
        return -1;
    }
    src[0] = 0xFFFFFFFFFFFFFFFF;
    src[1] = 0xFFFFFFFFFFFFFFFF;
    printf("Src memory has all bits set to 1\n");
    printBinary(src[0]);
    printBinary(src[1]);
    
    size_t *dst = (size_t*)malloc(sizeof(size_t) * 2);
    if(!dst){
        if(src) free(src);
        fprintf(stderr, "Cannot allocate dst memory");
        return -1;
    }
    dst[0] = 0x0;
    dst[1] = 0x0;
    printf("Dst memory has all bits set to 0\n");
    printBinary(dst[0]);
    printBinary(dst[1]);
    
    printf("----- Our memcpy start\n");
    dst = our_memcpy((void*)src, (void*)dst, 11ul);
    printf("----- Our memcpy end\n");
    printBinary(dst[0]);
    printBinary(dst[1]);
    free(src);
    free(dst);
    return 0;
}

