
typedef struct instruction {
  unsigned int address;
  unsigned int raw;
  int opcode;
  int isSyscall;
  int type;
  int rs;
  int rt;
  int rd;
  int shamt;
  int funct;
  int imm;
  int index;
  char mneumonic[5];
  int numClockCycles;
} instruction;


/*
void loopMem(void (*handler)(unsigned int));
void loopMem(void (*handler)(int));
*/
void loopMem();
//void handleInstruction(unsigned int instr);
void handleInstruction(int index);
void executeInstruction(instruction *instr);

void readInstruction(int index, instruction *instr);
void printInstruction(instruction *instr);
unsigned int fetchInstruction();
void decodeInstruction(unsigned int rawInstruction, instruction *instr);
void startSimulation(int mode);
void loadMemory();

void loadBinaryFile();
void printMemDescriptions();
int getType(unsigned int *wp);
int getOpcode(unsigned int *wp);
int getFunct(unsigned int *wp);
void getMneumonic(int opcode, int funct, char mneumonic[5]);
void getIJName(int wv, char *funcName);
int getRS(unsigned int *wp);
int getRT(unsigned int *wp);
int getRD(unsigned int *wp);
int getShamt(unsigned int *wp);
int isShift(int funcCode);
double getEff(unsigned int *wp, int branchOffset);
double getImm(unsigned int *wp);
double getIndex(unsigned int *wp);
int isBranch(int funcCode);

unsigned int getTReg(int regNum);
unsigned int getAReg(int regNum);
unsigned int getSReg(int regNum);
unsigned int getVReg(int regNum);
unsigned int getReg(int regNum);

void lw(instruction *instr);
void jal(instruction *instr);
void and(instruction *instr);
void ori(instruction *instr);
void systemCall(instruction *instr);
void addi(instruction *instr);
void beq(instruction *instr);
void bne(instruction *instr);
void sll(instruction *instr);
void jr(instruction *instr);
void or(instruction *instr);
void add(instruction *instr);
void addu(instruction *instr);
void sub(instruction *instr);
void subu(instruction *instr);
void nor(instruction *instr);
void xor(instruction *instr);
void srl(instruction *instr);
void sra(instruction *instr);
void sllv(instruction *instr);
void srlv(instruction *instr);
void srav(instruction *instr);
void slt(instruction *instr);
void sltu(instruction *instr);
void jalr(instruction *instr);
void addiu(instruction *instr);
void andi(instruction *instr);
void xori(instruction *instr);
void slti(instruction *instr);
void sltiu(instruction *instr);
void j(instruction *instr);
void lb(instruction *instr);
void lbu(instruction *instr);
void lh(instruction *instr);
void lhu(instruction *instr);
void sb(instruction *instr);
void sh(instruction *instr);
void sw(instruction *instr);
