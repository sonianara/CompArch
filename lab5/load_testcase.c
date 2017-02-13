/*----------------------------------------------------------------------* *  Example mips_asm program loader. This loads the mips_asm binary     *
 *  named "testcase1.mb" into an array in memory. It reads              *
 *  the 64-byte header, then loads the code into the mem array.         *
 *----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mips_asm_header.h"

typedef unsigned int MIPS, *MIPS_PTR;

MB_HDR mb_hdr;    /* Header area */
MIPS mem[1024];   /* Room for 4K bytes */

int main() {
  FILE *fd;
  int n;
  int memp;
  int byteOffset;
  unsigned int *wp;
  unsigned int wv;
  char filename[] = "testcase1.mb"; /* This is the filename to be loaded */

  /* format the MIPS Binary header */

  fd = fopen(filename, "rb");

  if (fd == NULL) {
    printf("\nCouldn't load test case - quitting.\n");
    exit(99);
  }

  /* This is the memory pointer, a byte offset */
  memp = 0;

  /* read the header and verify it. */
  fread((void *) &mb_hdr, sizeof(mb_hdr), 1, fd);

  if (! strcmp(mb_hdr.signature, "~MB")==0) {
    printf("\nThis isn't really a mips_asm binary file - quitting.\n");
    exit(98);
  }

  printf("\n%s Loaded ok, program size=%d bytes.\n\n", filename, mb_hdr.size);

  /* read the binary code a word at a time */
  do {
    n = fread((void *) &mem[memp/4], 4, 1, fd); /* note div/4 to make word index */

    if (n) {
      /* Increment byte pointer by size of instr */
      memp += 4;
    } else {
      break;       
    }

  } while (memp < sizeof(mem));

  fclose(fd);

  /* now dump out the instructions loaded */
  for (byteOffset = 0; byteOffset < memp; byteOffset += 4) {

    /* byteOffset contains byte offset addresses */
    printf("@%08X : %08X\n", byteOffset, mem[byteOffset / 4]);
    wp = (unsigned int *) &mem[byteOffset / 4];
    wv = *wp & 0b11111100000000000000000000000000;
    printf("d: %08X \n", wv);

  }

  printf("\n");
  exit(0);
}


