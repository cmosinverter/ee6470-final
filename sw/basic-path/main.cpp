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

union word {
  int sint;
  unsigned int uint;
  unsigned char uc[4];
};

// Path Counter ACC
static char* const PATHCOUNTER_START_ADDR = reinterpret_cast<char* const>(0x73000000);
static char* const PATHCOUNTER_READ_ADDR  = reinterpret_cast<char* const>(0x73000004);


// DMA addresses
static volatile uint32_t * const DMA_SRC_ADDR  = (uint32_t * const)0x70000000;
static volatile uint32_t * const DMA_DST_ADDR  = (uint32_t * const)0x70000004;
static volatile uint32_t * const DMA_LEN_ADDR  = (uint32_t * const)0x70000008;
static volatile uint32_t * const DMA_OP_ADDR   = (uint32_t * const)0x7000000C;
static volatile uint32_t * const DMA_STAT_ADDR = (uint32_t * const)0x70000010;
static const uint32_t DMA_OP_MEMCPY = 1;

// DMA parameters
bool _is_using_dma = true;
int write_cycles = 0;
int read_cycles = 0;

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

void store_response(const unsigned int response[], const std::string& filePath) {
    std::ofstream outputFile(filePath);
    if (outputFile.is_open()) {
        // Write the array data to the file
        for(int i = 0; i < STIM_LEN; i++) {
            outputFile << response[i] << "\n";
        }
        outputFile.close();
        std::cout << "Response stored successfully in the file." << std::endl;
    } else {
        std::cout << "Failed to open the file for writing." << std::endl;
    }
}



int main(int argc, char *argv[]) {

  FILE *infp = fopen("stimulus.dat", "r");
  unsigned char  buffer[4] = {0};
  word data;

  for (int s = 0; s < STIM_LEN; s ++){
    for (int i = 0; i < GRID_SIZE; i ++){
      for (int j = 0; j < GRID_SIZE ; j ++){
        int m;
        fscanf(infp, "%d ", &m);
			  bool b = m != 0;  // Convert integer to boolean
        buffer[0] = b;
        buffer[1] = 0;
        buffer[2] = 0;
        buffer[3] = 0;
        write_data_to_ACC(PATHCOUNTER_START_ADDR, buffer, 4);
      }
    fscanf(infp, "\n"); // To move to the next line
    }
    read_data_from_ACC(PATHCOUNTER_READ_ADDR, buffer, 4);
    memcpy(data.uc, buffer, 4);
    printf("Core 0--[%d/%d] :%d\n", s+1, STIM_LEN, data.uint);
    response[s] = data.uint;
  }
  store_response(response, "response.dat");
  printf("Total Write Cycles: %d\n", write_cycles);
  printf("Total Read Cycles: %d\n", read_cycles);
  printf("Total DMA Cycles: %d\n", write_cycles + read_cycles);
  return 0;
}

