#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Tools/Plugins/PassPlugin.h"

using namespace mlir;

class CustomDivPass
    : public PassWrapper<CustomDivPass, OperationPass<LLVM::LLVMFuncOp>> {
public:
  StringRef getArgument() const final { return "safronov_custom_ceildiv"; }
  StringRef getDescription() const final {
    return "transforms arith.ceildivui and arith.ceildivsi operations "
           "into a sequence of basic arithmetic operations";
  }

  void runOnOperation() override {
    getOperation()->walk([&](arith::CeilDivUIOp op) { replaceCeilDiv(op); });

    getOperation()->walk([&](arith::CeilDivSIOp op) { replaceCeilDiv(op); });
  }

private:
  void replaceCeilDiv(arith::CeilDivUIOp op) {
    OpBuilder builder(op.getContext());
    builder.setInsertionPoint(op);

    Value a = op.getLhs();
    Value b = op.getRhs();
    Type aType = a.getType();
    arith::ConstantOp one = builder.create<arith::ConstantOp>(
        op.getLoc(), aType, builder.getIntegerAttr(aType, 1));
    arith::AddIOp sum = builder.create<arith::AddIOp>(op.getLoc(), a, b);
    arith::SubIOp sumMinusOne =
        builder.create<arith::SubIOp>(op.getLoc(), sum, one);
    arith::DivUIOp div =
        builder.create<arith::DivUIOp>(op.getLoc(), sumMinusOne, b);

    op.replaceAllUsesWith(div.getOperation());
    op.erase();
  }

  void replaceCeilDiv(arith::CeilDivSIOp op) {
    OpBuilder builder(op.getContext());
    builder.setInsertionPoint(op);

    Value a = op.getLhs();
    Value b = op.getRhs();
    Type aType = a.getType();
    arith::ConstantOp one = builder.create<arith::ConstantOp>(
        op.getLoc(), aType, builder.getIntegerAttr(aType, 1));
    arith::AddIOp sum = builder.create<arith::AddIOp>(op.getLoc(), a, b);
    arith::SubIOp sumMinusOne =
        builder.create<arith::SubIOp>(op.getLoc(), sum, one);
    arith::DivSIOp div =
        builder.create<arith::DivSIOp>(op.getLoc(), sumMinusOne, b);

    op.replaceAllUsesWith(div.getOperation());
    op.erase();
  }
};

MLIR_DECLARE_EXPLICIT_TYPE_ID(CustomDivPass)
MLIR_DEFINE_EXPLICIT_TYPE_ID(CustomDivPass)

PassPluginLibraryInfo CustomDivPassPluginInfo() {
  return {MLIR_PLUGIN_API_VERSION, "safronov_custom_ceildiv",
          LLVM_VERSION_STRING, []() { PassRegistration<CustomDivPass>(); }};
}

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo mlirGetPassPluginInfo() {
  return CustomDivPassPluginInfo();
}
