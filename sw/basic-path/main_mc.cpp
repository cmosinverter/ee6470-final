#include "string"
#include "string.h"
#include "cassert"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

#define PROCESSORS 2
#define STIM_LEN 10
#define GRID_SIZE 8
#define DMA_BANDWIDTH 4
#define INITIAL_CYCLES 0

const bool STIM_DATA[STIM_LEN][GRID_SIZE*GRID_SIZE] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

extern "C" void *__dso_handle = 0;

union word {
  int sint;
  unsigned int uint;
  unsigned char uc[4];
};

// Path Counter ACC
static char* const PATHCOUNTER_START_ADDR[PROCESSORS] = {
    reinterpret_cast<char* const>(0x73000000),
    reinterpret_cast<char* const>(0x74000000)
};

static char* const PATHCOUNTER_READ_ADDR[PROCESSORS] = {
    reinterpret_cast<char* const>(0x73000004),
    reinterpret_cast<char* const>(0x74000004)
};

static char* const PATHCOUNTER_CYCLE_ADDR[PROCESSORS] = {
    reinterpret_cast<char* const>(0x73000008),
    reinterpret_cast<char* const>(0x74000008)
};

// DMA 
static volatile uint32_t * const DMA_SRC_ADDR  = (uint32_t * const)0x70000000;
static volatile uint32_t * const DMA_DST_ADDR  = (uint32_t * const)0x70000004;
static volatile uint32_t * const DMA_LEN_ADDR  = (uint32_t * const)0x70000008;
static volatile uint32_t * const DMA_OP_ADDR   = (uint32_t * const)0x7000000C;
static volatile uint32_t * const DMA_STAT_ADDR = (uint32_t * const)0x70000010;
static const uint32_t DMA_OP_MEMCPY = 1;

bool _is_using_dma = true;
unsigned int write_cycles = 0;
unsigned int read_cycles = 0;
unsigned int total_PE_latency = 0;
unsigned int total_PE_cycles = 0;
// global memory for the response
unsigned int response[STIM_LEN] = {0};

void write_data_to_ACC(char* ADDR, unsigned char* buffer, int len){
  if(_is_using_dma){  
    // Using DMA 
    *DMA_SRC_ADDR = (uint32_t)(buffer);
    *DMA_DST_ADDR = (uint32_t)(ADDR);
    *DMA_LEN_ADDR = len;
    *DMA_OP_ADDR  = DMA_OP_MEMCPY;
    write_cycles += INITIAL_CYCLES + len / DMA_BANDWIDTH;
  }else{
    // Directly Send
    memcpy(ADDR, buffer, sizeof(unsigned char)*len);
  }
}
void read_data_from_ACC(char* ADDR, unsigned char* buffer, int len){
  if(_is_using_dma){
    // Using DMA 
    *DMA_SRC_ADDR = (uint32_t)(ADDR);
    *DMA_DST_ADDR = (uint32_t)(buffer);
    *DMA_LEN_ADDR = len;
    *DMA_OP_ADDR  = DMA_OP_MEMCPY;
    read_cycles += INITIAL_CYCLES + len / DMA_BANDWIDTH;
  }else{
    // Directly Read
    memcpy(buffer, ADDR, sizeof(unsigned char)*len);
  }
}
int sem_init (uint32_t *__sem, uint32_t count) __THROW
{
  *__sem=count;
  return 0;
}

int sem_wait (uint32_t *__sem) __THROW
{
  uint32_t value, success; //RV32A
  __asm__ __volatile__("\
L%=:\n\t\
     lr.w %[value],(%[__sem])            # load reserved\n\t\
     beqz %[value],L%=                   # if zero, try again\n\t\
     addi %[value],%[value],-1           # value --\n\t\
     sc.w %[success],%[value],(%[__sem]) # store conditionally\n\t\
     bnez %[success], L%=                # if the store failed, try again\n\t\
"
    : [value] "=r"(value), [success]"=r"(success)
    : [__sem] "r"(__sem)
    : "memory");
  return 0;
}

int sem_post (uint32_t *__sem) __THROW
{
  uint32_t value, success; //RV32A
  __asm__ __volatile__("\
L%=:\n\t\
     lr.w %[value],(%[__sem])            # load reserved\n\t\
     addi %[value],%[value], 1           # value ++\n\t\
     sc.w %[success],%[value],(%[__sem]) # store conditionally\n\t\
     bnez %[success], L%=                # if the store failed, try again\n\t\
"
    : [value] "=r"(value), [success]"=r"(success)
    : [__sem] "r"(__sem)
    : "memory");
  return 0;
}

int barrier(uint32_t *__sem, uint32_t *__lock, uint32_t *counter, uint32_t thread_count) {
	sem_wait(__lock);
	if (*counter == thread_count - 1) { //all finished
		*counter = 0;
		sem_post(__lock);
		for (int j = 0; j < thread_count - 1; ++j) sem_post(__sem);
	} else {
		(*counter)++;
		sem_post(__lock);
		sem_wait(__sem);
	}
	return 0;
}

//synchronization objects
uint32_t barrier_counter=0; 
uint32_t barrier_lock; 
uint32_t barrier_sem; 
uint32_t lock;
uint32_t print_sem[PROCESSORS];



int main(unsigned hart_id) {
  
  if (hart_id == 1) {
		// create a barrier object with a count of PROCESSORS
		sem_init(&barrier_lock, 1);
		sem_init(&barrier_sem, 0); //lock all cores initially
		for(int i=0; i< PROCESSORS; ++i){
			sem_init(&print_sem[i], 0); //lock printing initially
		}
		// Create mutex lock
		sem_init(&lock, 1);
	}

  unsigned char  buffer[4] = {0};
  word data;

  int sets_per_processor = STIM_LEN / PROCESSORS;  // 5 in this case
  int start_set = hart_id * sets_per_processor;
  int end_set = start_set + sets_per_processor;

  for (int s = start_set; s < end_set; s++){
    sem_wait(&lock);  // Wait for the lock
    for (int i = 0; i < GRID_SIZE; i++){
      for (int j = 0; j < GRID_SIZE ; j++){
        buffer[0] = STIM_DATA[s][i*GRID_SIZE+j];
        buffer[1] = 0;
        buffer[2] = 0;
        buffer[3] = 0;
        write_data_to_ACC(PATHCOUNTER_START_ADDR[hart_id], buffer, 4);
      }
    }
    
    
    read_data_from_ACC(PATHCOUNTER_READ_ADDR[hart_id], buffer, 4);
    memcpy(data.uc, buffer, 4);
    printf("Core %d--[%d/%d] : %d\n", hart_id, s+1, STIM_LEN, data.uint);
    response[s] = data.uint;
    total_PE_cycles += 1;

    read_data_from_ACC(PATHCOUNTER_CYCLE_ADDR[hart_id], buffer, 4);
    memcpy(data.uc, buffer, 4);
    // printf("Core %d--[%d/%d] Compute Cycle: %d\n", hart_id, s+1, STIM_LEN, data.uint);
    total_PE_latency += data.uint;

    
    sem_post(&lock);  // Release the lock
    barrier(&barrier_sem, &barrier_lock, &barrier_counter, PROCESSORS);
  }
  

  if (hart_id == 0) {
    printf("Total DMA Write Cycles: %d\n", write_cycles);
    printf("Total DMA Read Cycles: %d\n", read_cycles);
    printf("Total DMA Cycles: %d\n", write_cycles + read_cycles);
    printf("Total PE Compuatation Cycles: %d\n", total_PE_cycles);
    printf("Total Application Cycles: %d\n", write_cycles + read_cycles + total_PE_cycles);
    printf("Total PE Latency: %d\n", total_PE_latency);
  }
  

  return 0;
}
