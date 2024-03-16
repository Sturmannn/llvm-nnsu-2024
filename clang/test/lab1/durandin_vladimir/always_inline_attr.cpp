// RUN: %clang_cc1 -load %llvmshlibdir/AlwaysInlineAttributePlugin%pluginext
// -plugin AddAlwaysInline %s 1>&1 | FileCheck %s

// CHECK: __attribute__((always_inline)) int sum(int a, int b) {
// CHECK-NEXT:   return a + b;
// CHECK-NEXT: }
int sum(int a, int b) { return a + b; }

void checkEmpty() {}

// CHECK: int min_nested(int a, int b) {
// CHECK-NEXT:     {
// CHECK-NEXT:         if (a < b) {
// CHECK-NEXT:             return a;
// CHECK-NEXT:         } else
// CHECK-NEXT:             return b;
// CHECK-NEXT:     }
// CHECK-NEXT: }
int min_nested(int a, int b) {
  {
    if (a < b) {
      return a;
    } else
      return b;
  }
}

int min(int a, int b) {
  if (a < b) {
    return a;
  } else
    return b;
}

template <typename T> T min(T a, T b) { return a; }
