#ifndef PATH_COUNTER_H_
#define PATH_COUNTER_H_
#include <systemc>
#include <cmath>
#include <iomanip>
using namespace sc_core;
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <algorithm>
#include "counter_def.h"

struct PathCounter : public sc_module {
  tlm_utils::simple_target_socket<PathCounter> tsock;

  sc_fifo<unsigned char> i_0;
  sc_fifo<unsigned int> o_result;
  sc_fifo<unsigned int> o_compute_cycle;  // FIFO channel for compute cycle

  SC_HAS_PROCESS(PathCounter);

  PathCounter(sc_module_name n): 
    sc_module(n), 
    tsock("t_skt"), 
    base_offset(0) 
  {
    tsock.register_b_transport(this, &PathCounter::blocking_transport);
    SC_THREAD(do_counter);
  }

  ~PathCounter() {
	}

  unsigned int base_offset;
  bool cur = 0;
  sc_time start_compute, end_compute;  // SystemC events
  void do_counter(){
    // cout << "Start PathCounter::do_counter" << endl;
    { wait(CLOCK_PERIOD, SC_NS); }

    while (true) {
      start_compute = sc_time_stamp();  // Store the current time
      sc_dt::sc_uint<32> dp[grid_size] = {0};
      dp[0]=1;
      for (int i = 0; i < grid_size; i ++) {
        for (int j = 0; j < grid_size ; j ++) {
          cur = i_0.read();
          if (cur == 1) {
            dp[j] = 0;
          } else if (j > 0) {
            dp[j] += dp[j-1];
          }
          
        }
      }

      end_compute = sc_time_stamp();  // Mark the end of computation
      o_result.write(dp[grid_size-1]);

      // Calculate and write the computation time
      sc_time compute_time = end_compute - start_compute;
      // Convert to nanoseconds and cast to integer
      unsigned int compute_cycle = static_cast<unsigned int>(compute_time.to_seconds() * 1e8);
      o_compute_cycle.write(compute_cycle);
    }
  }


  void blocking_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay){
    wait(delay);
    // unsigned char *mask_ptr = payload.get_byte_enable_ptr();
    // auto len = payload.get_data_length();
    tlm::tlm_command cmd = payload.get_command();
    sc_dt::uint64 addr = payload.get_address();
    unsigned char *data_ptr = payload.get_data_ptr();

    addr -= base_offset;

    word buffer;
    switch (cmd) {
      case tlm::TLM_READ_COMMAND:
        // cout << "READ" << endl;
        switch (addr) {
          case PATH_COUNTER_RESULT_ADDR:
            buffer.uint = o_result.read();
            break;
          case PATH_COUNTER_COMPUTE_TIME_ADDR:
            buffer.uint = o_compute_cycle.read();
            break;
          default:
            std::cerr << "READ Error! PathCounter::blocking_transport: address 0x"
                      << std::setfill('0') << std::setw(8) << std::hex << addr
                      << std::dec << " is not valid" << std::endl;
          }
        data_ptr[0] = buffer.uc[0];
        data_ptr[1] = buffer.uc[1];
        data_ptr[2] = buffer.uc[2];
        data_ptr[3] = buffer.uc[3];
        break;
      case tlm::TLM_WRITE_COMMAND:
        // cout << "WRITE" << endl;
        switch (addr) {
          case PATH_COUNTER_R_ADDR:
            i_0.write(data_ptr[0]);
            break;
          default:
            std::cerr << "WRITE Error! PathCounter::blocking_transport: address 0x"
                      << std::setfill('0') << std::setw(8) << std::hex << addr
                      << std::dec << " is not valid" << std::endl;
        }
        break;
      case tlm::TLM_IGNORE_COMMAND:
        payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
        return;
      default:
        payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
        return;
      }
      payload.set_response_status(tlm::TLM_OK_RESPONSE); // Always OK
  }
};
#endif
