
typedef struct fetch_decode {
  int isReady;
  instruction instr;
} fetch_decode;

typedef struct decode_execute {
  int isReady;
  instruction instr;
} decode_execute;

typedef struct execute_memory {
  int isReady;
  instruction instr;
} execute_memory;

typedef struct memory_writeback {
  int isReady;
  instruction instr;
} memory_writeback;

typedef struct Bus {
  fetch_decode fd;
  decode_execute de;
  execute_memory em;
  memory_writeback mw;
} Bus;

