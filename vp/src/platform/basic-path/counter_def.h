#ifndef COUNTER_DEF_H_
#define COUNTER_DEF_H_

#define CLOCK_PERIOD 10


// counter parameters
const int DMA_TRANS = 64;
const int grid_size = 8;

// Path Counter inner transport addresses
// Used between blocking_transport() & do_counter()
const int PATH_COUNTER_R_ADDR = 0x00000000;
const int PATH_COUNTER_RESULT_ADDR = 0x00000004;
const int PATH_COUNTER_COMPUTE_TIME_ADDR = 0x00000008;

const int PATH_COUNTER_RS_R_ADDR   = 0x00000000;
const int PATH_COUNTER_RS_W_WIDTH  = 0x00000004;
const int PATH_COUNTER_RS_W_HEIGHT = 0x00000008;
const int PATH_COUNTER_RS_W_DATA   = 0x0000000C;
const int PATH_COUNTER_RS_RESULT_ADDR = 0x00800000;


union word {
  int sint;
  unsigned int uint;
  unsigned char uc[4];
};


#endif
