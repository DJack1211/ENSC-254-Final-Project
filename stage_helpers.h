#ifndef __STAGE_HELPERS_H__
#define __STAGE_HELPERS_H__



#include <stdio.h>
#include "utils.h"
#include "pipeline.h"

/// EXECUTE STAGE HELPERS ///

/**
 * input  : idex_reg_t
 * output : uint32_t alu_control signal
 **/
uint32_t gen_alu_control(idex_reg_t idex_reg)
{


    //version1
  
  /**
   * YOUR CODE HERE
   */
  
  /*
  //add by A
  uint32_t alu_control = 0;
 switch(idex_reg.instr.rtype.funct3) {
        case 0x0:
            if (idex_reg.instr.rtype.funct7 == 0x00) {
                alu_control = 0x0; // ADD
            } else if (idex_reg.instr.rtype.funct7 == 0x20) {
                alu_control = 0x1; // SUB
            }
            break;
        case 0x7:
            alu_control = 0x2; // AND
            break;
        case 0x6:
            alu_control = 0x3; // OR
            break;
        case 0x4:
            alu_control = 0x4; // XOR
            break;
        case 0x2:
            alu_control = 0x5; // SLT
            break;
        case 0x3:
            alu_control = 0x6; // SLTU
            break;
        case 0x1:
            alu_control = 0x7; // SLL
            break;
        case 0x5:
            if (idex_reg.instr.rtype.funct7 == 0x00) {
                alu_control = 0x8; // SRL
            } else if (idex_reg.instr.rtype.funct7 == 0x20) {
                alu_control = 0x9; // SRA
            }
            break;
        default:
            break;
    }
  //add by A
  return alu_control;
  //1
  */

/*
 uint32_t alu_control = 0;

    switch (idex_reg.instr.opcode) {
        case 0x33: // R-type instructions
            switch (idex_reg.instr.rtype.funct3) {
                case 0x0:
                    if (idex_reg.instr.rtype.funct7 == 0x00) {
                        alu_control = 0x0; // ADD
                    } else if (idex_reg.instr.rtype.funct7 == 0x20) {
                        alu_control = 0x1; // SUB
                    }
                    break;
                case 0x7:
                    alu_control = 0x2; // AND
                    break;
                case 0x6:
                    alu_control = 0x3; // OR
                    break;
                case 0x4:
                    alu_control = 0x4; // XOR
                    break;
                case 0x2:
                    alu_control = 0x5; // SLT
                    break;
                case 0x3:
                    alu_control = 0x6; // SLTU
                    break;
                case 0x1:
                    alu_control = 0x7; // SLL
                    break;
                case 0x5:
                    if (idex_reg.instr.rtype.funct7 == 0x00) {
                        alu_control = 0x8; // SRL
                    } else if (idex_reg.instr.rtype.funct7 == 0x20) {
                        alu_control = 0x9; // SRA
                    }
                    break;
                default:
                    break;
            }
            break;
        case 0x13: // I-type arithmetic instructions
            switch (idex_reg.instr.itype.funct3) {
                case 0x0:
                    alu_control = 0x0; // ADDI
                    break;
                case 0x7:
                    alu_control = 0x2; // ANDI
                    break;
                case 0x6:
                    alu_control = 0x3; // ORI
                    break;
                case 0x4:
                    alu_control = 0x4; // XORI
                    break;
                case 0x2:
                    alu_control = 0x5; // SLTI
                    break;
                case 0x3:
                    alu_control = 0x6; // SLTIU
                    break;
                case 0x1:
                    alu_control = 0x7; // SLLI
                    break;
                case 0x5:
                    if (idex_reg.instr.itype.imm & 0x400) {
                        alu_control = 0x9; // SRAI
                    } else {
                        alu_control = 0x8; // SRLI
                    }
                    break;
                default:
                    break;
            }
            break;
        case 0x03: // I-type load instructions
            alu_control = 0x0; // All loads use ADD to calculate the address
            break;
        default:
            break;
    }

    return alu_control;   
    */


 uint32_t alu_control = 0;
  uint32_t funct3 = (idex_reg.instr_bits<<17)>>29;
  uint32_t funct7 = idex_reg.instr_bits>>25;

	switch(idex_reg.alu_op) { 
	case 0x00: //R-type
	  	switch(funct3) {
			case 0x0:
			  	switch(funct7) {
			    		case 0x0: //add
					alu_control = 0x00;
					break;
					case 0x01: //mul
					alu_control = 0x01;
					break;
			   		case 0x20: //sub
					alu_control = 0x02;
					break;
				}
			break;	
		       	case 0x1:
				switch(funct7) {
					case 0x00: //sll
					alu_control = 0x03;
					break;
					case 0x01: //mulh
					alu_control = 0x04;
					break;
				}
			break;
			case 0x2:
				switch(funct7) {
					case 0x00: //slt
					alu_control = 0x05;
					break;
				}
			break;
			case 0x4:
				switch(funct7) {
					case 0x00: //xor
					alu_control = 0x06;
					break;
					case 0x01: //div
					alu_control = 0x07;
					break;
				}
			break;
			case 0x5:
				switch(funct7) {
					case 0x00: //srl
					alu_control = 0x08;
					break;
					case 0x20: //sra
					alu_control = 0x09;
					break;
				}
			break;
			case 0x6:
				switch(funct7) {
					case 0x00: //or
					alu_control = 0x0A;
					break;
					case 0x01: //rem
					alu_control = 0x0B;
			}
			break;
			case 0x7:
				switch(funct7) {
					case 0x00: //and
					alu_control = 0x0C;
					break;
				}
			break;
		}
		break;

	case 0x1: //I-type (no load)
		switch(funct3) {
      			case 0x0: //addi
				alu_control = 0x00;
       				break;
      			case 0x1: //slli
				alu_control = 0x03;
        			break;
      			case 0x2: //slti
				alu_control = 0x05;
        			break;
      			case 0x4: //xori
				alu_control = 0x06;
        			break;
      			case 0x5:
         		switch(idex_reg.instr.rtype.funct7) {
        			case 0x00: //srli
					alu_control = 0x08;
            				break;
        			case 0x20: //srai
					alu_control = 0x09;
            				break;
				}
			break;
      			case 0x6: //ori
				alu_control = 0x0A;
        			break;
      			case 0x7: //andi
				alu_control = 0x0C;
        			break;
		}
		break;


	case 0x02: //I-type (load)/stype
 		alu_control = 0x00;
		break;

	case 0x03: //SB-type
             	alu_control = 0x2;
         	break;

	case 0x4: //U-type (lui)
		alu_control = 0x3;
		break;
	}
	return alu_control;




}

/**
 * input  : alu_inp1, alu_inp2, alu_control
 * output : uint32_t alu_result
 **/
uint32_t execute_alu(uint32_t alu_inp1, uint32_t alu_inp2, uint32_t alu_control)
{


  
  // switch(alu_control){
  //   case 0x0: //add
  //     result = alu_inp1 + alu_inp2;
  //     break;
    /**
     * YOUR CODE HERE
     */
  //   default:
  //     result = 0xBADCAFFE;
  //     break;
  // };

/*
  //version1
  uint32_t result;
  switch(alu_control) {
        case 0x0: // ADD
            result = alu_inp1 + alu_inp2;
            break;
        case 0x1: // SUB
            result = alu_inp1 - alu_inp2;
            break;
        case 0x2: // AND
            result = alu_inp1 & alu_inp2;
            break;
        case 0x3: // OR
            result = alu_inp1 | alu_inp2;
            break;
        case 0x4: // XOR
            result = alu_inp1 ^ alu_inp2;
            break;
        case 0x5: // SLT
            result = ((int32_t)alu_inp1 < (int32_t)alu_inp2)?1:0;
            break;
        case 0x6: // SLTU
            result = alu_inp1 < alu_inp2;
            break;
        case 0x7: // SLL
            result = alu_inp1 << (alu_inp2 & 0x1F);//nono
            break;
        case 0x8: // SRL
            result = alu_inp1 >> (alu_inp2 & 0x1F);
            break;
        case 0x9: // SRA
            result = (int32_t)alu_inp1 >> (alu_inp2 & 0x1F);
            break;
        default:
            result = 0xBADCAFFE;
            break;
    }
  //1
  return result;
  */

/*
 uint32_t result = 0;

    switch (alu_control) {
        case 0x0: // ADD
            result = alu_inp1 + alu_inp2;
            break;
        case 0x1: // SUB
            result = alu_inp1 - alu_inp2;
            break;
        case 0x2: // AND
            result = alu_inp1 & alu_inp2;
            break;
        case 0x3: // OR
            result = alu_inp1 | alu_inp2;
            break;
        case 0x4: // XOR
            result = alu_inp1 ^ alu_inp2;
            break;
        case 0x5: // SLT
            result = (int32_t)alu_inp1 < (int32_t)alu_inp2 ? 1 : 0;
            break;
        case 0x6: // SLTU
            result = alu_inp1 < alu_inp2 ? 1 : 0;
            break;
        case 0x7: // SLL
            result = alu_inp1 << (alu_inp2 & 0x1F);
            break;
        case 0x8: // SRL
            result = alu_inp1 >> (alu_inp2 & 0x1F);
            break;
        case 0x9: // SRA
            result = (int32_t)alu_inp1 >> (alu_inp2 & 0x1F);
            break;
        default:
            result = 0xBADCAFFE; // An invalid ALU control signal
            break;
    }

    return result;
    */
   int32_t result;
  switch(alu_control){
    case 0x0: //add
    	result = alu_inp1 + alu_inp2;
    	break;
    case 0x1: //mul
    	result = alu_inp1 * alu_inp2;
    	break;
    case 0x2: //sub
    	result = alu_inp1 - alu_inp2;
    	break;
    case 0x3: //sll
    	result = alu_inp1 << alu_inp2;
    	break;
    case 0x4: //mulh
    	result = (alu_inp1 * alu_inp2) >> 32;
    	break;
    case 0x5: //slt
    	result = (alu_inp1 < alu_inp2);
    	break;
    case 0x6: //xor
    	result = alu_inp1 ^ alu_inp2;
    	break;
    case 0x7: //div
    	result = alu_inp1 / alu_inp2;
    	break;
    case 0x8: //srl
    	result = alu_inp1 >> alu_inp2;
    	break;
    case 0x9: //sra
    	result = alu_inp1 >> alu_inp2;
    	break;
    case 0xA: //or
    	result = alu_inp1 | alu_inp2;
    	break;
    case 0xB: //rem
    	result = alu_inp1 % alu_inp2;
    	break;
    case 0xC: //and
    	result = alu_inp1 & alu_inp2;
    	break;
    default:
        result = 0xBADCAFFE;
        break;
  };	
  return result;
}

/// DECODE STAGE HELPERS ///
int sign_extend_number2(unsigned int field, unsigned int n) {//number and number of digits
        /* YOUR CODE HERE */
        unsigned int sign_bit = field & (1U << (n - 1));//we need know either its 0 or 1 //1000-1=0111
        // sign_bit now can be inputed
        if (sign_bit){
       return field | (~((1U << n) - 1));//its 1 negative
      } //(111...1100)
        else {//its 0 positive
       return field & ((1U << n) - 1);//& -> 0, eg.010 & 111 = 010->(000...010)
        }
}
int get_branch_offset2(Instruction instruction) {
  /* YOUR CODE HERE */
  int imm5 = instruction.sbtype.imm5;
  int imm7 = instruction.sbtype.imm7;
  int imm4_1 = imm5 & 0x1E;
  int imm11 = imm5 & 0x1;
  int imm10_5 = imm7 & 0x3F; 
  int imm12 = imm7 & 0x40;
  return sign_extend_number2((imm12 << 6) | (imm11 << 11) | (imm10_5 << 5) | (imm4_1), 12);
}

/* Returns the number of bytes (from the current PC) to the jump label using the
 * given jump instruction */
int get_jump_offset2(Instruction instruction) {
  /* YOUR CODE HERE */
	int imm = instruction.ujtype.imm;
	int imm20 = ((imm >> 19) & 0x1);
	int imm10_1 = ((imm >> 9) & 0x3FF);
	int imm11 = ((imm >> 8) & 0x1);
	int imm19_12 = ((imm >> 0) & 0xFF);
	
  return sign_extend_number2((imm20 << 20) | (imm19_12 << 12) | (imm11 << 11) | (imm10_1 << 1), 21);
}

/* Returns the number of bytes (from the current PC) to the base address using the
 * given store instruction */
int get_store_offset2(Instruction instruction) {
  /* YOUR CODE HERE */
  int imm5 = instruction.stype.imm5;
  int imm7 = instruction.stype.imm7;
  int imm11_5 = imm7 & 0x7F;
  int imm4_0 = imm5 & 0x1F;
  return sign_extend_number2((imm11_5 << 5) | (imm4_0 << 0), 12);
}
/**
 * input  : Instruction
 * output : idex_reg_t
 **/
uint32_t gen_imm(Instruction instruction)
{
  int imm_val = 0;
  /**
   * YOUR CODE HERE
   */
  // switch(instruction.opcode) {
  //       case 0x63: //B-type
  //           imm_val = get_branch_offset(instruction);
  //           break;
  //       /**
  //        * YOUR CODE HERE
  //        */
  //       default: // R and undefined opcode
  //           break;
  //   };

  //add by A
  switch(instruction.opcode) {
        case 0x13: // I-type
            imm_val = (instruction.itype.imm << 20) >> 20;
	        imm_val = sign_extend_number2(imm_val, 12);
            break;
        case 0x03: // Load (I-type)
            imm_val = (instruction.itype.imm << 20) >> 20;
            imm_val = sign_extend_number2(imm_val, 12);
            break;
        case 0x23: // S-type
            imm_val = get_store_offset2(instruction);
            break;
        case 0x63: // B-type
            imm_val = get_branch_offset2(instruction);
            break;
        case 0x37: // U-type LUI
            imm_val = (instruction.utype.imm >> 12) << 12;
            break;
        case 0x17: // U-type AUIPC
            imm_val = (instruction.utype.imm >> 12) << 12;
            break;
        case 0x6F: // J-type JAL
            imm_val = sign_extend_number2(get_jump_offset2(instruction), 20);
            break;
	case 0x67: // J-type JALR
	    imm_val = sign_extend_number2(get_jump_offset2(instruction), 20);
        default:
            break;
    }
  //add by A
    return imm_val;
}

/**
 * generates all the control logic that flows around in the pipeline
 * input  : Instruction
 * output : idex_reg_t
 **/
idex_reg_t gen_control(Instruction instruction)
{
  idex_reg_t idex_reg = {0};
  // switch(instruction.opcode) {
  //     case 0x33:  //R-type
  //       /**
  //        * YOUR CODE HERE
  //        */
  //         break;
  //     default:  // Remaining opcodes
  //         break;
  // }

  //
//  switch(instruction.opcode) {
//         case 0x33:  // R-type
//             idex_reg.alu_op = 2; // ALU operation
//             idex_reg.alu_src = 0; // Use register values
//             idex_reg.mem_read = 0; // Not a memory read
//             idex_reg.mem_write = 0; // Not a memory write
//             idex_reg.reg_write = 1; // Write to register
//             idex_reg.mem_to_reg = 0; // ALU result to register
//             break;
//         case 0x03:  // I-type (load)
//             idex_reg.alu_op = 0; // ALU operation
//             idex_reg.alu_src = 1; // Use immediate value
//             idex_reg.mem_read = 1; // Memory read
//             idex_reg.mem_write = 0; // Not a memory write
//             idex_reg.reg_write = 1; // Write to register
//             idex_reg.mem_to_reg = 1; // Memory result to register
//             break;
//         case 0x13:  // I-type (ALU immediate)
//             idex_reg.alu_op = 0; // ALU operation
//             idex_reg.alu_src = 1; // Use immediate value
//             idex_reg.mem_read = 0; // Not a memory read
//             idex_reg.mem_write = 0; // Not a memory write
//             idex_reg.reg_write = 1; // Write to register
//             idex_reg.mem_to_reg = 0; // ALU result to register
//             break;
//         case 0x23:  // S-type (store)
//             idex_reg.alu_op = 0; // ALU operation
//             idex_reg.alu_src = 1; // Use immediate value
//             idex_reg.mem_read = 0; // Not a memory read
//             idex_reg.mem_write = 1; // Memory write
//             idex_reg.reg_write = 0; // Not a register write
//             idex_reg.mem_to_reg = 0; // Not applicable
//             break;
//         case 0x63:  // B-type (branch)
//             idex_reg.alu_op = 1; // ALU operation (for branch comparison)
//             idex_reg.alu_src = 0; // Use register values
//             idex_reg.mem_read = 0; // Not a memory read
//             idex_reg.mem_write = 0; // Not a memory write
//             idex_reg.reg_write = 0; // Not a register write
//             idex_reg.mem_to_reg = 0; // Not applicable
//             break;
//         case 0x37:  // U-type (LUI)
//             idex_reg.alu_op = 0; // ALU operation
//             idex_reg.alu_src = 1; // Use immediate value
//             idex_reg.mem_read = 0; // Not a memory read
//             idex_reg.mem_write = 0; // Not a memory write
//             idex_reg.reg_write = 1; // Write to register
//             idex_reg.mem_to_reg = 0; // ALU result to register
//             break;
//         case 0x17:  // U-type (AUIPC)
//             idex_reg.alu_op = 0; // ALU operation
//             idex_reg.alu_src = 1; // Use immediate value
//             idex_reg.mem_read = 0; // Not a memory read
//             idex_reg.mem_write = 0; // Not a memory write
//             idex_reg.reg_write = 1; // Write to register
//             idex_reg.mem_to_reg = 0; // ALU result to register
//             break;
//         case 0x6F:  // J-type (JAL)
//             idex_reg.alu_op = 0; // ALU operation
//             idex_reg.alu_src = 1; // Use immediate value
//             idex_reg.mem_read = 0; // Not a memory read
//             idex_reg.mem_write = 0; // Not a memory write
//             idex_reg.reg_write = 1; // Write to register
//             idex_reg.mem_to_reg = 0; // ALU result to register
//             break;
//         default:
//             break;
//     }

/*
//version2
 switch(instruction.opcode) {
    case 0x33:  // R-type
        idex_reg.alu_op = 2; // ALU operation
        idex_reg.alu_src = 0; // Use register values
        idex_reg.mem_read = 0; // Not a memory read
        idex_reg.mem_write = 0; // Not a memory write
        idex_reg.reg_write = 1; // Write to register
        idex_reg.mem_to_reg = 0; // ALU result to register
        break;
    case 0x03:  // I-type (load)
        idex_reg.alu_op = 0; // ALU operation (address calculation)
        idex_reg.alu_src = 1; // Use immediate value
        idex_reg.mem_read = 1; // Memory read
        idex_reg.mem_write = 0; // Not a memory write
        idex_reg.reg_write = 1; // Write to register
        idex_reg.mem_to_reg = 1; // Memory result to register
        break;
    case 0x13:  // I-type (ALU immediate)
        idex_reg.alu_op = 2; // ALU operation (depends on funct3)
        idex_reg.alu_src = 1; // Use immediate value
        idex_reg.mem_read = 0; // Not a memory read
        idex_reg.mem_write = 0; // Not a memory write
        idex_reg.reg_write = 1; // Write to register
        idex_reg.mem_to_reg = 0; // ALU result to register
        break;
    case 0x23:  // S-type (store)
        idex_reg.alu_op = 0; // ALU operation (address calculation)
        idex_reg.alu_src = 1; // Use immediate value
        idex_reg.mem_read = 0; // Not a memory read
        idex_reg.mem_write = 1; // Memory write
        idex_reg.reg_write = 0; // Not a register write
        idex_reg.mem_to_reg = 0; // Not applicable
        break;
    case 0x63:  // B-type (branch)
        idex_reg.alu_op = 1; // ALU operation (for branch comparison)
        idex_reg.alu_src = 0; // Use register values
        idex_reg.mem_read = 0; // Not a memory read
        idex_reg.mem_write = 0; // Not a memory write
        idex_reg.reg_write = 0; // Not a register write
        idex_reg.mem_to_reg = 0; // Not applicable
        break;
    case 0x37:  // U-type (LUI)
        idex_reg.alu_op = 3; // ALU operation (load upper immediate)
        idex_reg.alu_src = 1; // Use immediate value
        idex_reg.mem_read = 0; // Not a memory read
        idex_reg.mem_write = 0; // Not a memory write
        idex_reg.reg_write = 1; // Write to register
        idex_reg.mem_to_reg = 0; // ALU result to register
        break;
    case 0x17:  // U-type (AUIPC)
        idex_reg.alu_op = 4; // ALU operation (add upper immediate to PC)
        idex_reg.alu_src = 1; // Use immediate value
        idex_reg.mem_read = 0; // Not a memory read
        idex_reg.mem_write = 0; // Not a memory write
        idex_reg.reg_write = 1; // Write to register
        idex_reg.mem_to_reg = 0; // ALU result to register
        break;
    case 0x6F:  // J-type (JAL)
        idex_reg.alu_op = 5; // ALU operation (jump address calculation)
        idex_reg.alu_src = 1; // Use immediate value
        idex_reg.mem_read = 0; // Not a memory read
        idex_reg.mem_write = 0; // Not a memory write
        idex_reg.reg_write = 1; // Write to register
        idex_reg.mem_to_reg = 0; // ALU result to register
        break;
    default:
        // Handle undefined instructions if necessary
        break;
  }
  //2
  */

 switch(instruction.opcode) {
    case 0x33:  // R-type
        idex_reg.alu_op = 2;  // ALU operation (determined by funct3 and funct7)
        idex_reg.alu_src = 0; // Use register values
        idex_reg.pc_src = 0;  // No change to PC source
        idex_reg.branch = 0;  // Not a branch
        idex_reg.mem_read = 0; // Not a memory read
        idex_reg.mem_write = 0; // Not a memory write
        idex_reg.reg_write = 1; // Write to register
        idex_reg.mem_to_reg = 0; // ALU result to register
        break;
    case 0x03:  // I-type (load)
        idex_reg.alu_op = 0;  // ALU operation (address calculation)
        idex_reg.alu_src = 1; // Use immediate value
        idex_reg.pc_src = 0;  // No change to PC source
        idex_reg.branch = 0;  // Not a branch
        idex_reg.mem_read = 1; // Memory read
        idex_reg.mem_write = 0; // Not a memory write
        idex_reg.reg_write = 1; // Write to register
        idex_reg.mem_to_reg = 1; // Memory result to register
        break;
    case 0x13:  // I-type (ALU immediate)
        //idex_reg.alu_op = 2;  // ALU operation (determined by funct3) CHECK
        idex_reg.alu_op = 0;
        idex_reg.alu_src = 1; // Use immediate value
        idex_reg.pc_src = 0;  // No change to PC source
        idex_reg.branch = 0;  // Not a branch
        idex_reg.mem_read = 0; // Not a memory read
        idex_reg.mem_write = 0; // Not a memory write
        idex_reg.reg_write = 1; // Write to register
        idex_reg.mem_to_reg = 0; // ALU result to register
        break;
    case 0x23:  // S-type (store)
        idex_reg.alu_op = 0;  // ALU operation (address calculation)
        idex_reg.alu_src = 1; // Use immediate value
        idex_reg.pc_src = 0;  // No change to PC source
        idex_reg.branch = 0;  // Not a branch
        idex_reg.mem_read = 0; // Not a memory read
        idex_reg.mem_write = 1; // Memory write
        idex_reg.reg_write = 0; // Not a register write
        idex_reg.mem_to_reg = 0; // Not applicable
        break;
    case 0x63:  // B-type (branch)
        idex_reg.alu_op = 1;  // ALU operation (for branch comparison)
        idex_reg.alu_src = 0; // Use register values
        idex_reg.pc_src = 1;  // PC source changes based on branch outcome
        idex_reg.branch = 1;  // Branch
        idex_reg.mem_read = 0; // Not a memory read
        idex_reg.mem_write = 0; // Not a memory write
        idex_reg.reg_write = 0; // Not a register write
        idex_reg.mem_to_reg = 0; // Not applicable
        break;
    case 0x37:  // U-type (LUI)
        //idex_reg.alu_op = 3;  // ALU operation (load upper immediate)
        idex_reg.alu_op = 0;
        idex_reg.alu_src = 1; // Use immediate value
        idex_reg.pc_src = 0;  // No change to PC source
        idex_reg.branch = 0;  // Not a branch
        idex_reg.mem_read = 0; // Not a memory read
        idex_reg.mem_write = 0; // Not a memory write
        idex_reg.reg_write = 1; // Write to register
        idex_reg.mem_to_reg = 0; // ALU result to register
        break;
    case 0x17:  // U-type (AUIPC)
        //idex_reg.alu_op = 4;  // ALU operation (add upper immediate to PC)
        idex_reg.alu_op = 0;
        idex_reg.alu_src = 1; // Use immediate value
        idex_reg.pc_src = 0;  // No change to PC source
        idex_reg.branch = 0;  // Not a branch
        idex_reg.mem_read = 0; // Not a memory read
        idex_reg.mem_write = 0; // Not a memory write
        idex_reg.reg_write = 1; // Write to register
        idex_reg.mem_to_reg = 0; // ALU result to register
        break;
    case 0x6F:  // J-type (JAL)
        //idex_reg.alu_op = 5;  // ALU operation (jump address calculation)
        idex_reg.alu_op = 0;
        idex_reg.alu_src = 1; // Use immediate value
        idex_reg.pc_src = 1;  // PC source changes to jump address
        idex_reg.branch = 1;  // Unconditional jump (treated as a branch)
        idex_reg.mem_read = 0; // Not a memory read
        idex_reg.mem_write = 0; // Not a memory write
        idex_reg.reg_write = 1; // Write to register
        idex_reg.mem_to_reg = 0; // ALU result to register
        break;
    default:
        // Handle undefined instructions if necessary
        break;
}
  return idex_reg;
}

/// MEMORY STAGE HELPERS ///

/**
 * evaluates whether a branch must be taken
 * input  : <open to implementation>
 * output : bool
 **/
bool gen_branch(/*<args>*/)
{
  /**
   * YOUR CODE HERE
   */
  
   return false;

}


/// PIPELINE FEATURES ///

/**
 * Task   : Sets the pipeline wires for the forwarding unit's control signals
 *           based on the pipeline register values.
 * input  : pipeline_regs_t*, pipeline_wires_t*
 * output : None
*/
void gen_forward(pipeline_regs_t* pregs_p, pipeline_wires_t* pwires_p)
{
  /**
   * YOUR CODE HERE
   */
}

/**
 * Task   : Sets the pipeline wires for the hazard unit's control signals
 *           based on the pipeline register values.
 * input  : pipeline_regs_t*, pipeline_wires_t*
 * output : None
*/
void detect_hazard(pipeline_regs_t* pregs_p, pipeline_wires_t* pwires_p, regfile_t* regfile_p)
{
  /**
   * YOUR CODE HERE
   */
}


///////////////////////////////////////////////////////////////////////////////


/// RESERVED FOR PRINTING REGISTER TRACE AFTER EACH CLOCK CYCLE ///
void print_register_trace(regfile_t* regfile_p)
{
  // print
  for (uint8_t i = 0; i < 8; i++)       // 8 columns
  {
    for (uint8_t j = 0; j < 4; j++)     // of 4 registers each
    {
      printf("r%2d=%08x ", i * 4 + j, regfile_p->R[i * 4 + j]);
    }
    printf("\n");
  }
  printf("\n");
}

#endif // __STAGE_HELPERS_H__

