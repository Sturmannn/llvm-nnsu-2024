// RUN: %clang_cc1 -load %llvmshlibdir/PrintikClassPlugin%pluginext -plugin prin-elds %s 2>&1 | FileCheck %s --check-prefix=CHECK1

// CHECK1: Empty
class Empty {};

// CHECK1: MyClass
struct MyClass {
    // CHECK1-NEXT: |_variable
    int variable;
};

// CHECK1: Test
struct Test
{
    // CHECK1-NEXT: |_A
    int A;
    // CHECK1-NEXT: |_B
    int B;
};

// CHECK1: TClass
template<typename T> class TClass {
    // CHECK1-NEXT: |_TVar
    T TVar;
};

// CHECK1: ClassWithStaticFields
class ClassWithStaticFields {
    // CHECK1-NEXT: |_staticField
    static int staticField;
    // CHECK1-NEXT: |_staticField2
    static int staticField2;
    // CHECK1-NEXT: |_field
    float field;
};
// RUN: %clang_cc1 -load %llvmshlibdir/PrintikClassPlugin%pluginext -plugin prin-elds -plugin-arg-prin-elds no_fields %s 2>&1 | FileCheck %s --check-prefix=CHECK2

// CHECK2: AnotherClassWithStaticFields
class AnotherClassWithStaticFields {
    // CHECK2-NOT: |_Field1
    static int Field1;
    // CHECK2-NOT: |_Field2
    static int Field2;
    // CHECK2-NOT: |_field3
    float field3;
};
