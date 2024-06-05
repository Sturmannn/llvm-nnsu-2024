#include <algorithm>

#include "X86.h"
#include "X86InstrInfo.h"
#include "X86Subtarget.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/Register.h"

using namespace llvm;

namespace {
class X86SafronovMulAddPass : public MachineFunctionPass {
public:
  static char ID;
  X86SafronovMulAddPass() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override {
    const X86Subtarget &STI = MF.getSubtarget<X86Subtarget>();
    const X86InstrInfo &TII = *STI.getInstrInfo();

    bool Changed = false;
    std::vector<MachineInstr *> InstructionsToRemove;

    for (MachineBasicBlock &MBB : MF) {
      for (auto I = MBB.begin(); I != MBB.end(); ++I) {
        if (I->getOpcode() != X86::MULPDrr) {
          continue;
        }
        auto MulInstr = &(*I);
        for (auto J = std::next(I); J != MBB.end(); ++J) {
          if (J->getOpcode() != X86::ADDPDrr) {
            continue;
          }
          if (MulInstr->getOperand(0).getReg() != J->getOperand(1).getReg() &&
              MulInstr->getOperand(0).getReg() != J->getOperand(2).getReg()) {
            continue;
          }
          auto AddInstr = &(*J);
          if (findInstruction(InstructionsToRemove, AddInstr)) {
            continue;
          }
          bool Used = false;
          for (auto K = std::next(I); K != J; ++K) {
            auto Instr = &(*K);
            for (auto &Reg : Instr->all_uses()) {
              if (Reg.getReg() != AddInstr->getOperand(1).getReg() &&
                  Reg.getReg() != AddInstr->getOperand(2).getReg()) {
                continue;
              }
              if (findInstruction(InstructionsToRemove, Instr)) {
                continue;
              }
              Used = true;
              break;
            }
            if (Used)
              break;
          }
          if (Used)
            continue;
          MIMetadata Metadata(*I);
          MachineInstrBuilder MIB =
              BuildMI(MBB, MulInstr, Metadata, TII.get(X86::VFMADD213PDr));
          MIB.addReg(AddInstr->getOperand(0).getReg(), RegState::Define);
          MIB.addReg(MulInstr->getOperand(1).getReg());
          MIB.addReg(MulInstr->getOperand(2).getReg());
          if (MulInstr->getOperand(0).getReg() == J->getOperand(1).getReg()) {
            MIB.addReg(AddInstr->getOperand(2).getReg());
          } else if (MulInstr->getOperand(0).getReg() == J->getOperand(2).getReg()) {
            MIB.addReg(AddInstr->getOperand(1).getReg());
          }
          if (!findInstruction(InstructionsToRemove, MulInstr)) {
            InstructionsToRemove.push_back(MulInstr);
          }
          if (!findInstruction(InstructionsToRemove, AddInstr)) {
            InstructionsToRemove.push_back(AddInstr);
          }
          if (MulInstr->getOperand(0).getReg() == AddInstr->getOperand(0).getReg()) {
            break;
          }
          Changed = true;
        }
      }
    }
    for (auto &MI : InstructionsToRemove) {
      MI->eraseFromParent();
    }
    return Changed;
  }

private:
  bool findInstruction(std::vector<MachineInstr *> &InstructionsToRemove,
                       MachineInstr *Instruction) {
    auto Result{std::find(begin(InstructionsToRemove), end(InstructionsToRemove), Instruction)};
    return (Result != end(InstructionsToRemove));
  }
};
} // end anonymous namespace

char X86SafronovMulAddPass::ID = 0;

static RegisterPass<X86SafronovMulAddPass>
    X("x86-my-custom-pass", "X86 my custom pass", false, false);
