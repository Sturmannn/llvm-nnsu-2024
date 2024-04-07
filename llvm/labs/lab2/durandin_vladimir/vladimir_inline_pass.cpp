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
#include <string>

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

    Function *Callee{};
    std::vector<CallInst *> callsToInline{};
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
    size_t counter_splited = 0;
    size_t counter_inlined = 0;
    for (CallInst *CI : callsToInline) {
      BasicBlock *InsertBB = CI->getParent();
      // Instruction *InsertPt = CI->getNextNonTerminator();
      Instruction *InsertPt = getNextNonTerminator(InsertBB, CI);

      // Create a map for block correspondence
      DenseMap<BasicBlock *, BasicBlock *> BlockMap;

      // Split the calling function at the call site (use ".inlined" for
      // clarity)

      BasicBlock *SplitBB = InsertBB->splitBasicBlock(
          InsertPt, InsertBB->getName() + ".splited." +
                        std::to_string(counter_splited++));

      // Create new blocks corresponding to empty function's blocks
      // size_t counter = 0;
      BasicBlock *CalleeEntryBasicBlockPointer = nullptr;
      for (BasicBlock &CalleeBB : *Callee) {
        BasicBlock *NewBB = BasicBlock::Create(
            F.getContext(),
            CalleeBB.getName() + ".inlined." + std::to_string(counter_inlined),
            &F);
        if (Callee->getEntryBlock().getName() == CalleeBB.getName())
          CalleeEntryBasicBlockPointer = NewBB;
        BlockMap[&CalleeBB] = NewBB;
      }
      InsertBB->getTerminator()->setSuccessor(0, CalleeEntryBasicBlockPointer);

      // for (BasicBlock &Caller : F) {
      //   // Instruction *terminator = Caller.getTerminator();
      //   if (dyn_cast<ReturnInst>(Caller.getTerminator()) && dyn_cast<ReturnInst>(Caller.getTerminator())->getNumOperands() == 0) {
      //     Caller.getTerminator()->setSuccessor(0, SplitBB);
      //   } else {
      //     for (BasicBlock &BB : *Callee) {
      //       if (BB.getName() == Caller.getTerminator()->getName().str() + ".inlined." + std::to_string(counter_inlined)) {
      //         Caller.getTerminator()->setSuccessor(0, &BB);
      //       }
      //     }
      //   }
      // }
      counter_inlined++;

      // Copy instructions from corresponding blocks
      ValueToValueMapTy VMap;
      for (BasicBlock &CalleeBB : *Callee) {
        BasicBlock *NewBB = BlockMap[&CalleeBB];
        for (Instruction &Inst : CalleeBB) {
          IRBuilder<> Builder(
              NewBB); // Создание IRBuilder с указанием базового блока
          Instruction *NewInst = Inst.clone(); // Клонируем инструкцию
          // outs() << "instr = " << *NewInst << "\n\n";
          Builder.Insert(
              NewInst /*, Inst.getName()*/); // Добавляем клонированную
                                             // инструкцию в конец базового
                                             // блока
          VMap[&Inst] = NewInst;
        }
      }
      // Update branch instructions to target correct blocks
      for (Instruction &Branch : *SplitBB) {
        if (auto *BI = dyn_cast<BranchInst>(&Branch)) {
          for (unsigned int i = 0, e = BI->getNumSuccessors(); i != e; ++i) {
            BasicBlock *Successor = BI->getSuccessor(i);
            BI->setSuccessor(i, BlockMap[Successor]);
          }
        }
      }
      outs() << "after: \n" << F << "\n\n";
      for (BasicBlock &CalleeBB : *Callee) {
        BasicBlock *NewBB = BlockMap[&CalleeBB];
        for (Instruction &Inst : *NewBB) {
          RemapInstruction(&Inst, VMap, RF_None, nullptr, nullptr);
        }
      }

      // Создание IRBuilder и установка его контекста
      IRBuilder<> Builder(F.getContext());

      // Обход базовых блоков в Caller
      for (BasicBlock &BB : F) {
        // Создаем копию списка инструкций для текущего базового блока
        std::vector<Instruction *> Instructions;
        for (Instruction &I : BB) {
          Instructions.push_back(&I);
        }

        // Обход инструкций в текущем базовом блоке через копию списка
        for (Instruction *I : Instructions) {
          // Проверка, является ли текущая инструкция возвратом (ReturnInst)
          if (auto *RI = dyn_cast<ReturnInst>(I)) {
            if (RI->getParent() != SplitBB) {
              // Удаление инструкции возврата из родительского базового блока
              RI->eraseFromParent();

              // Создание инструкции безусловного перехода с использованием
              // IRBuilder
              Builder.SetInsertPoint(
                  &BB); // Устанавливаем точку вставки в текущий базовый блок
              Builder.CreateBr(
                  SplitBB); // Создаем безусловный переход к SplitBB
            }
          }
        }
      }

      // Remove the call instruction
      CI->eraseFromParent();
    }
    return PreservedAnalyses::all();
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
