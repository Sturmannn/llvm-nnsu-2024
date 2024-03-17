// RUN: %clang_cc1 -load %llvmshlibdir/AlwaysInlineAttributePlugin%pluginext -plugin AddAlwaysInline %s 1>&1 | FileCheck %s --check-prefix=SUM
// SUM: __attribute__((always_inline)) int sum(int A, int B) {
// SUM-NEXT:   return A + B;
// SUM-NEXT: }
int sum(int A, int B) { return A + B; }

// RUN: %clang_cc1 -load %llvmshlibdir/AlwaysInlineAttributePlugin%pluginext -plugin AddAlwaysInline %s 1>&1 | FileCheck %s --check-prefix=EMPTY
// EMPTY: __attribute__((always_inline)) void checkEmpty() {
// EMPTY-NEXT: }
void checkEmpty() {}

// RUN: %clang_cc1 -load %llvmshlibdir/AlwaysInlineAttributePlugin%pluginext -plugin AddAlwaysInline %s 1>&1 | FileCheck %s --check-prefix=MIN-NESTED
// MIN-NESTED: int minNested(int A, int B) {
// MIN-NESTED-NEXT:     {
// MIN-NESTED-NEXT:         if (A < B) {
// MIN-NESTED-NEXT:             return A;
// MIN-NESTED-NEXT:         }
// MIN-NESTED-NEXT:         return B;
// MIN-NESTED-NEXT:     }
// MIN-NESTED-NEXT: }
int minNested(int A, int B) {
  {
    if (A < B) {
      return A;
    }
    return B;
  }
}

// CHECK: int min(int A, int B) {
// CHECK-NEXT:     if (A < b) {
// CHECK-NEXT:         return A;
// CHECK-NEXT:     }
// CHECK-NEXT:     return B;
// CHECK-NEXT: }
int min(int A, int B) {
  if (A < B) {
    return A;
  }
  return B;
}


// RUN: %clang_cc1 -load %llvmshlibdir/AlwaysInlineAttributePlugin%pluginext -plugin AddAlwaysInline %s 1>&1 | FileCheck %s --check-prefix=LOOP
// LOOP: int loop() {
// LOOP-NEXT:     int Counter = 0;
// LOOP-NEXT:     for (int I = 0; I < 2; I++) {
// LOOP-NEXT:         Counter += I;
// LOOP-NEXT:     }
// LOOP-NEXT:     return Counter;
// LOOP-NEXT: }
int loop() {
  int Counter = 0;
  for (int I = 0; I < 2; I++) {
    Counter += I;
  }
  return Counter;
}
