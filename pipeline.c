#include <stdbool.h>
#include "cache.h"
#include "riscv.h"
#include "types.h"
#include "utils.h"
#include "pipeline.h"
#include "stage_helpers.h"

uint64_t total_cycle_counter = 0;
uint64_t miss_count = 0;
uint64_t hit_count = 0;
uint64_t stall_counter = 0;
uint64_t branch_counter = 0;
uint64_t fwd_exex_counter = 0;
uint64_t fwd_exmem_counter = 0;

simulator_config_t sim_config = {0};

///////////////////////////////////////////////////////////////////////////////

void bootstrap(pipeline_wires_t* pwires_p, pipeline_regs_t* pregs_p, regfile_t* regfile_p)
{
  // PC src must get the same value as the default PC value
  pwires_p->pc_src0 = regfile_p->PC;
}

///////////////////////////
/// STAGE FUNCTIONALITY ///
///////////////////////////

/**
 * STAGE  : stage_fetch
 * output : ifid_reg_t
 **/ 
ifid_reg_t stage_fetch(pipeline_wires_t* pwires_p, regfile_t* regfile_p, Byte* memory_p)
{
	// Increment instruction address by 4, or if using branch using pc_src1
	if (pwires_p->pcsrc == 1)
	{
		pwires_p->pc_src0 = pwires_p->pc_src1;
		pwires_p->pcsrc = 0;
	}
	regfile_p->PC = pwires_p->pc_src0;

	// Initialize ifid register to 0
	ifid_reg_t ifid_reg = { 0 };

	// Obtain the instruction bits by adding current instruction address to base address of memory
	// Typecast to 32-bit unsigned and dereference pointer to obtain instructions bits
	ifid_reg.instr_bits = *(uint32_t*)(memory_p + regfile_p->PC);

	// Printing commands
#ifdef DEBUG_CYCLE
	printf("[IF ]: Instruction [%08x]@[%08x]: ", ifid_reg.instr_bits, regfile_p->PC);
	decode_instruction(ifid_reg.instr_bits);
#endif

	// Set instruction variable of register to the parsed instruction bits
	ifid_reg.instr = parse_instruction(ifid_reg.instr_bits);

	// Set instruction address of register to current instruction address
	ifid_reg.instr_addr = regfile_p->PC;

	pwires_p->pc_src0 += 4;

	// Return ifid register
	return ifid_reg;
}

/**
 * STAGE  : stage_decode
 * output : idex_reg_t
 **/ 
idex_reg_t stage_decode(ifid_reg_t ifid_reg, pipeline_wires_t* pwires_p, regfile_t* regfile_p)
{
	// Initialize idex register control logic
	idex_reg_t idex_reg = gen_control(ifid_reg.instr);

	// Initialize instruction from ifid register
	idex_reg.instr = ifid_reg.instr;

	// Initialize instruction address from ifid register
	idex_reg.instr_addr = ifid_reg.instr_addr;

	// Initialize instruction bits from ifid register
	idex_reg.instr_bits = ifid_reg.instr_bits;

	// Based on opcode, set decoded values
	switch (idex_reg.instr.opcode)
	{
		// R-Type
	case 0x33:
		idex_reg.rd = idex_reg.instr.rtype.rd;
		idex_reg.rs1 = idex_reg.instr.rtype.rs1;
		idex_reg.rs2 = idex_reg.instr.rtype.rs2;
		idex_reg.funct3 = idex_reg.instr.rtype.funct3;
		idex_reg.funct7 = idex_reg.instr.rtype.funct7;

		idex_reg.rd_val = regfile_p->R[idex_reg.rd];
		idex_reg.rs1_val = regfile_p->R[idex_reg.rs1];
		idex_reg.rs2_val = regfile_p->R[idex_reg.rs2];
		break;

		// SB-Type
	case 0x63:
		idex_reg.rs1 = idex_reg.instr.sbtype.rs1;
		idex_reg.rs2 = idex_reg.instr.sbtype.rs2;
		idex_reg.funct3 = idex_reg.instr.sbtype.funct3;
		idex_reg.imm = gen_imm(idex_reg.instr);

		idex_reg.rs1_val = regfile_p->R[idex_reg.rs1];
		idex_reg.rs2_val = regfile_p->R[idex_reg.rs2];
		break;

		// S-Type
	case 0x23:
		idex_reg.rs1 = idex_reg.instr.stype.rs1;
		idex_reg.rs2 = idex_reg.instr.stype.rs2;
		idex_reg.funct3 = idex_reg.instr.stype.funct3;
		idex_reg.imm = gen_imm(idex_reg.instr);

		idex_reg.rs1_val = regfile_p->R[idex_reg.rs1];
		idex_reg.rs2_val = regfile_p->R[idex_reg.rs2];
		break;

		// UJ-Type
	case 0x6F:
		idex_reg.rd = idex_reg.instr.ujtype.rd;
		idex_reg.imm = gen_imm(idex_reg.instr);

		idex_reg.rd_val = regfile_p->R[idex_reg.rd];
		break;

		// I-Type
	case 0x03:
		idex_reg.rd = idex_reg.instr.itype.rd;
		idex_reg.rs1 = idex_reg.instr.itype.rs1;
		idex_reg.funct3 = idex_reg.instr.itype.funct3;
		idex_reg.imm = gen_imm(idex_reg.instr);

		idex_reg.rd_val = regfile_p->R[idex_reg.rd];
		idex_reg.rs1_val = regfile_p->R[idex_reg.rs1];
		break;

		// I-Type (w/o Load)
	case 0x13:
		idex_reg.rd = idex_reg.instr.itype.rd;
		idex_reg.rs1 = idex_reg.instr.itype.rs1;
		idex_reg.funct3 = idex_reg.instr.itype.funct3;
		idex_reg.imm = gen_imm(idex_reg.instr);

		idex_reg.rd_val = regfile_p->R[idex_reg.rd];
		idex_reg.rs1_val = regfile_p->R[idex_reg.rs1];
		break;

		// U-Type
	case 0x37:
		idex_reg.rd = idex_reg.instr.utype.rd;
		idex_reg.imm = gen_imm(idex_reg.instr);

		idex_reg.rd_val = regfile_p->R[idex_reg.rd];
		break;

		// Undefined
	default:
		break;
	};

	// Printing commands
#ifdef DEBUG_CYCLE
	printf("[ID ]: Instruction [%08x]@[%08x]: ", idex_reg.instr_bits, idex_reg.instr_addr);
	decode_instruction(idex_reg.instr_bits);
#endif

	// Return idex register
	return idex_reg;
}

/**
 * STAGE  : stage_execute
 * output : exmem_reg_t
 **/ 
exmem_reg_t stage_execute(idex_reg_t idex_reg, pipeline_wires_t* pwires_p)
{
	// Initialize exmem register to 0
	exmem_reg_t exmem_reg = { 0 };

	// Initialize instruction from idex register
	exmem_reg.instr = idex_reg.instr;

	// Initialize instruction address from idex register
	exmem_reg.instr_addr = idex_reg.instr_addr;

	// Initialize instruction bits from idex register
	exmem_reg.instr_bits = idex_reg.instr_bits;

	// CALL gen_alu_control HERE!
	uint32_t control = gen_alu_control(idex_reg);

	// Move over decoded values and control logic
	exmem_reg.rd = idex_reg.rd;
	exmem_reg.rs1 = idex_reg.rs1;
	exmem_reg.rs2 = idex_reg.rs2;
	exmem_reg.funct3 = idex_reg.funct3;
	exmem_reg.funct7 = idex_reg.funct7;
	exmem_reg.imm = idex_reg.imm;

	exmem_reg.rd_val = idex_reg.rd_val;
	exmem_reg.rs1_val = idex_reg.rs1_val;
	exmem_reg.rs2_val = idex_reg.rs2_val;

	exmem_reg.alu_src = idex_reg.alu_src;
	exmem_reg.pc_src = idex_reg.pc_src;
	exmem_reg.reg_write = idex_reg.reg_write;
	exmem_reg.mem_read = idex_reg.mem_read;
	exmem_reg.mem_write = idex_reg.mem_write;
	exmem_reg.mem_to_reg = idex_reg.mem_to_reg;
	exmem_reg.branch = idex_reg.branch;

	// Using rs1 & rs2 for ALU (R instructions)
	if (exmem_reg.alu_src == 0 && exmem_reg.pc_src == 0)
	{
		exmem_reg.alu_result = execute_alu(exmem_reg.rs1_val, exmem_reg.rs2_val, control);
	}

	// Using PC & imm (UJ instructions)
	if (exmem_reg.alu_src == 1 && exmem_reg.pc_src == 1)
	{
		exmem_reg.alu_result = execute_alu(exmem_reg.instr_addr, 4, control);
	}

	// Using rs1 & imm (S, I, I w/o Load, or U instructions)
	if (exmem_reg.alu_src == 1 && exmem_reg.pc_src == 0)
	{
		// U instructions exception
		if (exmem_reg.instr.opcode == 0x37)
		{
			exmem_reg.alu_result = execute_alu(exmem_reg.imm, 12, control);
		}
		// SRAI, SRLI, SLLI instructions exception
		else if (exmem_reg.instr.opcode == 0x13 && (exmem_reg.funct3 == 0x1 || exmem_reg.funct3 == 0x5))
		{
			exmem_reg.alu_result = execute_alu(exmem_reg.rs1_val, exmem_reg.imm & 0x1F, control);
		}
		else
		{
			exmem_reg.alu_result = execute_alu(exmem_reg.rs1_val, exmem_reg.imm, control);
		}
	}

	// Printing commands
#ifdef DEBUG_CYCLE
	printf("[EX ]: Instruction [%08x]@[%08x]: ", exmem_reg.instr_bits, exmem_reg.instr_addr);
	decode_instruction(exmem_reg.instr_bits);
#endif

	// Return exmem register
	return exmem_reg;
}

/**
 * STAGE  : stage_mem
 * output : memwb_reg_t
 **/ 
memwb_reg_t stage_mem(exmem_reg_t exmem_reg, pipeline_wires_t* pwires_p, Byte* memory_p, Cache* cache_p)
{
	memwb_reg_t memwb_reg = { 0 };

	// Initialize instruction from exmem register
	memwb_reg.instr = exmem_reg.instr;

	// Initialize instruction address from exmem register
	memwb_reg.instr_addr = exmem_reg.instr_addr;

	// Initialize instruction bits from exmem register
	memwb_reg.instr_bits = exmem_reg.instr_bits;

	// Move over decoded values and control logic
	memwb_reg.rd = exmem_reg.rd;
	memwb_reg.rs1 = exmem_reg.rs1;
	memwb_reg.rs2 = exmem_reg.rs2;
	memwb_reg.funct3 = exmem_reg.funct3;
	memwb_reg.funct7 = exmem_reg.funct7;
	memwb_reg.imm = exmem_reg.imm;

	memwb_reg.rd_val = exmem_reg.rd_val;
	memwb_reg.rs1_val = exmem_reg.rs1_val;
	memwb_reg.rs2_val = exmem_reg.rs2_val;

	memwb_reg.alu_src = exmem_reg.alu_src;
	memwb_reg.pc_src = exmem_reg.pc_src;
	memwb_reg.reg_write = exmem_reg.reg_write;
	memwb_reg.mem_read = exmem_reg.mem_read;
	memwb_reg.mem_write = exmem_reg.mem_write;
	memwb_reg.mem_to_reg = exmem_reg.mem_to_reg;
	memwb_reg.branch = exmem_reg.branch;

	memwb_reg.alu_result = exmem_reg.alu_result;

	// Branch
	if (gen_branch(memwb_reg) == true)
	{
		pwires_p->pcsrc = true;
		pwires_p->pc_src1 = memwb_reg.instr_addr + memwb_reg.imm;
	}
	else
	{
		pwires_p->pcsrc = false;
	}

	// Write to Memory Enabled (S instructions)
	if (memwb_reg.mem_write == 1)
	{
		memory_p[memwb_reg.alu_result] = memwb_reg.rs2_val;
	}

	// Memory to Register Enabled (I instructions)
	if (memwb_reg.mem_to_reg == 1)
	{
		memwb_reg.mem_value = memory_p[memwb_reg.alu_result];
	}

	// Printing commands
#ifdef DEBUG_CYCLE
	printf("[MEM]: Instruction [%08x]@[%08x]: ", memwb_reg.instr_bits, memwb_reg.instr_addr);
	decode_instruction(memwb_reg.instr_bits);
#endif

	// Return memwb_reg register
	return memwb_reg;
}

/**
 * STAGE  : stage_writeback
 * output : nothing - The state of the register file may be changed
 **/ 
void stage_writeback(memwb_reg_t memwb_reg, pipeline_wires_t* pwires_p, regfile_t* regfile_p)
{
	// Write to Rd from ALU Enabled (R, I w/o L, UJ, or U instructions)
	if (memwb_reg.reg_write == 1 && memwb_reg.mem_to_reg == 0)
	{
		regfile_p->R[memwb_reg.rd] = memwb_reg.alu_result;
	}

	// Write to Rd from Memory Enabled (I)
	if (memwb_reg.reg_write == 1 && memwb_reg.mem_to_reg == 1)
	{
		regfile_p->R[memwb_reg.rd] = memwb_reg.mem_value;
	}

	regfile_p->R[0] = 0;

	// Printing commands
#ifdef DEBUG_CYCLE
	printf("[WB ]: Instruction [%08x]@[%08x]: ", memwb_reg.instr_bits, memwb_reg.instr_addr);
	decode_instruction(memwb_reg.instr_bits);
#endif
}

///////////////////////////////////////////////////////////////////////////////

/** 
 * excite the pipeline with one clock cycle
 **/
void cycle_pipeline(regfile_t* regfile_p, Byte* memory_p, Cache* cache_p, pipeline_regs_t* pregs_p, pipeline_wires_t* pwires_p, bool* ecall_exit)
{
  #ifdef DEBUG_CYCLE
  printf("v==============");
  printf("Cycle Counter = %5ld", total_cycle_counter);
  printf("==============v\n\n");
  #endif

  // process each stage

  /* Output               |    Stage      |       Inputs  */
  pregs_p->ifid_preg.inp  = stage_fetch     (pwires_p, regfile_p, memory_p);
  
  pregs_p->idex_preg.inp  = stage_decode    (pregs_p->ifid_preg.out, pwires_p, regfile_p);

  pregs_p->exmem_preg.inp = stage_execute   (pregs_p->idex_preg.out, pwires_p);

  pregs_p->memwb_preg.inp = stage_mem       (pregs_p->exmem_preg.out, pwires_p, memory_p, cache_p);

                            stage_writeback (pregs_p->memwb_preg.out, pwires_p, regfile_p);

  // update all the output registers for the next cycle from the input registers in the current cycle
  pregs_p->ifid_preg.out  = pregs_p->ifid_preg.inp;
  pregs_p->idex_preg.out  = pregs_p->idex_preg.inp;
  pregs_p->exmem_preg.out = pregs_p->exmem_preg.inp;
  pregs_p->memwb_preg.out = pregs_p->memwb_preg.inp;

  /////////////////// NO CHANGES BELOW THIS ARE REQUIRED //////////////////////

  // increment the cycle
  total_cycle_counter++;

  #ifdef DEBUG_REG_TRACE
  print_register_trace(regfile_p);
  #endif

  /**
   * check ecall condition
   * To do this, the value stored in R[10] (a0 or x10) should be 10.
   * Hence, the ecall condition is checked by the existence of following
   * two instructions in sequence:
   * 1. <instr>  x10, <val1>, <val2> 
   * 2. ecall
   * 
   * The first instruction must write the value 10 to x10.
   * The second instruction is the ecall (opcode: 0x73)
   * 
   * The condition checks whether the R[10] value is 10 when the
   * `memwb_reg.instr.opcode` == 0x73 (to propagate the ecall)
   * 
   * If more functionality on ecall needs to be added, it can be done
   * by adding more conditions on the value of R[10]
   */
  if( (pregs_p->memwb_preg.out.instr.bits == 0x00000073) &&
      (regfile_p->R[10] == 10) )
  {
    *(ecall_exit) = true;
  }
}



