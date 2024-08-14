#include "llvm/ADT/ArrayRef.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/Compiler.h"

namespace {

#define LoopStartFuncIR "_Z10loop_startv"
#define LoopEndFuncIR "_Z8loop_endv"

struct LysanovaLoopWrapper : public llvm::PassInfoMixin<LysanovaLoopWrapper> {
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &FAM) {
    llvm::LoopInfo &LI = FAM.getResult<llvm::LoopAnalysis>(F);

    llvm::FunctionCallee LoopStartFunc =
        getOrInsertLoopFunc(F.getParent(), LoopStartFuncIR);
    llvm::FunctionCallee LoopEndFunc =
        getOrInsertLoopFunc(F.getParent(), LoopEndFuncIR);

    bool ModifiedIR = false;

    for (llvm::Loop *loop : LI) {
      if (llvm::BasicBlock *preheader = loop->getLoopPreheader()) {
        if (!isLoopFuncCalled(*preheader, LoopStartFuncIR)) {
          llvm::IRBuilder<> builderStart(preheader->getTerminator());
          builderStart.CreateCall(LoopStartFunc);
          ModifiedIR = true;
        }
      }
      llvm::SmallVector<llvm::BasicBlock *, 5> successors;
      loop->getExitBlocks(successors);

      for (llvm::BasicBlock *BB : successors) {
        if (!isLoopFuncCalled(*BB, LoopEndFuncIR)) {
          llvm::IRBuilder<> builderEnd(&*BB->getFirstInsertionPt());
          builderEnd.CreateCall(LoopEndFunc);
          ModifiedIR = true;
        }
      }
    }

    return ModifiedIR ? llvm::PreservedAnalyses::none()
                      : llvm::PreservedAnalyses::all();
  }
  static bool isRequired() { return true; }

private:
  llvm::FunctionCallee getOrInsertLoopFunc(llvm::Module *M,
                                           const char *FuncName) {
    llvm::LLVMContext &Context = M->getContext();
    return M->getOrInsertFunction(
        FuncName,
        llvm::FunctionType::get(llvm::Type::getVoidTy(Context), false));
  }

  bool isLoopFuncCalled(const llvm::BasicBlock &BB, const char *FuncName) {
    unsigned int InstOpcode = 0;
    for (const llvm::Instruction &I : BB) {
      InstOpcode = I.getOpcode();
      if (InstOpcode == llvm::Instruction::Call) {
        const llvm::CallInst &CI = llvm::cast<llvm::CallInst>(I);
        if (!CI.getCalledFunction()->getName().str().compare(FuncName))
          return true;
      }
    }
    return false;
  }
};
} // namespace

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "LoopWrapperPass", "v0.1",
          [](llvm::PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](llvm::StringRef Name, llvm::FunctionPassManager &FPM,
                   llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) {
                  if (Name == "loop-wrapper-pass") {
                    FPM.addPass(LysanovaLoopWrapper());
                    return true;
                  }
                  return false;
                });
          }};
}