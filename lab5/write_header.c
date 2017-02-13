/*----------------------------------------------------------------------*
 *  write_header - writes mips_asm header, along with test data         *
 *                                                                      *
 *  This shows how the mips_asm header is written, and provides         *
 *  a test case (testcase1.mb) to be used for lab projects.             *
 *----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Define the mips_asm binary header */
#include "mips_asm_header.h"

MB_HDR mb_hdr; /* Storage space for header to be written */
typedef unsigned int MIPS, *MIPS_PTR;   /* MIPS is 32-bit machine */


/* sample HW2 assignment: can use as test case 1. */

MIPS mem[] = {
  0x0C10000C,
  0x3402000A,
  0x0000000C,
  0x02114026,
  0x31050002,
  0x1088FFFE,
  0x08100010,
  0x03E00008
};


main() {
  FILE *fd;
  char outfile_name[] = "testcase1.mb";

  /* the MIPS Binary header */

  strcpy(mb_hdr.signature, "~MB");
  mb_hdr.entry = 0;
  mb_hdr.filler1 = 0;
  mb_hdr.size = sizeof(mem); /* Size of program code, bytes */
  memset(mb_hdr.filler2, 0, sizeof(mb_hdr.filler2) );

  fd = fopen(outfile_name, "wb");

  /* Now, write the header */
  fwrite((void *) &mb_hdr, sizeof(mb_hdr), 1, fd); 

  /* write the binary code */
  fwrite((void *) &mem[0], sizeof(mem), 1, fd);

  fclose(fd);

  printf("\nmips_asm program header and sample code written:" +
      "\n\t%d bytes of program code, %d bytes of header, %d bytes total.\n\n",
      mb_hdr.size, sizeof(mb_hdr), mb_hdr.size+sizeof(mb_hdr) );

  exit(0);
}
