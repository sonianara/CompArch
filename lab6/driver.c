/*----------------------------------------------------------------------*
 *  Example mips_asm program loader. This loads the mips_asm binary     *
 *  named "testcase1.mb" into an array in memory. It reads              *
 *  the 64-byte header, then loads the code into the mem array.         *
 *----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mips_asm_header.h"
#include "load_testcase.h"

#define FILENAME "countbits_benchmark2.mb"
//#define FILENAME "diagnostics.mb"
//#define FILENAME "simple_add.mb"

#define VARIABLE 1
#define FIXED 2
#define OTHER 0

#define R_TYPE 0
#define I_TYPE 1
#define J_TYPE 2
#define INVALID_TYPE 2

#define SINGLE_STEP 1
#define RUN 2

typedef unsigned int MIPS, *MIPS_PTR;

MB_HDR mb_hdr;    /* Header area */
MIPS mem[1024];   /* Room for 4K bytes */
unsigned int Reg[32];
unsigned int pc;
/* This is the memory pointer, a byte offset */
int memOffset = 0;
int instructionCount = 0;
int memRefCount = 0;
int exitTriggered = 0;
int userMemoryBase = 300;
int entryPoint;
int totalClockCycles = 0;
//int mockEntryPoint = 4;

int main() {
  int mode;
  int answer;
  unsigned int *wp;
  Reg[0] = 0;

  loadBinaryFile();

  loadMemory();

  printf("Would you like to start in (1) 'single step' mode or in (2) 'run' mode?\n");
  scanf(" %d", &answer);
  getchar();
  while (answer != 1 && answer != 2) {
    printf("Please enter either 1 or 2\n");
    scanf("%d", &answer);
    getchar();
  }

  mode = (answer == 1) ? SINGLE_STEP : RUN;
  startSimulation(mode);

  //loopMem(mode);
  //printMemDescriptions(&memOffset);

  printf("\n\nSimulation complete.\n");
  printf("Number of instructions simulated: %d\n", instructionCount);
  printf("Number of memory references: %d\n", memRefCount);
  printf("Total clock cycles: %d\n", totalClockCycles);
  printf("\n");
  exit(0);
}

void loadMemory() {

  int idx;
  for (idx = 0; idx < entryPoint; idx += 4) {
    mem[idx / 4];
  }

}

void startSimulation(int mode) {
  int byteOffset;
  int memIndex;
  unsigned int rawInstruction;
  instruction instr;
  pc = userMemoryBase;

  /* now dump out the instructions loaded */

  while (!exitTriggered) {
    printf("pc: 0x%04X\n", pc);
    unsigned int rawInstruction = fetchInstruction();
    instruction instr;
    instr.address = pc;
    decodeInstruction(rawInstruction, &instr);
    instr.numClockCycles = 2;

    printInstruction(&instr);
    executeInstruction(&instr);
    printRegisters();


    if (mode == SINGLE_STEP) {
      getchar();
    }

    printf("\n");
  }

  printRegisters();
}

unsigned int fetchInstruction() {
  unsigned int instr = mem[pc / 4];
  printf("instr: %d\n", instr);
  pc = pc + 4;
  return instr;

}

void decodeInstruction(unsigned int rawInstruction, instruction *instr) {
  instr->raw = rawInstruction;
  instr->type = getType(&rawInstruction);
  instr->opcode = getOpcode(&rawInstruction);
  if (instr->type == R_TYPE) {
    instr->rs = getRS(&rawInstruction);
    instr->rd = getRD(&rawInstruction);
    instr->rt = getRT(&rawInstruction);
    instr->shamt = getShamt(&rawInstruction);
    instr->funct = getFunct(&rawInstruction);
  } else if (instr->type == I_TYPE) {
    instr->rs = getRS(&rawInstruction);
    instr->rt = getRT(&rawInstruction);
    instr->imm = getImm(&rawInstruction);
  } else if (instr->type == J_TYPE) {
    instr->index = getIndex(&rawInstruction);
  }
  if (rawInstruction == 0) {
    strcpy(instr->mneumonic, "NOP");
  } else {
    getMneumonic(instr->opcode, instr->funct, (*instr).mneumonic);
  }
  instr->isSyscall = (strcmp(instr->mneumonic, "SYSCL") == 0) ? 1: 0;
}

void executeInstruction(instruction *instr) {
  printf("Execute `%s` type instruction\n", instr->mneumonic);
  instructionCount++;

//  if (instr->isSyscall)
//    systemCall(instr);
//
//  else if (strcmp(instr->mneumonic, "add") == 0)
//    add(instr);
//  else if (strcmp(instr->mneumonic, "addi") == 0)
//    addi(instr);
//  else if (strcmp(instr->mneumonic, "lw") == 0)
//    lw(instr);
//  else if (strcmp(instr->mneumonic, "jal") == 0)
//    jal(instr);
//  else if (strcmp(instr->mneumonic, "and") == 0)
//    and(instr);
//  else if (strcmp(instr->mneumonic, "ori") == 0)
//    ori(instr);
//  else if (strcmp(instr->mneumonic, "beq") == 0)
//    beq(instr);
//  else if (strcmp(instr->mneumonic, "bne") == 0)
//    bne(instr);
//  else if (strcmp(instr->mneumonic, "sll") == 0)
//    sll(instr);
//  else if (strcmp(instr->mneumonic, "jr") == 0)
//    jr(instr);
//  else if (strcmp(instr->mneumonic, "or") == 0)
//    or(instr);






  if (instr->isSyscall)
    systemCall(instr);
  else if (strcmp(instr->mneumonic, "add") == 0)
    add(instr);
  else if (strcmp(instr->mneumonic, "addi") == 0)
    addi(instr);
  else if (strcmp(instr->mneumonic, "lw") == 0)
    lw(instr);
  else if (strcmp(instr->mneumonic, "jal") == 0)
    jal(instr);
  else if (strcmp(instr->mneumonic, "and") == 0)
    and(instr);
  else if (strcmp(instr->mneumonic, "ori") == 0)
    ori(instr);
  else if (strcmp(instr->mneumonic, "beq") == 0)
    beq(instr);
  else if (strcmp(instr->mneumonic, "bne") == 0)
    bne(instr);
  else if (strcmp(instr->mneumonic, "sll") == 0)
    sll(instr);
  else if (strcmp(instr->mneumonic, "jr") == 0)
    jr(instr);
  else if (strcmp(instr->mneumonic, "or") == 0)
    or(instr);
  else if (strcmp(instr->mneumonic, "add") == 0)
    add(instr);
  else if (strcmp(instr->mneumonic, "addu") == 0)
    addu(instr);
  else if (strcmp(instr->mneumonic, "sub") == 0)
    sub(instr);
  else if (strcmp(instr->mneumonic, "subu") == 0)
    subu(instr);
  else if (strcmp(instr->mneumonic, "nor") == 0)
    nor(instr);
  else if (strcmp(instr->mneumonic, "xor") == 0)
    xor(instr);
  else if (strcmp(instr->mneumonic, "srl") == 0)
    srl(instr);
  else if (strcmp(instr->mneumonic, "sra") == 0)
    sra(instr);
  else if (strcmp(instr->mneumonic, "sllv") == 0)
    sllv(instr);
  else if (strcmp(instr->mneumonic, "srlv") == 0)
    srlv(instr);
  else if (strcmp(instr->mneumonic, "or") == 0)
    srav(instr);
  else if (strcmp(instr->mneumonic, "slt") == 0)
    slt(instr);
  else if (strcmp(instr->mneumonic, "sltu") == 0)
    sltu(instr);
  else if (strcmp(instr->mneumonic, "jalr") == 0)
    jalr(instr);
  else if (strcmp(instr->mneumonic, "addiu") == 0)
    addiu(instr);
  else if (strcmp(instr->mneumonic, "andi") == 0)
    andi(instr);
  else if (strcmp(instr->mneumonic, "xori") == 0)
    xori(instr);
  else if (strcmp(instr->mneumonic, "slti") == 0)
    slti(instr);
  else if (strcmp(instr->mneumonic, "sltiu") == 0)
    sltiu(instr);
  else if (strcmp(instr->mneumonic, "j") == 0)
    j(instr);
  else if (strcmp(instr->mneumonic, "lb") == 0)
    lb(instr);
  else if (strcmp(instr->mneumonic, "lbu") == 0)
    lbu(instr);
  else if (strcmp(instr->mneumonic, "lh") == 0)
    lh(instr);
  else if (strcmp(instr->mneumonic, "lhu") == 0)
    lhu(instr);
  else if (strcmp(instr->mneumonic, "sb") == 0)
    sb(instr);
  else if (strcmp(instr->mneumonic, "sh") == 0)
    sh(instr);
  else if (strcmp(instr->mneumonic, "sw") == 0)
    sw(instr);










}

void loopMem() {
  int byteOffset;
  int memIndex;
  unsigned int rawInstruction;
  instruction instr;

  /* now dump out the instructions loaded */
  for (byteOffset = 0; byteOffset < memOffset; byteOffset += 4) {
    memIndex = byteOffset / 4;
    readInstruction(memIndex, &instr);
    printInstruction(&instr);
    //printf("%d | %d\n\n", instr.type, instr.opcode);

    handleInstruction(memIndex);
  }
}

void readInstruction(int index, instruction *instr) {
  unsigned int rawInstruction = mem[index];
  instr->address = index * 4;
  instr->raw = rawInstruction;
  instr->type = getType(&rawInstruction);
  instr->opcode = getOpcode(&rawInstruction);
  if (instr->type == R_TYPE) {
    instr->rs = getRS(&rawInstruction);
    instr->rd = getRD(&rawInstruction);
    instr->rt = getRT(&rawInstruction);
    instr->shamt = getShamt(&rawInstruction);
    instr->funct = getFunct(&rawInstruction);
  } else if (instr->type == I_TYPE) {
    instr->rs = getRS(&rawInstruction);
    instr->rt = getRT(&rawInstruction);
    instr->imm = getImm(&rawInstruction);
  } else if (instr->type == J_TYPE) {
    instr->index = getIndex(&rawInstruction);
  }
  printf("raw: %d\n", rawInstruction);
  if (rawInstruction == 0) {
    printf("NOOOP\n");
    strcpy(instr->mneumonic, "NOP");
  } else {
    getMneumonic(instr->opcode, instr->funct, (*instr).mneumonic);
  }
}

void handleInstruction(int index) {
  unsigned int instr = mem[index];
  int memLocation = index * 4;
}

void printInstruction(instruction *instr) {
  printf("@%04X | ", instr->address);
  printf("0x%08X | ", instr->raw);
  printf("%5s | ", instr->mneumonic);

  if (instr->type == R_TYPE) {
    printf("R | ");
    printf("rs:0x%02X | ", instr->rs);
    printf("rt:0x%02X | ", instr->rt);
    printf("rd:0x%02X    | ", instr->rd);
    printf("shamt:0x%02X | ", instr->shamt);
    printf("funct:0x%02X | ", instr->funct);
  } else if (instr->type == I_TYPE) {
    printf("I | ");
    printf("rs:0x%02X | ", instr->rs);
    printf("rt:0x%02X | ", instr->rt);
    printf("imm:0x%04X | ", instr->imm);

  } else if (instr->type == J_TYPE) {
    printf("J | ");
    printf("index:0x%07X   | ", instr->index);
  } else {

  }

  printf("\n");
}

void printMemDescriptions() {
  unsigned int *wp;

  int byteOffset;

  /* now dump out the instructions loaded */
  for (byteOffset = 0; byteOffset < memOffset; byteOffset += 4) {
    /* byteOffset contains byte offset addresses */
    printf("@%08X : %08X\n", byteOffset, mem[byteOffset / 4]);
    wp = (unsigned int *) &mem[byteOffset / 4];

    int type = getType(wp);
    int funcCode = getFunct(wp);
    int opcode = getOpcode(wp);
    int rs = getRS(wp);
    int rd = getRD(wp);
    int rt = getRT(wp);
    double imm = getImm(wp);
    double eff = getEff(wp, byteOffset);
    int shamt = getShamt(wp);
    char mneumonic[5];
    char funcName[5];
    char IJName[6];
    getMneumonic(opcode, funcCode, mneumonic);

    //getMneumonic(funcCode, IJName);

    /*R-Type*/
    printf("\tAddress:\t [0x%X]\n", byteOffset);
    printf("\tMneumonic:\t [%s]\n", mneumonic);

    if (type == R_TYPE && strcmp(mneumonic, "other")) {
      printf("\t*SYSCALL*:\n");
    }
    if (type == R_TYPE) {
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

    else if (type == I_TYPE) {
      printf("\tName:\t\t [%s]\n", IJName);
      printf("\tType:\t\t [%c]\n", type);
      printf("\tRs:\t\t [%d]\n", rs);
      printf("\tRt:\t\t [%d]\n", rt);
      printf("\tImm:\t\t [%lf]\n", imm);
      if (isBranch(funcCode)) {
        printf("\t*BRANCH*");
        printf("\tEffAddr:\t [%lf]\n", eff);
        // ADD PC + R[Rs]
      }
    }

    else if (type == J_TYPE) {
      printf("\tName:\t\t [%s]\n", IJName);
      printf("\tType:\t\t [%c]\n", type);
      printf("\tEffAddr:\t [%lf]\n", eff);
    }

    else {
      printf("INVALID INSTRUCTION\n");
    }


    /*I-Type*/
    //printf("Address\t Type\t Name\t ", opcode, funcName, rs);

    /*J-Type*/
    //printf("Address\t Type\t Name\t ", opcode, funcName, rs);

   // printf("\tType: [%c] \tName: [%s] \tRs: [%d]\n\n", opcode, funcName, rs);
    printf("\n\n");
  }
}

int getType(unsigned int *wp) {
  unsigned int wv;
  wv = *wp >> 26;
  switch (wv) {
    case 0x00: return R_TYPE;
    case 0x02: return J_TYPE;
    case 0x03: return J_TYPE;
    case 0x08: return I_TYPE;
    case 0x09: return I_TYPE;
    case 0x0C: return I_TYPE;
    case 0x0D: return I_TYPE;
    case 0x0E: return I_TYPE;
    case 0x0A: return I_TYPE;
    case 0x0B: return I_TYPE;
    case 0x04: return I_TYPE;
    case 0x05: return I_TYPE;
    case 0x20: return I_TYPE;
    case 0x24: return I_TYPE;
    case 0x21: return I_TYPE;
    case 0x25: return I_TYPE;
    case 0x0F: return I_TYPE;
    case 0x23: return I_TYPE;
    case 0x28: return I_TYPE;
    case 0x29: return I_TYPE;
    case 0x2B: return I_TYPE;
    default: return INVALID_TYPE;
  }
}

int getOpcode(unsigned int *wp) {
  return *wp >> 26;
}

int getFunct(unsigned int *wp) {
  return *wp & 0b00000000000000000000000000111111;
}

void getMneumonic(int opcode, int funct, char *mneumonic) {
  if (opcode != 0) {
    switch (opcode) {
      case 0x08: strcpy(mneumonic, "addi"); break;
      case 0x09: strcpy(mneumonic, "addiu"); break;
      case 0x0C: strcpy(mneumonic, "andi"); break;
      case 0x0D: strcpy(mneumonic, "ori"); break;
      case 0x0E: strcpy(mneumonic, "xori"); break;
      case 0x0A: strcpy(mneumonic, "slti"); break;
      case 0x0B: strcpy(mneumonic, "sltiu"); break;
      case 0x04: strcpy(mneumonic, "beq"); break;
      case 0x05: strcpy(mneumonic, "bne"); break;
      case 0x02: strcpy(mneumonic, "j"); break;
      case 0x03: strcpy(mneumonic, "jal"); break;
      case 0x20: strcpy(mneumonic, "lb"); break;
      case 0x24: strcpy(mneumonic, "lbu"); break;
      case 0x21: strcpy(mneumonic, "lh"); break;
      case 0x25: strcpy(mneumonic, "lhu"); break;
      case 0x0F: strcpy(mneumonic, "lui"); break;
      case 0x23: strcpy(mneumonic, "lw"); break;
      case 0x28: strcpy(mneumonic, "sb"); break;
      case 0x29: strcpy(mneumonic, "sh"); break;
      case 0x2B: strcpy(mneumonic, "sw"); break;
      default: strcpy(mneumonic, "other"); break;
    }
  } else  {
    switch (funct) {
      case 0x20: strcpy(mneumonic, "add"); break;
      case 0x21: strcpy(mneumonic, "addu"); break;
      case 0x22: strcpy(mneumonic, "sub"); break;
      case 0x23: strcpy(mneumonic, "subu"); break;
      case 0x24: strcpy(mneumonic, "and"); break;
      case 0x27: strcpy(mneumonic, "nor"); break;
      case 0x25: strcpy(mneumonic, "or"); break;
      case 0x26: strcpy(mneumonic, "xor"); break;
      case 0x00: strcpy(mneumonic, "sll"); break;
      case 0x02: strcpy(mneumonic, "srl"); break;
      case 0x03: strcpy(mneumonic, "sra"); break;
      case 0x04: strcpy(mneumonic, "sllv"); break;
      case 0x06: strcpy(mneumonic, "srlv"); break;
      case 0x07: strcpy(mneumonic, "srav"); break;
      case 0x2A: strcpy(mneumonic, "slt"); break;
      case 0x2b: strcpy(mneumonic, "sltu"); break;
      case 0x08: strcpy(mneumonic, "jr"); break;
      case 0x09: strcpy(mneumonic, "jalr"); break;
      case 0x0C: strcpy(mneumonic, "SYSCL"); break;
      default: strcpy(mneumonic, "other"); break;
    }
  }
}

void getIJName(int wv, char *funcName) {
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

double getIndex(unsigned int *wp) {
  return *wp & 0b00000011111111111111111111111111;
}

int isBranch(int funcCode) {
  switch (funcCode) {
    case 0x04: return 1;
    case 0x05: return 1;
    default: return 0;
  }
}

unsigned int getTReg(int regNum) {
  return Reg[regNum + 8];
}
unsigned int getAReg(int regNum) {
  return Reg[regNum + 4];
}
unsigned int getSReg(int regNum) {
  return Reg[regNum + 16];
}
unsigned int getVReg(int regNum) {
  return Reg[regNum + 2];
}
unsigned int getReg(int regNum) {
  return Reg[regNum];
}

void loadBinaryFile() {
  FILE *fd;
  /* This is the filename to be loaded */
  //char filename[] = "testcase1.mb";
  char filename[] = FILENAME;
  int byteOffset;
  int n;

  fd = fopen(filename, "rb");

  if (fd == NULL) {
    printf("\nCouldn't load test case - quitting.\n");
    exit(99);
  }

  /* This is the memory pointer, a byte offset */
  memOffset = 0;

  /* read the header and verify it. */
  fread((void *) &mb_hdr, sizeof(mb_hdr), 1, fd);

  if (! strcmp(mb_hdr.signature, "~MB")==0) {
    printf("\nThis isn't really a mips_asm binary file - quitting.\n");
    exit(98);
  }

  //printf("mb_hdr.entry: %d \n", mb_hdr.entry);
  printf("\n%s Loaded ok, program size=%d bytes.\n\n", filename, mb_hdr.size);

  /* read the binary code a word at a time */

  entryPoint = mb_hdr.entry;
  printf("EntryPoint: %d\n", entryPoint);

  while ((memOffset / 4) < entryPoint / 4) {
    printf("load memory item\n");
    n = fread((void *) &mem[(memOffset)/4], 4, 1, fd); /* note div/4 to make word index */
    (memOffset) += 4;
  }

  memOffset = userMemoryBase;

  do {
    n = fread((void *) &mem[(memOffset)/4], 4, 1, fd); /* note div/4 to make word index */

    if (n) {
      /* Increment byte pointer by size of instr */
      (memOffset) += 4;
    } else {
      break;
    }

  } while ((memOffset) < sizeof(mem));

  fclose(fd);

}

/*******************************************************/
/*******************************************************/

void printRegisters() {
  int idx;
  for (idx = 0; idx < 32; idx++) {
    printf("R%02d: %05d | ", idx, Reg[idx]);
    if ((idx + 1) % 7 == 0) {
      printf("\n");
    }
  }
  printf("\n");
}

/*******************************************************/
/*******************************************************/

void lw(instruction *instr) {
  int rt = instr->rt;
  int rs = instr->rs;
  short imm = instr->imm * 4;
  //rt = M[rs + s.imm]
  int oldRt = Reg[rt];
  printf("rt: %d\n", rt);
  printf("Reg[rt]: %d\n", Reg[rt]);
  Reg[rt] = mem[Reg[rs] + imm];
  memRefCount++;
  printf("Executed LW; rt: %x -> %x\n", oldRt, Reg[rt]);
  instr->numClockCycles = 5;
  totalClockCycles += 5;
}

void jal(instruction *instr) {
  int index = userMemoryBase - 4 + instr->index * 4;
  Reg[31] = pc;
  pc = index;
  printf("Executed JAL\n");
  instr->numClockCycles = 3;
  totalClockCycles += 3;
}

void and(instruction *instr) {
  int rd = instr->rd;
  int rt = instr->rt;
  int rs = instr->rs;
  instr->numClockCycles = 4;
  totalClockCycles += 4;
  int oldRd = Reg[rd];
  Reg[rd] = Reg[rs] & Reg[rt];
  printf("Executed AND; rd: %d -> %d \n", oldRd, Reg[rd]);
}

void ori(instruction *instr) {
  int rt = instr->rt;
  int rs = instr->rs;
  short imm = instr->imm;
  int oldRt = Reg[rt];
  Reg[rt] = Reg[rs] | imm;
  printf("Executed ORI; rt: %d -> %d \n", oldRt, Reg[rt]);
  instr->numClockCycles = 4;
  totalClockCycles += 4;
}

void beq(instruction *instr) {
  int rs = instr->rs;
  int rt = instr->rt;
  short imm = (instr->imm * 4);
  int oldPC = pc;
  instr->numClockCycles = 3;
  totalClockCycles += 3;
  if (Reg[rs] == Reg[rt]) {
    pc += imm;
    printf("Executed beq[BRANCHING]; pc: %d -> %d \n", oldPC, pc);
  } else {
    printf("Executed beq[no-br]; pc: %d -> %d \n", oldPC, pc);
  }
}

void systemCall(instruction *instr) {
  int v0 = getVReg(0);
  switch (v0) {
    case 1: printf("SYSCALL: print_int\n"); break;
    case 2: printf("SYSCALL: print_float\n"); break;
    case 3: printf("SYSCALL: print_double\n"); break;
    case 4: printf("SYSCALL: print_string\n"); break;
    case 5: printf("SYSCALL: read_int\n"); break;
    case 6: printf("SYSCALL: read_float\n"); break;
    case 7: printf("SYSCALL: read_double\n"); break;
    case 8: printf("SYSCALL: read_string\n"); break;
    case 9: printf("SYSCALL: sbrk\n"); break;
    case 10:
            printf("Syscall to exit\n");
            exitTriggered = 1;
            break;
    default: printf("Unknown syscall type\n"); break;
  }
}

// TO-DO NUMBER OF CLOCK CYCLES????
void addi(instruction *instr) {
  int rt = instr->rt;
  int rs = instr->rs;
  short imm = instr->imm;
  int oldRt = Reg[rt];
  Reg[rt] = Reg[rs] + imm;
  printf("Executed ADDI; rt: %d -> %d \n", oldRt, Reg[rt]);
  instr->numClockCycles = 4;
  totalClockCycles += 4;
}

//TO-DO NUMBER OF CLOCK CYCLES????
void sll(instruction *instr) {
  int rd = instr->rd;
  int rt = instr->rd;
  int shamt = instr->shamt;

  int oldRd = Reg[rd];
  Reg[rd] = Reg[rt] << shamt;
  printf("Executed SLL; rd: %d -> %d \n", oldRd, Reg[rd]);
  instr->numClockCycles = 4;
  totalClockCycles += 4;
}

void bne(instruction *instr) {
  int rs = instr->rs;
  int rt = instr->rt;
  short imm = instr->imm * 4;
  int oldPC = pc;
  instr->numClockCycles = 3;
  totalClockCycles += 3;
  if (Reg[rs] != Reg[rt]) {
    pc += imm;
    printf("Executed bne [BRANCHING]; pc: %d -> %d \n", oldPC, pc);
  } else {
    printf("Executed bne [no-br]; pc: %d -> %d \n", oldPC, pc);
  }
}

void jr(instruction *instr) {
  int rs = instr->rs;
  int oldPC = pc;
  pc = Reg[rs];
  instr->numClockCycles = 3;
  totalClockCycles += 3;
  printf("Executed jr; pc: %d -> %d \n", oldPC, pc);
}

void or(instruction *instr) {
  int rd = instr->rd;
  int rs = instr->rs;
  int rt = instr->rt;
  int oldRd = Reg[rd];
  instr->numClockCycles = 4;
  totalClockCycles += 4;
  Reg[rd] = Reg[rs] | Reg[rt];
  printf("Executed or; rd: %d -> %d \n", oldRd, Reg[rd]);
}

void add(instruction *instr) {
  int rd = instr->rd;
  int rt = instr->rt;
  int rs = instr->rs;

  instr->numClockCycles = 4;
  totalClockCycles += 4;
  //check for overflow???
  int oldRd = Reg[rd];
  Reg[rd] = Reg[rs] + Reg[rt];
  printf("Executed ADD; rd: %d -> %d \n", oldRd, Reg[rd]);
}

void addu(instruction *instr) {
  int rd = instr->rd;
  int rt = instr->rt;
  int rs = instr->rs;

  instr->numClockCycles = 4;
  totalClockCycles += 4;
  //no overflow
  int oldRd = Reg[rd];
  Reg[rd] = Reg[rs] + Reg[rt];
  printf("Executed ADDU; rd: %d -> %d \n", oldRd, Reg[rd]);
}

/* subtract */
void sub(instruction *instr) {
  int rd = instr->rd;
  int rt = instr->rt;
  int rs = instr->rs;

  instr->numClockCycles = 4;
  totalClockCycles += 4;
  int oldRd = Reg[rd];
  Reg[rd] = Reg[rs] - Reg[rt];
  printf("Executed SUB; rd: %d -> %d \n", oldRd, Reg[rd]);
}

/* subtract unsigned */
void subu(instruction *instr) {
  int rd = instr->rd;
  int rt = instr->rt;
  int rs = instr->rs;

  instr->numClockCycles = 4;
  totalClockCycles += 4;
  int oldRd = Reg[rd];
  Reg[rd] = Reg[rs] - Reg[rt];
  printf("Executed SUBU; rd: %d -> %d \n", oldRd, Reg[rd]);
}

void nor(instruction *instr) {
  int rd = instr->rd;
  int rt = instr->rt;
  int rs = instr->rs;

  instr->numClockCycles = 4;
  totalClockCycles += 4;
  int oldRd = Reg[rd];
  Reg[rd] = ~(Reg[rs] | Reg[rt]);
  printf("Executed NOR; rd: %d -> %d \n", oldRd, Reg[rd]);
}

void xor(instruction *instr) {
  int rd = instr->rd;
  int rt = instr->rt;
  int rs = instr->rs;
  instr->numClockCycles = 4;
  totalClockCycles += 4;

  int oldRd = Reg[rd];
  Reg[rd] = (Reg[rs] ^ Reg[rt]);
  printf("Executed XOR; rd: %d -> %d \n", oldRd, Reg[rd]);
}

//TO-DO NUMBER OF CLOCK CYCLES?????
void srl(instruction *instr) {
  int rd = instr->rd;
  int rt = instr->rt;
  int shamt = instr->shamt;
  instr->numClockCycles = 4;
  totalClockCycles += 4;

  int oldRd = Reg[rd];
  Reg[rd] = Reg[rt] >> shamt;
  Reg[rd] &= ~(1 << 31);
  printf("Executed SRL; rd: %d -> %d \n", oldRd, Reg[rd]);
}

void sra(instruction *instr) {
  int rd = instr->rd;
  int rt = instr->rt;
  int shamt = instr->shamt;
  instr->numClockCycles = 4;
  totalClockCycles += 4;

  int oldRd = Reg[rd];
  Reg[rd] = Reg[rt] >> shamt;
  printf("Executed SRA; rd: %d -> %d \n", oldRd, Reg[rd]);
}

void sllv(instruction *instr) {
  int rd = instr->rd;
  int rt = instr->rt;
  int rs = instr->rs;
  instr->numClockCycles = 4;
  totalClockCycles += 4;

  int oldRd = Reg[rd];
  Reg[rd] = Reg[rt] << Reg[rs];
  printf("Executed SLLV; rd: %d -> %d \n", oldRd, Reg[rd]);
}

void srlv(instruction *instr) {
  int rd = instr->rd;
  int rt = instr->rt;
  int rs = instr->rs;
  instr->numClockCycles = 4;
  totalClockCycles += 4;

  int oldRd = Reg[rd];
  Reg[rd] = (Reg[rt] >> Reg[rs]);
  printf("Executed SRLV; rd: %d -> %d \n", oldRd, Reg[rd]);
}

void srav(instruction *instr) {
  int rd = instr->rd;
  int rt = instr->rt;
  int rs = instr->rs;
  instr->numClockCycles = 4;
  totalClockCycles += 4;

  int oldRd = Reg[rd];
  Reg[rd] = (Reg[rt] >> Reg[rs]);
  printf("Executed SRAV; rd: %d -> %d \n", oldRd, Reg[rd]);
}

void slt(instruction *instr) {
  int rd = instr->rd;
  int rt = instr->rt;
  int rs = instr->rs;
  instr->numClockCycles = 4;
  totalClockCycles += 4;
  int oldRd = Reg[rd];
  Reg[rd] = (Reg[rs] < Reg[rt]);
  printf("Executed SLT; rd: %d -> %d \n", oldRd, Reg[rd]);
}

void sltu(instruction *instr) {
  int rd = instr->rd;
  int rt = instr->rt;
  int rs = instr->rs;
  instr->numClockCycles = 4;
  totalClockCycles += 4;
  int oldRd = Reg[rd];
  Reg[rd] = (Reg[rs] < Reg[rt]);
  printf("Executed SLTU; rd: %d -> %d \n", oldRd, Reg[rd]);
}

void jalr(instruction *instr) {
  instr-> numClockCycles = 3;
  totalClockCycles += 3;
  int rt = instr->rt;
  int rs = instr->rs;
  int rd = instr->rd;
  int oldPc = pc;
  int oldRa = Reg[31];

  pc = Reg[rs];
  Reg[31] = pc + 4;
  printf("Executed jalr; pc: %d -> %d ra: %d -> %d\n", oldPc, Reg[pc], Reg[31], oldRa);
}

void addiu(instruction *instr) {
  int rt = instr->rt;
  int rs = instr->rs;
  short imm = instr->imm;
  int oldRt = Reg[rt];
  Reg[rt] = (unsigned int)Reg[rs] + imm;
  printf("Executed ADDIU; rt: %d -> %d \n", oldRt, Reg[rt]);
  instr->numClockCycles = 4;
  totalClockCycles += 4;
}

void andi(instruction *instr) {
  int rt = instr->rt;
  int rs = instr->rs;
  short imm = instr->imm;
  int oldRt = Reg[rt];
  Reg[rt] = Reg[rs] & imm;
  printf("Executed ANDI; rt: %d -> %d \n", oldRt, Reg[rt]);
  instr->numClockCycles = 4;
  totalClockCycles += 4;
}

void xori(instruction *instr) {
  int rt = instr->rt;
  int rs = instr->rs;
  short imm = instr->imm;
  int oldRt = Reg[rt];
  Reg[rt] = Reg[rs] ^ imm;
  printf("Executed XORI; rt: %d -> %d \n", oldRt, Reg[rt]);
  instr->numClockCycles = 4;
  totalClockCycles += 4;
}

void slti(instruction *instr) {
  int rt = instr->rt;
  int rs = instr->rs;
  short imm = instr->imm;
  int oldRt = Reg[rt];
  if (rs < imm) {
    Reg[rt] = 1;
  } 
  else {
    Reg[rt] = 0;
  }
  printf("Executed SLTI; rt: %d -> %d \n", oldRt, Reg[rt]);
  instr->numClockCycles = 4;
  totalClockCycles += 4;
}

void sltiu(instruction *instr) {
  int rt = instr->rt;
  int rs = instr->rs;
  short imm = instr->imm;
  int oldRt = Reg[rt];
  if ((unsigned int)rs < imm) {
    Reg[rt] = 1;
  } 
  else {
    Reg[rt] = 0;
  }
  printf("Executed SLTIU; rt: %d -> %d \n", oldRt, Reg[rt]);
  instr->numClockCycles = 4;
  totalClockCycles += 4;
}

void j(instruction *instr) {
  int rd = instr->rd;
  int imm = instr->imm;
  instr->numClockCycles = 3;
  totalClockCycles += 3;
  int oldRd = Reg[rd];
  Reg[rd] = Reg[imm];
  printf("Executed J; rd: %d -> %d \n", oldRd, Reg[rd]);

  int oldPc = pc;
  pc = instr->index;
  printf("Executed j; pc: %d -> %d \n", oldPc, pc);
}

void lb(instruction *instr) {
  instr->numClockCycles = 5;
  totalClockCycles += 5;
  int rs = instr->rs;
  int rt = instr->rt;
  short imm = instr->imm;
  memRefCount++;

  Reg[rt] = (char)(mem[Reg[rs]] + imm);
  //printf("Executed lb; rt: %d -> %d \n", oldRt, Reg[rt]);
}

void lbu(instruction *instr) {
  instr->numClockCycles = 5;
  totalClockCycles += 5;
  int rs = instr->rs;
  int rt = instr->rt;
  short imm = instr->imm;
  memRefCount++;

  Reg[rt] = (unsigned char)(mem[Reg[rs]] + imm);
}

void lh(instruction *instr) {
  instr->numClockCycles = 5;
  totalClockCycles += 5;
  int rs = instr->rs;
  int rt = instr->rt;
  short imm = instr->imm;
  memRefCount++;

  Reg[rt] = (short)(mem[Reg[rs]] + imm);
}

/* R[rt]={16’b0, M[R[rs]+ZeroExtImm]( */
void lhu(instruction *instr) {
  instr->numClockCycles = 5;
  totalClockCycles += 5;
  int rs = instr->rs;
  int rt = instr->rt;
  short imm = instr->imm;

  memRefCount++;
  Reg[rt] = (unsigned short)(mem[Reg[rs]] + imm);
}

void sb(instruction *instr) {
  instr->numClockCycles = 4;
  totalClockCycles += 4;
  int rs = instr->rs;
  int rt = instr->rt;
  short imm = instr->imm;

  memRefCount++;
  mem[Reg[rs] + imm] = (char)Reg[rt];
}

void sh(instruction *instr) {
  instr->numClockCycles = 4;
  totalClockCycles += 4;
  int rs = instr->rs;
  int rt = instr->rt;
  short imm = instr->imm;

  memRefCount++;
  mem[Reg[rs] + imm] = (short)Reg[rt];
  printf("Executed LW;");
}

/* UNTESTED!!! */
/* M[R[rs]+SignExtImm]=R[rt]  */
void sw(instruction *instr) {
  instr->numClockCycles = 4;
  totalClockCycles += 4;
  int rs = instr->rs;
  int rt = instr->rt;
  short imm = instr->imm;

  memRefCount++;
  mem[Reg[rs] + imm] = Reg[rt];
  printf("Executed LW;");
}
