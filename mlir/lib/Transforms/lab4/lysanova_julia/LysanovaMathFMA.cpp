#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Tools/Plugins/PassPlugin.h"

using namespace mlir;

namespace {

#define PASS_NAME "lysanova-fma"

class LysanovaMathFMA
    : public PassWrapper<LysanovaMathFMA, OperationPass<LLVM::LLVMFuncOp>> {
  StringRef getArgument() const final { return "lysanova-fma"; }
  StringRef getDescription() const final {
    return "Converts addition and multiplication operations to FMA in the "
           "LLVM's dialect";
  }

  void runOnOperation() override {
    auto funcOp = getOperation();

    // Iterate over each operation inside the function.
    funcOp.walk([&](LLVM::FAddOp addOp) {
      // Find the preceding multiplication operation.
      LLVM::FMulOp mulOp = addOp.getOperand(0).getDefiningOp<LLVM::FMulOp>();
      Value otherOperand = addOp.getOperand(1);

      if (!mulOp) {
        mulOp = addOp.getOperand(1).getDefiningOp<LLVM::FMulOp>();
        otherOperand = addOp.getOperand(0);
        if (!mulOp)
          return;
      }

      // Check if the multiplication operand has other users.
      if (!mulOp.getResult().hasOneUse()) {
        return;
      }

      // Replace addition with LLVM FMA.
      OpBuilder builder(addOp);
      LLVM::FMAOp fmaOp =
          builder.create<LLVM::FMAOp>(addOp.getLoc(), mulOp.getOperand(0),
                                      mulOp.getOperand(1), otherOperand);

      // Replace all uses of addOp with the result of fmaOp.
      addOp.replaceAllUsesWith(fmaOp.getResult());

      // Erase the original operations.
      addOp.erase();
      mulOp.erase();
    });
  }
};

} // namespace

MLIR_DECLARE_EXPLICIT_TYPE_ID(LysanovaMathFMA)
MLIR_DEFINE_EXPLICIT_TYPE_ID(LysanovaMathFMA)

PassPluginLibraryInfo getFMAPassPluginInfo() {
  return {MLIR_PLUGIN_API_VERSION, PASS_NAME, LLVM_VERSION_STRING,
          []() { PassRegistration<LysanovaMathFMA>(); }};
}

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo mlirGetPassPluginInfo() {
  return getFMAPassPluginInfo();
}