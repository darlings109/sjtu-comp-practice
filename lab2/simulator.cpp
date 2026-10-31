#include "simulator.h"
#include <iostream>
Simulator::Simulator() : pc(0) {
}

void Simulator::run(const int max_steps) {
    int step = 0;
    Status status = Status::AOK;
    for (step = 0; step < max_steps && status == Status::AOK; step++) {
        status = next_instruction();
    }
    report_stopped(step, status);
}

static uint8_t get_hi4(const uint8_t pack) { return (pack >> 4) & 0xF; }
static uint8_t get_lo4(const uint8_t pack) { return pack & 0xF; }

Status Simulator::next_instruction() {
    uint64_t next_pc = pc;

    // get code and function (1 byte)
    const std::optional<uint8_t> codefun = memory.get_byte(next_pc);
    if (!codefun) {
        report_bad_inst_addr();
        return Status::ADR;
    }
    const auto icode = static_cast<InstructionCode>(get_hi4(codefun.value()));
    uint8_t ifun = get_lo4(codefun.value());
    next_pc++;

    // execute the instruction
    switch (icode) {
        case InstructionCode::HALT: // 0:0
        {
            return Status::HLT;
        }
        case InstructionCode::NOP: // 1:0
        {
            pc = next_pc;
            return Status::AOK;
        }
        case InstructionCode::RRMOVQ: // 2:x regA:regB
        {
            const std::optional<uint8_t> regs = memory.get_byte(next_pc);
            if (!regs) {
                report_bad_inst_addr();
                return Status::ADR;
            }

            const uint8_t reg_a = get_hi4(regs.value());
            const uint8_t reg_b = get_lo4(regs.value());
            if (error_invalid_reg(reg_a)) return Status::INS;
            if (error_invalid_reg(reg_b)) return Status::INS;
            next_pc++;

            if (cc.satisfy(static_cast<Condition>(ifun))) {
                registers[reg_b] = registers[reg_a];
            }
            pc = next_pc;
            return Status::AOK;
        }
            //以下就是我需要进行的更改了
        case InstructionCode::IRMOVQ: // 3:0 F:regB imm
        {
            const std::optional<uint8_t> regs = memory.get_byte(next_pc);
            if (!regs) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            const uint8_t reg_a = get_hi4(regs.value());
            const uint8_t reg_b = get_lo4(regs.value());
            if (reg_a!=0xF)return Status::INS;
            if (error_invalid_reg(reg_b)) return Status::INS;
            next_pc++;
            const std::optional<uint64_t> V = memory.get_long(next_pc);
            if (!V) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            next_pc +=8 ;
            if (cc.satisfy(static_cast<Condition>(ifun))) {
                registers[reg_b] = *V;
            }
            pc=next_pc;
            return Status::AOK;
        }
        case InstructionCode::RMMOVQ: // 4:0 regA:regB imm
        {
            const std::optional<uint8_t> regs = memory.get_byte(next_pc);
            if (!regs) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            const uint8_t reg_a = get_hi4(regs.value());
            const uint8_t reg_b = get_lo4(regs.value());
            if (error_invalid_reg(reg_a)) return Status::INS;
            if (error_invalid_reg(reg_b)) return Status::INS;
            next_pc++;
            const std::optional<uint64_t> D = memory.get_long(next_pc);
            if (!D) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            next_pc +=8 ;
            if (cc.satisfy(static_cast<Condition>(ifun))) {
                bool finish=memory.set_long(registers[reg_b]+*D,registers[reg_a]);
                if (!finish) {
                    report_bad_data_addr(registers[reg_b]+*D);
                    return Status::ADR;
                }
            }
            pc = next_pc;
            return Status::AOK;
        }
        case InstructionCode::MRMOVQ: // 5:0 regA:regB imm
        {
            const std::optional<uint8_t> regs = memory.get_byte(next_pc);
            if (!regs) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            const uint8_t reg_a = get_hi4(regs.value());
            const uint8_t reg_b = get_lo4(regs.value());
            if (error_invalid_reg(reg_a)) return Status::INS;
            if (error_invalid_reg(reg_b)) return Status::INS;
            next_pc++;
            const std::optional<uint64_t> D = memory.get_long(next_pc);
            if (!D) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            next_pc +=8 ;
            if (cc.satisfy(static_cast<Condition>(ifun))) {
                uint64_t addr =registers[reg_b]+ *D;
                const std::optional<uint64_t> load = memory.get_long(addr);
                if (!load) {
                    report_bad_data_addr(addr);
                    return Status::ADR;
                }
                registers[reg_a] = *load;
            }
            pc = next_pc;
            return Status::AOK;
        }
        case InstructionCode::ALU: // 6:x regA:regB
        {
            const std::optional<uint8_t> regs = memory.get_byte(next_pc);
            if (!regs) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            const uint8_t reg_a = get_hi4(regs.value());
            const uint8_t reg_b = get_lo4(regs.value());
            if (error_invalid_reg(reg_a)) return Status::INS;
            if (error_invalid_reg(reg_b)) return Status::INS;
            next_pc++;
            uint64_t dest =registers[reg_a];
            uint64_t addr =registers[reg_b];
            uint64_t result =0;
            switch (ifun) {
                case 0x0: result=addr+dest; break;
                case 0x1: result=addr-dest; break;
                case 0x2: result=addr&dest; break;
                case 0x3: result=addr^dest; break;
                default: return Status::INS;
            }
            AluOp op = static_cast<AluOp>(ifun);
            cc=ConditionCodes::compute(op,addr,dest,result);
            registers[reg_b] = result;
            pc = next_pc;
            return Status::AOK;
        }
        case InstructionCode::JMP: // 7:x imm
        {
            const std::optional<uint64_t> Dest = memory.get_long(next_pc);
            if (!Dest) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            next_pc +=8 ;
            if (cc.satisfy(static_cast<Condition>(ifun))) {
                pc=Dest.value();
            }
            else pc=next_pc;
            return Status::AOK;
        }
        case InstructionCode::CALL: // 8:0 imm
        {
            const std::optional<uint64_t> Dest = memory.get_long(next_pc);
            if (!Dest) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            uint64_t reg_addr = next_pc +8 ;
            uint64_t rsp=registers[RegId::RSP];
            rsp-=8;
            bool ok=memory.set_long(rsp,reg_addr);
            if (!ok) {
                report_bad_stack_addr(rsp);
                return Status::ADR;
            }
            registers[RegId::RSP] = rsp;
            pc = Dest.value();
            return Status::AOK;
        }
        case InstructionCode::RET: // 9:0
        {
            uint64_t rsp = registers[RegId::RSP];
            const std::optional<uint64_t> Dest = memory.get_long(rsp);
            if (!Dest) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            rsp+=8;
            registers[RegId::RSP] = rsp;
            pc = Dest.value();
            return Status::AOK;
        }
        case InstructionCode::PUSHQ: // A:0 regA:F
        {
            if (ifun!= 0) return Status::INS;
            const std::optional<uint64_t> reg = memory.get_byte(next_pc);
            if (!reg) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            const uint8_t reg_a = get_hi4(reg.value());
            const uint8_t reg_b = get_lo4(reg.value());
            if (error_invalid_reg(reg_a)) return Status::INS;
            if (reg_b != 0xF ) return Status::INS;
            next_pc ++ ;
            uint64_t addr =registers[reg_a];
            uint64_t rsp = registers[RegId::RSP];
            rsp-= 8;
            bool ok = memory.set_long(rsp,addr);
            if (!ok) {
                report_bad_stack_addr(rsp);
                return Status::ADR;
            };
            registers[RegId::RSP] = rsp;
            pc = next_pc;
            return Status::AOK;
        }
        case InstructionCode::POPQ: // B:0 regA:F
        {
            if (ifun != 0 ) return Status::INS;
            const std::optional<uint64_t> reg = memory.get_byte(next_pc);
            if (!reg) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            uint8_t reg_a = get_hi4(reg.value());
            uint8_t reg_b = get_lo4(reg.value());
            if (error_invalid_reg(reg_a)) return Status::INS;
            if (reg_b != 0xF ) return Status::INS;
            next_pc ++ ;

            uint64_t rsp = registers[RegId::RSP];
            const std::optional<uint64_t> Dest = memory.get_long(rsp);
            if (!Dest) {
                report_bad_inst_addr();
                return Status::ADR;
            }

            rsp += 8 ;
            registers[RegId::RSP] = rsp;
            registers[reg_a] = Dest.value();
            pc = next_pc;
            return Status::AOK;
        }
        default:
            report_bad_inst(codefun.value());
            return Status::INS;
    }
}
