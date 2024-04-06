#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Cloning.h"

using namespace llvm;

namespace {
Instruction *getNextNonTerminator(BasicBlock *BB, Instruction *I) {
  for (auto &Inst : *BB) {
    if (&Inst == I) {
      for (auto &NextInst :
           llvm::make_range(std::next(I->getIterator()), BB->end())) {
        if (!NextInst.isTerminator()) {
          return &NextInst;
        }
      }
    }
  }
  return nullptr;
}

struct CustomInliningPass : public PassInfoMixin<CustomInliningPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {

    // std::vector<CallInst *> callsToInline;
    // for (Function &Callee : F.getParent()->functions()) {
    //   for (auto &I : BB) {
    //     if (Callee.arg_empty() && Callee.getReturnType()->isVoidTy()) {
    //       inlineEmptyFunction(F, &Callee);
    //     }
    //   }
    // }

    Function *Caller{};
    Function *Callee{};
    std::vector<CallInst *> callsToInline;
    for (auto &BB : F) {
      for (auto &I : BB) {
        if (auto *CI = dyn_cast<CallInst>(&I)) {
          Callee = CI->getCalledFunction();
          if (Callee && Callee->arg_size() == 0 &&
              Callee->getReturnType()->isVoidTy()) {
            callsToInline.push_back(CI);
          }
        }
      }
    }

    // Process each call site
    for (CallInst *CI : callsToInline) {
      BasicBlock *InsertBB = CI->getParent();
      // Instruction *InsertPt = CI->getNextNonTerminator();
      Instruction *InsertPt = getNextNonTerminator(InsertBB, CI);

      // Create a map for block correspondence
      DenseMap<BasicBlock *, BasicBlock *> BlockMap;

      // Split the calling function at the call site (use ".inlined" for
      // clarity)
      BasicBlock *SplitBB = InsertBB->splitBasicBlock(InsertPt, ".inlined");

      // Create new blocks corresponding to empty function's blocks
      for (BasicBlock &CalleeBB : *Callee) {
        BasicBlock *NewBB = BasicBlock::Create(
            F.getContext(), CalleeBB.getName() + ".inlined", &F);
        BlockMap[&CalleeBB] = NewBB;
        InsertBB->getTerminator()->setSuccessor(0, NewBB);
      }

      // Copy instructions from corresponding blocks
      for (BasicBlock &CalleeBB : *Callee) {
        BasicBlock *NewBB = BlockMap[&CalleeBB];
        for (Instruction &Inst : CalleeBB) {
          // Clone instruction and insert before terminator in new block
          // Instruction *NewInst =
          // Inst.clone()->insertBefore(NewBB->getTerminator());
          Inst.clone()->insertBefore(NewBB->getTerminator());
          // Rename variables to avoid conflicts (consider using ValueMap)
          // Update uses of variables (if necessary)
        }
      }

      // Update branch instructions to target correct blocks
      for (Instruction &Branch : *SplitBB) {
        // auto *Branch = dyn_cast<BranchInst>(&(BB.getTerminator()))
        if (Branch.isTerminator()) {
          for (unsigned int i = 0, e = Branch.getNumSuccessors(); i != e; ++i) {
            BasicBlock *Successor = Branch.getSuccessor(i);
            Branch.setSuccessor(i, BlockMap[Successor]);
          }
        }
      }

      // Создание IRBuilder и установка его контекста
      IRBuilder<> Builder(F.getContext());

      // Обход базовых блоков в Caller
      for (BasicBlock &BB : F) {
        // Обход инструкций в текущем базовом блоке
        for (Instruction &I : BB) {
          // Проверка, является ли текущая инструкция возвратом (ReturnInst)
          if (auto *RI = dyn_cast<ReturnInst>(&I)) {
            // Удаление инструкции возврата из родительского базового блока
            RI->eraseFromParent();

            // Создание инструкции безусловного перехода с использованием
            // IRBuilder
            Builder.SetInsertPoint(
                &BB); // Устанавливаем точку вставки в текущий базовый блок
            Builder.CreateBr(SplitBB); // Создаем безусловный переход к SplitBB
          }
        }
      }

      // Remove the call instruction
      CI->eraseFromParent();


      return PreservedAnalyses::all();
    }
  }
  static bool isRequired() { return true; }
};

} // end anonymous namespace

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "CustomInliningPass", "v0.1",
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "custom-inlining") {
                    FPM.addPass(CustomInliningPass());
                    return true;
                  }
                  return false;
                });
          }};
}
