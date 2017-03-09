
#define BUSY 1
#define READY 2
#define TRUE 1
#define FALSE 0

typedef struct fetchInbox {
  int isEmpty;
} fetchInbox;

typedef struct fetchOutbox {
  int isEmpty;
} fetchOutbox;

typedef struct decodeInbox {
  int isEmpty;
} decodeInbox;

typedef struct decodeOutbox {
  int isEmpty;
} decodeOutbox;

typedef struct executeInbox {
  int isEmpty;
} executeInbox;

typedef struct executeOutbox {
  int isEmpty;
} executeOutbox;

typedef struct memoryInbox {
  int isEmpty;
} memoryInbox;

typedef struct memoryOutbox {
  int isEmpty;
} memoryOutbox;

typedef struct writebackInbox {
  int isEmpty;
} writebackInbox;

typedef struct writebackOutbox {
  int isEmpty;
} writebackOutbox;

typedef struct fetchBox {
  int count;
  fetchInbox in;
  fetchOutbox out;
} fetchBox;

typedef struct decodeBox {
  int count;
  decodeInbox in;
  decodeOutbox out;
} decodeBox;

typedef struct executeBox {
  int count;
  executeInbox in;
  executeOutbox out;
} executeBox;

typedef struct memoryBox {
  int count;
  memoryInbox in;
  memoryOutbox out;
} memoryBox;

typedef struct writebackBox {
  int count;
  writebackInbox in;
  writebackOutbox out;
} writebackBox;

typedef struct Bus {
  fetchBox fetch;
  decodeBox decode;
  executeBox execute;
  memoryBox memory;
  writebackBox writeback;
} Bus;



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
  short imm;
  unsigned int index;
  char mneumonic[5];
  int numClockCycles;
} instruction;

void fetch();
void decode();
void execute();
void memory();
void writeback();

void initInOutBoxes();

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
void startPipelinedSimulation(int mode);
void loadMemory();
unsigned int signExtend(unsigned int val);
int getRunMode();
void printRegisters();

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


//void srl(instruction *instr);
//void sra(instruction *instr);
//void sllv(instruction *instr);
//void srlv(instruction *instr);

