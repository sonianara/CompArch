

void printMemDescriptions(unsigned int *mem, unsigned int *wp, int memp);
char getType(unsigned int *wp);
int getOpcode(unsigned int *wp);
int getFuncCode(unsigned int *wp);
void getFuncName(int funcCode, char *funcName);
void getIJName(int wv, char *funcName);
int getRS(unsigned int *wp);
int getRT(unsigned int *wp);
int getRD(unsigned int *wp);
int getShamt(unsigned int *wp);
int isShift(int funcCode);
double getEff(unsigned int *wp, int branchOffset);
double getImm(unsigned int *wp);
int isBranch(int funcCode);




