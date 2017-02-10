#include <stdio.h>
#include <stdlib.h>

#define PRINT_BITS(L, N) printf("(L):"); print_bits((N));

void print_bits(unsigned int num) {
   unsigned int mask = 0x0001;
   unsigned char* bits = (char *) malloc(sizeof(char) * 16);
   int idx;
   for (idx = 0; idx < 16; idx++, mask = mask << 1) {
      if (mask & num) {
         bits[idx] = 1;
      } else {
         bits[idx] = 0;
      }
   }
   for (idx = 15; idx >= 0; idx--) {
      printf("%d", bits[idx]);
      if (idx % 4 == 0)
         printf(" ");
   }
   printf("\n");
}

unsigned int umultiply(unsigned int a, unsigned int b) {

   unsigned int product = 0;
   unsigned int mask = 0x0001;
   int numIterations = 16;

   int i;
   for (i = 0; i < numIterations; i++) {
      unsigned int rmb = b & mask; // right most bit

      if (rmb > 0) { 
         product += a;
      }

      PRINT_BITS("mask: ", mask);
      mask = mask << 1;
      a = a << 1;
   }

   return product;
}


unsigned int a = 0x0010;
unsigned int b = 0x0010;

void problem_1() {
   unsigned int a, b, result;

   a = 0x0001;
   b = 0x0001;
   printf("1.a) a = 0x%04X, b = 0x%04X\n", a, b);
   result = umultiply(a, b);
   printf("\t=> result = 0x%04X\n\n", result);

   a = 0x0001;
   b = 0xFFFF;
   printf("1.b) a = 0x%04X, b = 0x%04X\n", a, b);
   result = umultiply(a, b);
   printf("\t=> result: 0x%04X\n\n", result);

   a = 0x8000;
   b = 0x0001;
   printf("1.b) a = 0x%04X, b = 0x%04X\n", a, b);
   result = umultiply(a, b);
   printf("\t=> result: 0x%04X\n\n", result);

   a = 0x4000;
   b = 0x4000;
   printf("1.b) a = 0x%04X, b = 0x%04X\n", a, b);
   result = umultiply(a, b);
   printf("\t=> result: 0x%04X\n\n", result);
}

int main(int argc, const char *argv[]) {
   
   problem_1();
   return 0;
}


// Convert a C floating point number into a fixed point unsiged int variable
// float g; 
// unsigned int k;
// g = 1.25;
// k = (unsiged int) * (unsigned int *) &g;
