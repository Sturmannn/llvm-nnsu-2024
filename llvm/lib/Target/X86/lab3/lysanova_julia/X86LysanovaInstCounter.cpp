#include "X86Subtarget.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/CodeGen/GlobalISel/Utils.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/Register.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/SymbolTableListTraits.h"
#include "llvm/IR/Type.h"
#include "llvm/Pass.h"
#include <cstdint>

using namespace llvm;

namespace {

#define GLOBAL_VAR "ic"
#define PASS_NAME "x86-lysanova-mir-counter"
#define PASS_DESC "Pass, counting the number of machine instructions executed"

class X86LysanovaMiCounter : public MachineFunctionPass {
public:
  static char ID;
  X86LysanovaMiCounter() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

private:
  GlobalVariable *getOrInsertGlobalVariable(Module *M, const char *Name);
  StringRef getPassName() const override { return PASS_DESC; }
};

char X86LysanovaMiCounter::ID = 0;

bool X86LysanovaMiCounter::runOnMachineFunction(MachineFunction &MF) {

  MachineRegisterInfo &MRI = MF.getRegInfo();
  const TargetRegisterInfo *TRI = MRI.getTargetRegisterInfo();
  const TargetRegisterClass *TRC =
      TRI->getMinimalPhysRegClass(X86::RAX, MVT::i64);
  Register IcReg = MRI.createVirtualRegister(TRC);

  const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
  const DebugLoc &DL = MF.front().begin()->getDebugLoc();

  uint32_t Counter{};

  // Load the address of the global variable Ic into the register
  GlobalVariable *Ic =
      getOrInsertGlobalVariable(MF.getFunction().getParent(), GLOBAL_VAR);
  Register IcAddrReg = MRI.createVirtualRegister(TRC);

  for (MachineBasicBlock &BB : MF) {
    Counter = BB.size();
    BuildMI(BB, BB.getFirstTerminator(), DL, TII->get(X86::ADD64ri32), IcReg)
        .addReg(IcReg)
        .addImm(Counter);
    if (BB.getFirstTerminator() != BB.end()) {
      MachineInstr &TerminatorInst = *BB.getFirstTerminator();
      if (TerminatorInst.isReturn()) {
        BuildMI(BB, TerminatorInst, DL, TII->get(X86::MOV64mr))
            .addReg(IcAddrReg)  // base register
            .addImm(1)          // scale
            .addReg(Register()) // index register
            .addImm(0)          // displacement
            .addReg(
                Register()) // segment register (usually 0 for default segment)
            .addReg(IcReg); // SrcReg (value to move)
      }
    }
  }
  // Load the address of the global variable Ic into the register
  BuildMI(MF.front(), MF.front().begin(), DL, TII->get(X86::MOV64ri), IcAddrReg)
      .addGlobalAddress(Ic);

  // Initialize IcReg with zero
  BuildMI(MF.front(), MF.front().begin(), DL, TII->get(X86::MOV64ri), IcReg)
      .addImm(0);

  return true;
}

GlobalVariable *
X86LysanovaMiCounter::getOrInsertGlobalVariable(Module *M, const char *Name) {
  GlobalVariable *Ic = M->getGlobalVariable(Name);
  if (!Ic) {
    Ic = new GlobalVariable(llvm::Type::getInt64Ty(M->getContext()), false,
                            llvm::GlobalValue::LinkageTypes::ExternalLinkage, 0,
                            Name);
    M->insertGlobalVariable(Ic);
    return Ic;
  }
  return Ic;
}

} // namespace

static RegisterPass<X86LysanovaMiCounter> X(PASS_NAME, PASS_DESC);