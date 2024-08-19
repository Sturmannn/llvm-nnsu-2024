// RUN: mlir-opt -load-pass-plugin=%mlir_lib_dir/LysanovaMathFMA_MLIR_PASS%shlibext --pass-pipeline="builtin.module(llvm.func(lysanova-fma))" %s | FileCheck %s

// COM: LLVM IR for further translation into MLIR was generated with the optimization key -O2

module attributes {dlti.dl_spec = #dlti.dl_spec<#dlti.dl_entry<f16, dense<16> : vector<2xi32>>, #dlti.dl_entry<f64, dense<64> : vector<2xi32>>, #dlti.dl_entry<i32, dense<32> : vector<2xi32>>, #dlti.dl_entry<i16, dense<16> : vector<2xi32>>, #dlti.dl_entry<i8, dense<8> : vector<2xi32>>, #dlti.dl_entry<i1, dense<8> : vector<2xi32>>, #dlti.dl_entry<!llvm.ptr, dense<64> : vector<4xi32>>, #dlti.dl_entry<i64, dense<64> : vector<2xi32>>, #dlti.dl_entry<f80, dense<128> : vector<2xi32>>, #dlti.dl_entry<!llvm.ptr<271>, dense<32> : vector<4xi32>>, #dlti.dl_entry<!llvm.ptr<272>, dense<64> : vector<4xi32>>, #dlti.dl_entry<!llvm.ptr<270>, dense<32> : vector<4xi32>>, #dlti.dl_entry<f128, dense<128> : vector<2xi32>>, #dlti.dl_entry<"dlti.endianness", "little">, #dlti.dl_entry<"dlti.stack_alignment", 128 : i32>>} {
  
  // COM: double test1(double a, double b, double c) {
  // COM:   double mul = a * b;
  // COM:   double add = mul + c;
  // COM:   return add;
  // COM: }
  llvm.func local_unnamed_addr @_Z5test1ddd(%arg0: f64 {llvm.noundef}, %arg1: f64 {llvm.noundef}, %arg2: f64 {llvm.noundef}) -> (f64 {llvm.noundef}) attributes {memory = #llvm.memory_effects<other = none, argMem = none, inaccessibleMem = none>, passthrough = ["mustprogress", "nofree", "norecurse", "nosync", "nounwind", "willreturn", ["uwtable", "2"], ["min-legal-vector-width", "0"], ["no-trapping-math", "true"], ["stack-protector-buffer-size", "8"], ["target-cpu", "x86-64"], ["target-features", "+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87"], ["tune-cpu", "generic"]]} {
    %0 = llvm.fmul %arg0, %arg1  : f64
    %1 = llvm.fadd %0, %arg2  : f64
    llvm.return %1 : f64
  }
  // CHECK-LABEL: llvm.func local_unnamed_addr @_Z5test1ddd(%arg0: f64 {llvm.noundef}, %arg1: f64 {llvm.noundef}, %arg2: f64 {llvm.noundef}) -> (f64 {llvm.noundef}) attributes {memory = #llvm.memory_effects<other = none, argMem = none, inaccessibleMem = none>, passthrough = ["mustprogress", "nofree", "norecurse", "nosync", "nounwind", "willreturn", ["uwtable", "2"], ["min-legal-vector-width", "0"], ["no-trapping-math", "true"], ["stack-protector-buffer-size", "8"], ["target-cpu", "x86-64"], ["target-features", "+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87"], ["tune-cpu", "generic"]]} {
  // CHECK-NEXT:   %0 = llvm.intr.fma(%arg0, %arg1, %arg2)  : (f64, f64, f64) -> f64
  // CHECK-NEXT:   llvm.return %0 : f64
  // CHECK-NEXT: }

// =====================================================================================
  
  // COM: float test2(float a, float b, float c) {
  // COM:   float mul = a * b;
  // COM:   float add = mul + c;
  // COM:   return add;
  // COM: }
  llvm.func local_unnamed_addr @_Z5test2fff(%arg0: f32 {llvm.noundef}, %arg1: f32 {llvm.noundef}, %arg2: f32 {llvm.noundef}) -> (f32 {llvm.noundef}) attributes {memory = #llvm.memory_effects<other = none, argMem = none, inaccessibleMem = none>, passthrough = ["mustprogress", "nofree", "norecurse", "nosync", "nounwind", "willreturn", ["uwtable", "2"], ["min-legal-vector-width", "0"], ["no-trapping-math", "true"], ["stack-protector-buffer-size", "8"], ["target-cpu", "x86-64"], ["target-features", "+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87"], ["tune-cpu", "generic"]]} {
    %0 = llvm.fmul %arg0, %arg1  : f32
    %1 = llvm.fadd %0, %arg2  : f32
    llvm.return %1 : f32
  }
  // CHECK-LABEL: llvm.func local_unnamed_addr @_Z5test2fff(%arg0: f32 {llvm.noundef}, %arg1: f32 {llvm.noundef}, %arg2: f32 {llvm.noundef}) -> (f32 {llvm.noundef}) attributes {memory = #llvm.memory_effects<other = none, argMem = none, inaccessibleMem = none>, passthrough = ["mustprogress", "nofree", "norecurse", "nosync", "nounwind", "willreturn", ["uwtable", "2"], ["min-legal-vector-width", "0"], ["no-trapping-math", "true"], ["stack-protector-buffer-size", "8"], ["target-cpu", "x86-64"], ["target-features", "+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87"], ["tune-cpu", "generic"]]} {
  // CHECK-NEXT:   %0 = llvm.intr.fma(%arg0, %arg1, %arg2)  : (f32, f32, f32) -> f32
  // CHECK-NEXT:   llvm.return %0 : f32
  // CHECK-NEXT: }

// =====================================================================================
  
  // COM: double test3(double a, double b, double c) {
  // COM:   double mul = a * b;
  // COM:   double add = c + mul;
  // COM:   return add;
  // COM: }
  llvm.func local_unnamed_addr @_Z5test3ddd(%arg0: f64 {llvm.noundef}, %arg1: f64 {llvm.noundef}, %arg2: f64 {llvm.noundef}) -> (f64 {llvm.noundef}) attributes {memory = #llvm.memory_effects<other = none, argMem = none, inaccessibleMem = none>, passthrough = ["mustprogress", "nofree", "norecurse", "nosync", "nounwind", "willreturn", ["uwtable", "2"], ["min-legal-vector-width", "0"], ["no-trapping-math", "true"], ["stack-protector-buffer-size", "8"], ["target-cpu", "x86-64"], ["target-features", "+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87"], ["tune-cpu", "generic"]]} {
    %0 = llvm.fmul %arg0, %arg1  : f64
    %1 = llvm.fadd %0, %arg2  : f64
    llvm.return %1 : f64
  }
  // CHECK-LABEL: llvm.func local_unnamed_addr @_Z5test3ddd(%arg0: f64 {llvm.noundef}, %arg1: f64 {llvm.noundef}, %arg2: f64 {llvm.noundef}) -> (f64 {llvm.noundef}) attributes {memory = #llvm.memory_effects<other = none, argMem = none, inaccessibleMem = none>, passthrough = ["mustprogress", "nofree", "norecurse", "nosync", "nounwind", "willreturn", ["uwtable", "2"], ["min-legal-vector-width", "0"], ["no-trapping-math", "true"], ["stack-protector-buffer-size", "8"], ["target-cpu", "x86-64"], ["target-features", "+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87"], ["tune-cpu", "generic"]]} {
  // CHECK-NEXT:   %0 = llvm.intr.fma(%arg0, %arg1, %arg2)  : (f64, f64, f64) -> f64
  // CHECK-NEXT:   llvm.return %0 : f64
  // CHECK-NEXT: }

// =====================================================================================
  
  // COM: double test4(double a, double b, double c) {
  // COM:   double mul = a * b;
  // COM:   double add = mul + c;
  // COM:   double use = mul + a;
  // COM:   return add + use;
  // COM: }
  llvm.func local_unnamed_addr @_Z5test4ddd(%arg0: f64 {llvm.noundef}, %arg1: f64 {llvm.noundef}, %arg2: f64 {llvm.noundef}) -> (f64 {llvm.noundef}) attributes {memory = #llvm.memory_effects<other = none, argMem = none, inaccessibleMem = none>, passthrough = ["mustprogress", "nofree", "norecurse", "nosync", "nounwind", "willreturn", ["uwtable", "2"], ["min-legal-vector-width", "0"], ["no-trapping-math", "true"], ["stack-protector-buffer-size", "8"], ["target-cpu", "x86-64"], ["target-features", "+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87"], ["tune-cpu", "generic"]]} {
    %0 = llvm.fmul %arg0, %arg1  : f64
    %1 = llvm.fadd %0, %arg2  : f64
    %2 = llvm.fadd %0, %arg0  : f64
    %3 = llvm.fadd %1, %2  : f64
    llvm.return %3 : f64
  }
  // CHECK-LABEL: llvm.func local_unnamed_addr @_Z5test4ddd(%arg0: f64 {llvm.noundef}, %arg1: f64 {llvm.noundef}, %arg2: f64 {llvm.noundef}) -> (f64 {llvm.noundef}) attributes {memory = #llvm.memory_effects<other = none, argMem = none, inaccessibleMem = none>, passthrough = ["mustprogress", "nofree", "norecurse", "nosync", "nounwind", "willreturn", ["uwtable", "2"], ["min-legal-vector-width", "0"], ["no-trapping-math", "true"], ["stack-protector-buffer-size", "8"], ["target-cpu", "x86-64"], ["target-features", "+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87"], ["tune-cpu", "generic"]]} {
  // CHECK-NEXT:   %0 = llvm.fmul %arg0, %arg1  : f64
  // CHECK-NEXT:   %1 = llvm.fadd %0, %arg2  : f64
  // CHECK-NEXT:   %2 = llvm.fadd %0, %arg0  : f64
  // CHECK-NEXT:   %3 = llvm.fadd %1, %2  : f64
  // CHECK-NEXT:   llvm.return %3 : f64
  // CHECK-NEXT: }

// =====================================================================================
  // COM: double test5(double a, double b, double c) {
  // COM:   double mul = a * b;
  // COM:   double add = mul + c;
  // COM:   double use_not_mul = c + a;
  // COM:   return add + use_not_mul;
  // COM: }
  llvm.func local_unnamed_addr @_Z5test5ddd(%arg0: f64 {llvm.noundef}, %arg1: f64 {llvm.noundef}, %arg2: f64 {llvm.noundef}) -> (f64 {llvm.noundef}) attributes {memory = #llvm.memory_effects<other = none, argMem = none, inaccessibleMem = none>, passthrough = ["mustprogress", "nofree", "norecurse", "nosync", "nounwind", "willreturn", ["uwtable", "2"], ["min-legal-vector-width", "0"], ["no-trapping-math", "true"], ["stack-protector-buffer-size", "8"], ["target-cpu", "x86-64"], ["target-features", "+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87"], ["tune-cpu", "generic"]]} {
    %0 = llvm.fmul %arg0, %arg1  : f64
    %1 = llvm.fadd %0, %arg2  : f64
    %2 = llvm.fadd %arg0, %arg2  : f64
    %3 = llvm.fadd %1, %2  : f64
    llvm.return %3 : f64
  }
  // CHECK-LABEL: llvm.func local_unnamed_addr @_Z5test5ddd(%arg0: f64 {llvm.noundef}, %arg1: f64 {llvm.noundef}, %arg2: f64 {llvm.noundef}) -> (f64 {llvm.noundef}) attributes {memory = #llvm.memory_effects<other = none, argMem = none, inaccessibleMem = none>, passthrough = ["mustprogress", "nofree", "norecurse", "nosync", "nounwind", "willreturn", ["uwtable", "2"], ["min-legal-vector-width", "0"], ["no-trapping-math", "true"], ["stack-protector-buffer-size", "8"], ["target-cpu", "x86-64"], ["target-features", "+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87"], ["tune-cpu", "generic"]]} {
  // CHECK-NEXT:   %0 = llvm.intr.fma(%arg0, %arg1, %arg2)  : (f64, f64, f64) -> f64
  // CHECK-NEXT:   %1 = llvm.fadd %arg0, %arg2  : f64
  // CHECK-NEXT:   %2 = llvm.fadd %0, %1  : f64
  // CHECK-NEXT:   llvm.return %2 : f64
  // CHECK-NEXT: }
}
