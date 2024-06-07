// RUN: mlir-opt -load-pass-plugin=%mlir_lib_dir/SafronovCustomDivPass%shlibext --pass-pipeline="builtin.module(llvm.func(safronov_custom_ceildiv))" %s | FileCheck %s


module {
  llvm.func @ceilDivTest1(%arg0: i32, %arg1: i32) -> i32 attributes {llvm.linkage = #llvm.linkage<external>} {
    %0 = arith.ceildivui %arg0, %arg1 : i32
    llvm.return %0 : i32
  }

  // CHECK: llvm.func @ceilDivTest1(%arg0: i32, %arg1: i32) -> i32 attributes {llvm.linkage = #llvm.linkage<external>} {
  // CHECK-NEXT:   %c1_i32 = arith.constant 1 : i32
  // CHECK-NEXT:   %0 = arith.addi %arg0, %arg1 : i32
  // CHECK-NEXT:   %1 = arith.subi %0, %c1_i32 : i32
  // CHECK-NEXT:   %2 = arith.divui %1, %arg1 : i32
  // CHECK-NEXT:   llvm.return %2 : i32
  // CHECK-NEXT: }

  // =================================================================================================

  llvm.func @ceilDivTest2(%arg0: i16, %arg1: i16) -> i16 attributes {llvm.linkage = #llvm.linkage<external>} {
    %0 = arith.ceildivsi %arg0, %arg1 : i16
    llvm.return %0 : i16
  }

  // CHECK: llvm.func @ceilDivTest2(%arg0: i16, %arg1: i16) -> i16 attributes {llvm.linkage = #llvm.linkage<external>} {
  // CHECK-NEXT:   %c1_i16 = arith.constant 1 : i16
  // CHECK-NEXT:   %0 = arith.addi %arg0, %arg1 : i16
  // CHECK-NEXT:   %1 = arith.subi %0, %c1_i16 : i16
  // CHECK-NEXT:   %2 = arith.divsi %1, %arg1 : i16
  // CHECK-NEXT:   llvm.return %2 : i16
  // CHECK-NEXT: }

  // =================================================================================================


  llvm.func @ceilDivTest3(%arg0: i64, %arg1: i64) -> i64 attributes {llvm.linkage = #llvm.linkage<external>} {
    %0 = arith.ceildivui %arg0, %arg1 : i64
    llvm.return %0 : i64
  }

  // CHECK: llvm.func @ceilDivTest3(%arg0: i64, %arg1: i64) -> i64 attributes {llvm.linkage = #llvm.linkage<external>} {
  // CHECK-NEXT:   %c1_i64 = arith.constant 1 : i64
  // CHECK-NEXT:   %0 = arith.addi %arg0, %arg1 : i64
  // CHECK-NEXT:   %1 = arith.subi %0, %c1_i64 : i64
  // CHECK-NEXT:   %2 = arith.divui %1, %arg1 : i64
  // CHECK-NEXT:   llvm.return %2 : i64
  // CHECK-NEXT: }

  // =================================================================================================

  llvm.func @ceilDivTest4(%arg0: i8, %arg1: i8) -> i8 attributes {llvm.linkage = #llvm.linkage<external>} {
    %0 = arith.ceildivsi %arg0, %arg1 : i8
    llvm.return %0 : i8
  }

  // CHECK: llvm.func @ceilDivTest4(%arg0: i8, %arg1: i8) -> i8 attributes {llvm.linkage = #llvm.linkage<external>} {
  // CHECK-NEXT:   %c1_i8 = arith.constant 1 : i8
  // CHECK-NEXT:   %0 = arith.addi %arg0, %arg1 : i8
  // CHECK-NEXT:   %1 = arith.subi %0, %c1_i8 : i8
  // CHECK-NEXT:   %2 = arith.divsi %1, %arg1 : i8
  // CHECK-NEXT:   llvm.return %2 : i8
  // CHECK-NEXT: }
  
  // =================================================================================================


  llvm.func @ceilDivTest5(%arg0: i32, %arg1: i32) -> i32 attributes {llvm.linkage = #llvm.linkage<external>} {
    %0 = arith.ceildivsi %arg0, %arg1 : i32
    llvm.return %0 : i32
  }

  // CHECK: llvm.func @ceilDivTest5(%arg0: i32, %arg1: i32) -> i32 attributes {llvm.linkage = #llvm.linkage<external>} {
  // CHECK-NEXT:   %c1_i32 = arith.constant 1 : i32
  // CHECK-NEXT:   %0 = arith.addi %arg0, %arg1 : i32
  // CHECK-NEXT:   %1 = arith.subi %0, %c1_i32 : i32
  // CHECK-NEXT:   %2 = arith.divsi %1, %arg1 : i32
  // CHECK-NEXT:   llvm.return %2 : i32
  // CHECK-NEXT: }

  // =================================================================================================


  llvm.func @ceilDivTest6(%arg0: i16, %arg1: i16) -> i16 attributes {llvm.linkage = #llvm.linkage<external>} {
    %0 = arith.ceildivui %arg0, %arg1 : i16
    llvm.return %0 : i16
  }

  // CHECK: llvm.func @ceilDivTest6(%arg0: i16, %arg1: i16) -> i16 attributes {llvm.linkage = #llvm.linkage<external>} {
  // CHECK-NEXT:   %c1_i16 = arith.constant 1 : i16
  // CHECK-NEXT:   %0 = arith.addi %arg0, %arg1 : i16
  // CHECK-NEXT:   %1 = arith.subi %0, %c1_i16 : i16
  // CHECK-NEXT:   %2 = arith.divui %1, %arg1 : i16
  // CHECK-NEXT:   llvm.return %2 : i16
  // CHECK-NEXT: }

  // =================================================================================================


  llvm.func @ceilDivTest7(%arg0: i64, %arg1: i64) -> i64 attributes {llvm.linkage = #llvm.linkage<external>} {
    %0 = arith.ceildivsi %arg0, %arg1 : i64
    llvm.return %0 : i64
  }

  // CHECK: llvm.func @ceilDivTest7(%arg0: i64, %arg1: i64) -> i64 attributes {llvm.linkage = #llvm.linkage<external>} {
  // CHECK-NEXT:   %c1_i64 = arith.constant 1 : i64
  // CHECK-NEXT:   %0 = arith.addi %arg0, %arg1 : i64
  // CHECK-NEXT:   %1 = arith.subi %0, %c1_i64 : i64
  // CHECK-NEXT:   %2 = arith.divsi %1, %arg1 : i64
  // CHECK-NEXT:   llvm.return %2 : i64
  // CHECK-NEXT: }

  // =================================================================================================

  llvm.func @ceilDivTest8(%arg0: i8, %arg1: i8) -> i8 attributes {llvm.linkage = #llvm.linkage<external>} {
    %0 = arith.ceildivui %arg0, %arg1 : i8
    llvm.return %0 : i8
  }

  // CHECK: llvm.func @ceilDivTest8(%arg0: i8, %arg1: i8) -> i8 attributes {llvm.linkage = #llvm.linkage<external>} {
  // CHECK-NEXT:   %c1_i8 = arith.constant 1 : i8
  // CHECK-NEXT:   %0 = arith.addi %arg0, %arg1 : i8
  // CHECK-NEXT:   %1 = arith.subi %0, %c1_i8 : i8
  // CHECK-NEXT:   %2 = arith.divui %1, %arg1 : i8
  // CHECK-NEXT:   llvm.return %2 : i8
  // CHECK-NEXT: }
}
