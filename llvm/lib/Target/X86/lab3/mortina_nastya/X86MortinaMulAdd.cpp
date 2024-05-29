#include "X86.h"
#include "X86InstrInfo.h"
#include "X86Subtarget.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicsX86.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
class MortinaMulAddPass : public MachineFunctionPass {
public:
  static char ID;
  MortinaMulAddPass() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override {
    SmallVector<MachineInstr *> deletedInstrPtr;
    bool Changed = false;

    for (auto &MBB : MF) {
      for (auto MI = MBB.begin(); MI != MBB.end(); ++MI) {
        if (MI->getOpcode() == X86::MULPDrr) {
          Changed = processInstruction(MF, MBB, MI, deletedInstrPtr) || Changed;
        }
      }
    }

    for (auto it : deletedInstrPtr)
      it->eraseFromParent();

    return Changed;
  }

private:
  bool processInstruction(MachineFunction &MF, MachineBasicBlock &MBB,
                          MachineBasicBlock::iterator MI,
                          SmallVector<MachineInstr *> &deletedInstrPtr) {
    Register Reg = MI->getOperand(0).getReg();

    for (auto NextMI = std::next(MI); NextMI != MBB.end(); ++NextMI) {
      if (NextMI->getOpcode() == X86::ADDPDrr && isRegInOperands(NextMI, Reg)) {
        if (!hasDependency(NextMI, Reg, MBB)) {
          buildNewInstruction(MF, MBB, MI, NextMI, Reg);
          deletedInstrPtr.push_back(&*MI);
          deletedInstrPtr.push_back(&*NextMI);
          return true;
        }
        break;
      } else if (isRegInOperands(NextMI, Reg)) {
        break;
      }
    }

    return false;
  }

  bool isRegInOperands(MachineBasicBlock::iterator MI, Register Reg) {
    for (unsigned i = 0, e = MI->getNumOperands(); i != e; ++i) {
      if (MI->getOperand(i).getReg() == Reg) {
        return true;
      }
    }
    return false;
  }

  bool hasDependency(MachineBasicBlock::iterator NextMI, Register Reg,
                     MachineBasicBlock &MBB) {
    if (NextMI->getOperand(0).getReg() != Reg) {
      for (auto CheckMI = std::next(NextMI); CheckMI != MBB.end(); ++CheckMI) {
        if (isRegInOperands(CheckMI, Reg)) {
          return true;
        }
      }
    }
    return false;
  }

  void buildNewInstruction(MachineFunction &MF, MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator MI,
                           MachineBasicBlock::iterator NextMI, Register Reg) {
    MachineInstrBuilder BuilderMI =
        BuildMI(MBB, MI, MI->getDebugLoc(),
                MF.getSubtarget().getInstrInfo()->get(X86::VFMADD213PDr));
    BuilderMI.addReg(NextMI->getOperand(0).getReg(), RegState::Define);
    BuilderMI.addReg(MI->getOperand(1).getReg());
    BuilderMI.addReg(MI->getOperand(2).getReg());
    BuilderMI.addReg(NextMI->getOperand(2).getReg());
  }
};

char MortinaMulAddPass::ID = 0;
static RegisterPass<MortinaMulAddPass> X("x86-mortina-muladd-pass",
                                         "x86 Mortina Intrinsics Pass");
} // namespace