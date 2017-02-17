/*----------------------------------------------------------------------* *  Example mips_asm program loader. This loads the mips_asm binary     *
 *  named "testcase1.mb" into an array in memory. It reads              *
 *  the 64-byte header, then loads the code into the mem array.         *
 *----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mips_asm_header.h"
#include "load_testcase.h"

#define VARIABLE 1 
#define FIXED 2 
#define OTHER 0

typedef unsigned int MIPS, *MIPS_PTR;

MB_HDR mb_hdr;    /* Header area */
MIPS mem[1024];   /* Room for 4K bytes */

int main() {
  FILE *fd;
  int n;
  int memp;
  int byteOffset;
  unsigned int *wp;
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

    char type = getType(wp);
    int funcCode = getFuncCode(wp);
    int opCode = getOpcode(wp);
    int rs = getRS(wp);
    int rd = getRD(wp);
    int rt = getRT(wp);
    double imm = getImm(wp);
    double eff = getEff(wp, byteOffset);
    int shamt = getShamt(wp);
    char funcName[5];
    char IJName[6];
    getFuncName(funcCode, funcName);
    getFuncName(funcCode, IJName);

    /*R-Type*/
    printf("\tAddress:\t [0x%X]\n", byteOffset);

    if (type == 'R') {
      printf("\tName:\t\t [%s]\n", funcName);
      printf("\tType:\t\t [%c]\n", type);
      printf("\tFuncCode:\t [%0X]\n", funcCode);
      printf("\tRs:\t\t [%d]\n", rs);
      printf("\tRt:\t\t [%d]\n", rt);
      printf("\tRd:\t\t [%d]\n", rd);

      if (isShift(funcCode) == 'F') {
        printf("\tShamt\t [%d]\n", shamt);

      } else if (isShift(funcCode) == 'V') {
        printf("\tShamt:\t [%d]\n", shamt);
        printf("\tShiftReg:\t\t [%d]\n", rt);
      }
    }

    else if (type == 'I') {
      printf("\tName:\t\t [%s]\n", IJName);
      printf("\tType:\t\t [%c]\n", type);
      printf("\tRs:\t\t [%d]\n", rs);
      printf("\tRt:\t\t [%d]\n", rt);
      printf("\tImm:\t\t [%lf]\n", imm);
      if (isBranch(funcCode)) {
        printf("\t*BRANCH*");
        printf("\tEffAddr:\t [%s]\n", "??");
        // ADD PC + R[Rs]
      }
    }

    else if (type == 'J') {
      printf("\tName:\t\t [%s]\n", IJName);
      printf("\tType:\t\t [%c]\n", type);
      printf("\tEffAddr:\t [%lf]\n", eff);
    }

    else {
      printf("INVALID INSTRUCTION\n");
    }


    /*I-Type*/
    //printf("Address\t Type\t Name\t ", opCode, funcName, rs);

    /*J-Type*/
    //printf("Address\t Type\t Name\t ", opCode, funcName, rs);

   // printf("\tType: [%c] \tName: [%s] \tRs: [%d]\n\n", opCode, funcName, rs);
    printf("\n\n");
  }

  printf("\n");
  exit(0);
}

char getType(unsigned int *wp) {
  unsigned int wv;
  wv = *wp >> 26;
  switch (wv) {
    case 0x0: return 'R';
    case 0x02: return 'J';
    case 0x03: return 'J';
    case 0x08: return 'I';
    case 0x09: return 'I';
    case 0x0C: return 'I';
    case 0x0D: return 'I';
    case 0x0E: return 'I';
    case 0x0A: return 'I';
    case 0x0B: return 'I';
    case 0x04: return 'I';
    case 0x05: return 'I';
    case 0x20: return 'I';
    case 0x24: return 'I';
    case 0x21: return 'I';
    case 0x25: return 'I';
    case 0x0F: return 'I';
    case 0x23: return 'I';
    case 0x28: return 'I';
    case 0x29: return 'I';
    case 0x2B: return 'I';
    default: return ' ';
  }
}

int getOpcode(unsigned int *wp) {
  return *wp >> 26;
}
int getFuncCode(unsigned int *wp) {
  return *wp & 0b00000000000000000000000000111111;
}

void getFuncName(int funcCode, char *funcName) {
  switch (funcCode) {
    case 0x20: strcpy(funcName, "add"); break;
    case 0x21: strcpy(funcName, "addu"); break;
    case 0x22: strcpy(funcName, "sub"); break;
    case 0x23: strcpy(funcName, "subu"); break;
    case 0x24: strcpy(funcName, "and"); break;
    case 0x27: strcpy(funcName, "nor"); break;
    case 0x25: strcpy(funcName, "or"); break;
    case 0x26: strcpy(funcName, "xor"); break;
    case 0x00: strcpy(funcName, "sll"); break;
    case 0x02: strcpy(funcName, "srl"); break;
    case 0x03: strcpy(funcName, "sra"); break;
    case 0x04: strcpy(funcName, "sllv"); break;
    case 0x06: strcpy(funcName, "srlv"); break;
    case 0x07: strcpy(funcName, "srav"); break;
    case 0x2a: strcpy(funcName, "slt"); break;
    case 0x2b: strcpy(funcName, "sltu"); break;
    case 0x08: strcpy(funcName, "jr"); break;
    case 0x09: strcpy(funcName, "jalr"); break;
    default: strcpy(funcName, "other"); break;
  }
}

void getIJName(int wv, char *funcName) {
  switch (wv) {
    case 0x08: strcpy(funcName, "addi"); break;
    case 0x09: strcpy(funcName, "addiu"); break;
    case 0x0C: strcpy(funcName, "andi"); break;
    case 0x0D: strcpy(funcName, "ori"); break;
    case 0x0E: strcpy(funcName, "xori"); break;
    case 0x0A: strcpy(funcName, "slti"); break;
    case 0x0B: strcpy(funcName, "sltiu"); break;
    case 0x04: strcpy(funcName, "beq"); break;
    case 0x05: strcpy(funcName, "bne"); break;
    case 0x02: strcpy(funcName, "j"); break;
    case 0x03: strcpy(funcName, "jal"); break;
    case 0x20: strcpy(funcName, "lb"); break;
    case 0x24: strcpy(funcName, "lbu"); break;
    case 0x21: strcpy(funcName, "lh"); break;
    case 0x25: strcpy(funcName, "lhu"); break;
    case 0x0F: strcpy(funcName, "lui"); break;
    case 0x23: strcpy(funcName, "lw"); break;
    case 0x28: strcpy(funcName, "sb"); break;
    case 0x29: strcpy(funcName, "sh"); break;
    case 0x2B: strcpy(funcName, "sw"); break;
  }
}

int getRS(unsigned int *wp) {
  unsigned int wv = *wp >> 21;
  return wv & 0b00000000000000000000000000011111;
}

int getRT(unsigned int *wp) {
  unsigned int wv = *wp >> 16;
  return wv & 0b00000000000000000000000000011111;
}
int getRD(unsigned int *wp) {
  unsigned int wv = *wp >> 11;
  return wv & 0b00000000000000000000000000011111;
}
int getShamt(unsigned int *wp) {
  unsigned int wv = *wp >> 6;
  return wv & 0b00000000000000000000000000011111;
}

/* F: 1, V: 2, Other: 0 */
int isShift(int funcCode) {
  switch (funcCode) {
    case 0x00:
    case 0x02:
    case 0x03:
      return VARIABLE;
    case 0x04:
    case 0x06:
    case 0x07:
      return FIXED;
    default:
      return OTHER;
  }
}

double getImm(unsigned int *wp) {
  return *wp & 0b00000000000000001111111111111111;
}

double getEff(unsigned int *wp, int branchOffset) {
  return *wp & 0b00000011111111111111111111111111 + branchOffset + 4;
  
}

int isBranch(int funcCode) {
  switch (funcCode) {
    case 0x04: return 1;
    case 0x05: return 1;
    default: return 0;
  }
}

