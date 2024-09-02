/***************************************************************************************
* Copyright (c) 2024 Beijing Institute of Open Source Chip (BOSC)
* Copyright (c) 2020-2024 Institute of Computing Technology, Chinese Academy of Sciences
*
* DiffTest is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/
#ifndef __XDMA_H__
#define __XDMA_H__

#include "common.h"
#include "diffstate.h"
#include "mpool.h"
#include <queue>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <thread>
#include <vector>

#define WITH_FPGA
typedef struct FpgaPackgeHead {
  DiffTestState difftestinfo;
  uint8_t corid;
} FpgaPackgeHead;

class FpgaXdma {
public:
  struct FpgaPackgeHead *shmadd_recv;
  MemoryPool xdma_mempool;
  DiffTestState difftest_pack[NUM_CORES] = {};
  int shmid_recv;
  int ret_recv;
  key_t key_recv;

  int fd_c2h;
  int fd_interrupt;
  bool running = false;

  unsigned int recv_size = sizeof(FpgaPackgeHead);
  unsigned long old_exec_instr = 0;

  std::condition_variable diff_filled_cv;
  std::condition_variable diff_empile_cv;
  std::mutex diff_mtx;
  bool diff_packge_filled = false;
  FpgaXdma(const char *device_name);
  ~FpgaXdma() {
    stop_thansmit_thread();
  };

  void set_dma_fd_block();

  // thread api
  void start_transmit_thread();
  void stop_thansmit_thread();
  void read_xdma_thread();
  void write_difftest_thread();

private:
  std::thread receive_thread;
  std::thread process_thread;

  static void handle_sigint(int sig);
};

#endif
