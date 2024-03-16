// RUN: %clang_cc1 -load %llvmshlibdir/AlwaysInlineAttributePlugin%pluginext -plugin AddAlwaysInline %s 1>&1 | FileCheck %s --check-prefix=SUM
// SUM: __attribute__((always_inline)) int sum(int a, int b) {
// SUM-NEXT:   return a + b;
// SUM-NEXT: }
int sum(int a, int b) { return a + b; }

// RUN: %clang_cc1 -load %llvmshlibdir/AlwaysInlineAttributePlugin%pluginext -plugin AddAlwaysInline %s 1>&1 | FileCheck %s --check-prefix=EMPTY
// EMPTY: __attribute__((always_inline)) void checkEmpty() {
// EMPTY-NEXT: }
void checkEmpty() {}

// RUN: %clang_cc1 -load %llvmshlibdir/AlwaysInlineAttributePlugin%pluginext -plugin AddAlwaysInline %s 1>&1 | FileCheck %s --check-prefix=MIN-NESTED
// MIN-NESTED: int min_nested(int a, int b) {
// MIN-NESTED-NEXT:     {
// MIN-NESTED-NEXT:         if (a < b) {
// MIN-NESTED-NEXT:             return a;
// MIN-NESTED-NEXT:         } else
// MIN-NESTED-NEXT:             return b;
// MIN-NESTED-NEXT:     }
// MIN-NESTED-NEXT: }
int min_nested(int a, int b) {
  {
    if (a < b) {
      return a;
    } else
      return b;
  }
}

// CHECK: int min(int a, int b) {
// CHECK-NEXT:     if (a < b) {
// CHECK-NEXT:         return a;
// CHECK-NEXT:     } else
// CHECK-NEXT:         return b;
// CHECK-NEXT: }
int min(int a, int b) {
  if (a < b) {
    return a;
  } else
    return b;
}


// RUN: %clang_cc1 -load %llvmshlibdir/AlwaysInlineAttributePlugin%pluginext -plugin AddAlwaysInline %s 1>&1 | FileCheck %s --check-prefix=LOOP
// LOOP: void loop() {
// LOOP-NEXT:     int counter = 0;
// LOOP-NEXT:     for (int i = 0; i < 2; i++) {
// LOOP-NEXT:         counter = i;
// LOOP-NEXT:     }
// LOOP-NEXT: }
void loop() {
  int counter = 0;
  for (int i = 0; i < 2; i++) {
    counter = i;
  }
}
