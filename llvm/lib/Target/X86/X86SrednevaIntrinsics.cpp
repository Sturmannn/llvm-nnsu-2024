#include "X86.h"
#include "X86InstrInfo.h"
#include "X86Subtarget.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/CodeGen/TargetOpcodes.h"

#define PASS_NAME "x86-sredneva-muladd"
#define PASS_DESC "muladd optimization pass";

using namespace llvm;

namespace {
struct X86SrednevaIntrinsics : public MachineFunctionPass {
public:
  static char ID;
  X86SrednevaIntrinsics() : MachineFunctionPass(ID) {}

 bool runOnMachineFunction(MachineFunction &MF) override {
    const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
    for (auto &MBB : MF) {
      for (auto MI = MBB.instr_begin(), E = MBB.instr_end(); MI != E;) {
        MachineInstr &Inst = *MI++;
        if (Inst.isBinaryOp() && Inst.getNumOperands() >= 3) {
          MachineInstr *MulInst = &Inst;
          MachineInstr *AddInst = nullptr;
          for (auto NextMI = std::next(MachineBasicBlock::iterator(MulInst));
               NextMI != E; ++NextMI) {
            if (NextMI->isBinaryOp() && NextMI->getNumOperands() >= 3 &&
                NextMI->getOperand(1).isReg() &&
                NextMI->getOperand(1).getReg() == MulInst->getOperand(0).getReg()) {
              AddInst = &*NextMI;
              break;
            }
          }
          if (AddInst) {
            BuildMI(MBB, AddInst, AddInst->getDebugLoc(), TII->get(TargetOpcode::VFMADD213PD))
                .add(AddInst->getOperand(0))
                .add(MulInst->getOperand(1))
                .add(MulInst->getOperand(2))
                .add(AddInst->getOperand(2));
            MulInst->eraseFromParent();
            AddInst->eraseFromParent();
          }
        }
      }
    }
    return true;
  }
};
} // end anonymous namespace

FunctionPass *llvm::createX86SrednevaMulAddIntrinPass() {
  return new X86SrednevaIntrinsics();
}

INITIALIZE_PASS(X86SrednevaIntrinsics, PASS_NAME, PASS_DESC, false, false)

char X86SrednevaIntrinsics::ID = 0;