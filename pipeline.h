#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#include "types.h"
#include "cache.h"
#include <stdbool.h>

// enable `DEBUG_CYCLE` this after completing the code in each stage
#define DEBUG_CYCLE
#define DEBUG_REG_TRACE         // prints the register trace after each cycle
//#define DEBUG_CACHE_TRACE       // prints the cache trace at the end of program

///////////////////////////////////////////////////////////////////////////////
/// Functionality
///////////////////////////////////////////////////////////////////////////////

#define MEM_LATENCY 0
extern simulator_config_t sim_config;
extern uint64_t miss_count;
extern uint64_t hit_count;
extern uint64_t total_cycle_counter;

///////////////////////////////////////////////////////////////////////////////
/// RISC-V Pipeline Register Types
///////////////////////////////////////////////////////////////////////////////

typedef struct
{
  Instruction instr;
  uint32_t    instr_addr;
  /**
   * Add other fields here
   */
  //add by A
  uint32_t    instr_bits;
  uint32_t    pc_next;   // Address of the next instruction
  uint32_t    imm;       // Immediate value for instructions
  uint32_t    rs1;       // Source register 1
  uint32_t    rs2;       // Source register 2
  uint32_t    rd;        // Destination register
  //add by A
}ifid_reg_t;

typedef struct
{
  uint32_t    instr_bits;
  Instruction instr;
  uint32_t    instr_addr;
  uint32_t    pc_next;
  uint32_t    imm;
  uint32_t    rs1_val;   // Value of source register 1
  uint32_t    rs2_val;   // Value of source register 2
  uint32_t    rd_val;
  uint32_t    rd;
  uint32_t    funct3;
  uint32_t    funct7;
  uint32_t    opcode;
  

  uint32_t    rs1;
  uint32_t    rs2;

  uint32_t    alu_op;    // ALU operation code
  bool        pc_src;
  bool        branch;
  bool        alu_src;  //// Control signal for Use immediate value
  bool        mem_read;  // Control signal for memory read
  bool        mem_write; // Control signal for memory write
  bool        reg_write; // Control signal for register write
  bool        mem_to_reg;// Control signal for memory to register
  //add by A
}idex_reg_t;

typedef struct
{
  uint32_t    instr_bits;
  Instruction instr;
  uint32_t    instr_addr;
  uint32_t    alu_result; // Result from the ALU
  uint32_t    rs2_val;    // Value to be written to memory (store instructions)
  uint32_t    rd;
  uint32_t    rs1;
  uint32_t    rs2;
  uint32_t    funct3;
  uint32_t    funct7;
  uint32_t    imm;
  uint32_t    rd_val;
  uint32_t    rs1_val;
  uint32_t    rs2_vla;
  bool        branch;
  bool        alu_src;
  bool        pc_src;
  bool        mem_read;
  bool        mem_write;
  bool        reg_write;
  bool        mem_to_reg;
  //add by A
}exmem_reg_t;

typedef struct
{
  uint32_t    instr_bits;
  Instruction instr;
  uint32_t    instr_addr;
  uint32_t    alu_result;
  uint32_t    mem_data;   // Data read from memory
  uint32_t    rd;
  uint32_t    rs1;
  uint32_t    rs2;
  uint32_t    funct3;
  uint32_t    funct7;
  uint32_t    imm;
  uint32_t    rd_val;
  uint32_t    rs1_val;
  uint32_t    rs2_val;
  uint32_t    mem_value;   // Data read from memory
  
  bool        reg_write;
  bool        mem_to_reg;
  bool        alu_src;
  bool        pc_src;
  bool        branch;
  bool        mem_read;
  bool        mem_write;

  //add by A
}memwb_reg_t;


///////////////////////////////////////////////////////////////////////////////
/// Register types with input and output variants for simulator
///////////////////////////////////////////////////////////////////////////////

typedef struct
{
  ifid_reg_t inp;
  ifid_reg_t out;
}ifid_reg_pair_t;

typedef struct
{
  idex_reg_t inp;
  idex_reg_t out;
}idex_reg_pair_t;

typedef struct
{
  exmem_reg_t inp;
  exmem_reg_t out;
}exmem_reg_pair_t;

typedef struct
{
  memwb_reg_t inp;
  memwb_reg_t out;
}memwb_reg_pair_t;

///////////////////////////////////////////////////////////////////////////////
/// Functional pipeline requirements
///////////////////////////////////////////////////////////////////////////////

typedef struct
{
  ifid_reg_pair_t  ifid_preg;
  idex_reg_pair_t  idex_preg;
  exmem_reg_pair_t exmem_preg;
  memwb_reg_pair_t memwb_preg;
}pipeline_regs_t;

typedef struct
{
  bool      pcsrc;
  uint32_t  pc_src0;
  uint32_t  pc_src1;
  /**
   * Add other fields here
   */
  uint32_t  alu_control; // ALU control signal
  bool      branch_taken; // Branch control signal
}pipeline_wires_t;


///////////////////////////////////////////////////////////////////////////////
/// Function definitions for different stages
///////////////////////////////////////////////////////////////////////////////

/**
 * output : ifid_reg_t
 **/ 
ifid_reg_t stage_fetch(pipeline_wires_t* pwires_p, regfile_t* regfile_p, Byte* memory_p);

/**
 * output : idex_reg_t
 **/ 
idex_reg_t stage_decode(ifid_reg_t ifid_reg, pipeline_wires_t* pwires_p, regfile_t* regfile_p);

/**
 * output : exmem_reg_t
 **/ 
exmem_reg_t stage_execute(idex_reg_t idex_reg, pipeline_wires_t* pwires_p);

/**
 * output : memwb_reg_t
 **/ 
memwb_reg_t stage_mem(exmem_reg_t exmem_reg, pipeline_wires_t* pwires_p, Byte* memory, Cache* cache_p);

/**
 * output : write_data
 **/ 
void stage_writeback(memwb_reg_t memwb_reg, pipeline_wires_t* pwires_p, regfile_t* regfile_p);

void cycle_pipeline(regfile_t* regfile_p, Byte* memory_p, Cache* cache_p, pipeline_regs_t* pregs_p, pipeline_wires_t* pwires_p, bool* ecall_exit);

void bootstrap(pipeline_wires_t* pwires_p, pipeline_regs_t* pregs_p, regfile_t* regfile_p);

#endif  // __PIPELINE_H__

