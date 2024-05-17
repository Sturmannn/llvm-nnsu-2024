#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

namespace {
struct FunctionInstrumentation : llvm::PassInfoMixin<FunctionInstrumentation> {
  llvm::PreservedAnalyses run(llvm::Function &func,
                              llvm::FunctionAnalysisManager &) {
    llvm::LLVMContext &ctx = func.getContext();
    llvm::IRBuilder<> builder(ctx);
    llvm::Module *mod = func.getParent();
    bool hasStart = false;
    bool hasEnd = false;

    llvm::FunctionType *funcType =
        llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), false);
    llvm::FunctionCallee startInstr =
        (*mod).getOrInsertFunction("start_instrument", funcType);
    llvm::FunctionCallee endInstr =
        (*mod).getOrInsertFunction("end_instrument", funcType);

    for (auto &blk : func) {
      for (auto &instr : blk) {
        if (llvm::isa<llvm::CallInst>(&instr)) {
          llvm::CallInst *callInstruction = llvm::cast<llvm::CallInst>(&instr);
          if (callInstruction->getCalledFunction() == startInstr.getCallee()) {
            hasStart = true;
          } else if (callInstruction->getCalledFunction() ==
                     endInstr.getCallee()) {
            hasEnd = true;
          }
        }
      }
    }

    if (!hasStart) {
      builder.SetInsertPoint(&func.getEntryBlock().front());
      builder.CreateCall(startInstr);
    }
    if (!hasEnd) {
      for (llvm::BasicBlock &BB : func) {
        if (llvm::dyn_cast<llvm::ReturnInst>(BB.getTerminator())) {
          builder.SetInsertPoint(BB.getTerminator());
          builder.CreateCall(endInstr);
        }
      }
    }

    return llvm::PreservedAnalyses::all();
  }

  static bool isRequired() { return true; }
};
} // namespace

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "instrumentation_func_wrapper", "0.1",
          [](llvm::PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](llvm::StringRef name, llvm::FunctionPassManager &FPM,
                   llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) -> bool {
                  if (name == "instrumentation_func_wrapper") {
                    FPM.addPass(FunctionInstrumentation{});
                    return true;
                  }
                  return false;
                });
          }};
}
