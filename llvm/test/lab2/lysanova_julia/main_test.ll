; RUN: opt -load-pass-plugin %llvmshlibdir/lysanova_loop_wrapper_plugin%pluginext\
; RUN: -passes=loop-wrapper-pass -S %s | FileCheck %s


; ModuleID = '/home/sturmannn/Translators/llvm-nnsu-2024/llvm/test/lab2/lysanova_julia/my_test.cpp'
source_filename = "/home/sturmannn/Translators/llvm-nnsu-2024/llvm/test/lab2/lysanova_julia/my_test.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"


; COM: Starting test without any loops

; int recursive_factorial(const int n) {
;   if (n <= 1)
;     return 1;
;   return n * recursive_factorial(n - 1);
; }

; Function Attrs: mustprogress noinline optnone uwtable
define dso_local noundef i32 @_Z19recursive_factoriali(i32 noundef %n) #0 {
entry:
  %retval = alloca i32, align 4
  %n.addr = alloca i32, align 4
  store i32 %n, ptr %n.addr, align 4
  %0 = load i32, ptr %n.addr, align 4
  %cmp = icmp sle i32 %0, 1
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 1, ptr %retval, align 4
  br label %return

if.end:                                           ; preds = %entry
  %1 = load i32, ptr %n.addr, align 4
  %2 = load i32, ptr %n.addr, align 4
  %sub = sub nsw i32 %2, 1
  %call = call noundef i32 @_Z19recursive_factoriali(i32 noundef %sub)
  %mul = mul nsw i32 %1, %call
  store i32 %mul, ptr %retval, align 4
  br label %return

return:                                           ; preds = %if.end, %if.then
  %3 = load i32, ptr %retval, align 4
  ret i32 %3
}

; CHECK-LABEL: define dso_local noundef i32 @_Z19recursive_factoriali(i32 noundef %n) #0 {
; CHECK-NEXT: entry:
; CHECK-NEXT:   %retval = alloca i32, align 4
; CHECK-NEXT:   %n.addr = alloca i32, align 4
; CHECK-NEXT:   store i32 %n, ptr %n.addr, align 4
; CHECK-NEXT:   %0 = load i32, ptr %n.addr, align 4
; CHECK-NEXT:   %cmp = icmp sle i32 %0, 1
; CHECK-NEXT:   br i1 %cmp, label %if.then, label %if.end
; CHECK-EMPTY: 
; CHECK-NEXT: if.then:                                          ; preds = %entry
; CHECK-NEXT:   store i32 1, ptr %retval, align 4
; CHECK-NEXT:   br label %return
; CHECK-EMPTY: 
; CHECK-NEXT: if.end:                                           ; preds = %entry
; CHECK-NEXT:   %1 = load i32, ptr %n.addr, align 4
; CHECK-NEXT:   %2 = load i32, ptr %n.addr, align 4
; CHECK-NEXT:   %sub = sub nsw i32 %2, 1
; CHECK-NEXT:   %call = call noundef i32 @_Z19recursive_factoriali(i32 noundef %sub)
; CHECK-NEXT:   %mul = mul nsw i32 %1, %call
; CHECK-NEXT:   store i32 %mul, ptr %retval, align 4
; CHECK-NEXT:   br label %return
; CHECK-EMPTY: 
; CHECK-NEXT: return:                                           ; preds = %if.end, %if.then
; CHECK-NEXT:   %3 = load i32, ptr %retval, align 4
; CHECK-NEXT:   ret i32 %3
; CHECK-NEXT: }

; ===================================================================================================

; COM: Empty function

; void empty_func() {}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local void @_Z10empty_funcv() #1 {
entry:
  ret void
}

; CHECK-LABEL: define dso_local void @_Z10empty_funcv() #1 {
; CHECK-NEXT: entry:
; CHECK-NEXT:   ret void
; CHECK-NEXT: }

; ===================================================================================================

; COM: Sample with "for" loop

; int for_loop(const int a, const int b) {
;   int res = 0;
;   if (a < b) {
;     for (int i = 0; i < 10; i++) {
;       res += 2;
;     }
;   } else {
;     for (int i = 0; i < 10; i++) {
;       res *= 2;
;     }
;   }
;   return res;
; }

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local noundef i32 @_Z8for_loopii(i32 noundef %a, i32 noundef %b) #1 {
; CHECK-LABEL: define dso_local noundef i32 @_Z8for_loopii(i32 noundef %a, i32 noundef %b) #1 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  %res = alloca i32, align 4
  %i = alloca i32, align 4
  %i2 = alloca i32, align 4
  store i32 %a, ptr %a.addr, align 4
  store i32 %b, ptr %b.addr, align 4
  store i32 0, ptr %res, align 4
  %0 = load i32, ptr %a.addr, align 4
  %1 = load i32, ptr %b.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  store i32 0, ptr %i, align 4
  br label %for.cond

; CHECK-LABEL: if.then:                                          ; preds = %entry
; CHECK-NEXT: store i32 0, ptr %i, align 4
; CHECK-NEXT: call void @_Z10loop_startv()
; CHECK-NEXT: br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.then
  %2 = load i32, ptr %i, align 4
  %cmp1 = icmp slt i32 %2, 10
  br i1 %cmp1, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %3 = load i32, ptr %res, align 4
  %add = add nsw i32 %3, 2
  store i32 %add, ptr %res, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %4 = load i32, ptr %i, align 4
  %inc = add nsw i32 %4, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !6

for.end:                                          ; preds = %for.cond
  br label %if.end

; CHECK-LABEL: for.end:                                          ; preds = %for.cond
; CHECK-NEXT:   call void @_Z8loop_endv()
; CHECK-NEXT:   br label %if.end

if.else:                                          ; preds = %entry
  store i32 0, ptr %i2, align 4
  br label %for.cond3

; CHECK-LABEL: if.else:                                          ; preds = %entry
; CHECK-NEXT:   store i32 0, ptr %i2, align 4
; CHECK-NEXT:   call void @_Z10loop_startv()
; CHECK-NEXT:   br label %for.cond3

for.cond3:                                        ; preds = %for.inc6, %if.else
  %5 = load i32, ptr %i2, align 4
  %cmp4 = icmp slt i32 %5, 10
  br i1 %cmp4, label %for.body5, label %for.end8

for.body5:                                        ; preds = %for.cond3
  %6 = load i32, ptr %res, align 4
  %mul = mul nsw i32 %6, 2
  store i32 %mul, ptr %res, align 4
  br label %for.inc6

for.inc6:                                         ; preds = %for.body5
  %7 = load i32, ptr %i2, align 4
  %inc7 = add nsw i32 %7, 1
  store i32 %inc7, ptr %i2, align 4
  br label %for.cond3, !llvm.loop !8

for.end8:                                         ; preds = %for.cond3
  br label %if.end

; CHECK-LABEL: for.end8:                                         ; preds = %for.cond3
; CHECK-NEXT:   call void @_Z8loop_endv()
; CHECK-NEXT:   br label %if.end

if.end:                                           ; preds = %for.end8, %for.end
  %8 = load i32, ptr %res, align 4
  ret i32 %8
}


; ===================================================================================================

; COM: Sample with "while" loop

; int while_loop(int n) {
;   int res = 0;
;   while (n >= 0) {
;     res /= n;
;     n--;
;   }
;   return res;
; }

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local noundef i32 @_Z10while_loopi(i32 noundef %n) #1 {
; CHECK-LABEL: define dso_local noundef i32 @_Z10while_loopi(i32 noundef %n) #1 {
entry:
  %n.addr = alloca i32, align 4
  %res = alloca i32, align 4
  store i32 %n, ptr %n.addr, align 4
  store i32 0, ptr %res, align 4
  br label %while.cond

; CHECK-LABEL: entry:
; CHECK-NEXT:   %n.addr = alloca i32, align 4
; CHECK-NEXT:   %res = alloca i32, align 4
; CHECK-NEXT:   store i32 %n, ptr %n.addr, align 4
; CHECK-NEXT:   store i32 0, ptr %res, align 4
; CHECK-NEXT:   call void @_Z10loop_startv()
; CHECK-NEXT:   br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %0 = load i32, ptr %n.addr, align 4
  %cmp = icmp sge i32 %0, 0
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %1 = load i32, ptr %n.addr, align 4
  %2 = load i32, ptr %res, align 4
  %div = sdiv i32 %2, %1
  store i32 %div, ptr %res, align 4
  %3 = load i32, ptr %n.addr, align 4
  %dec = add nsw i32 %3, -1
  store i32 %dec, ptr %n.addr, align 4
  br label %while.cond, !llvm.loop !9

while.end:                                        ; preds = %while.cond
  %4 = load i32, ptr %res, align 4
  ret i32 %4

; CHECK-LABEL: while.end:                                        ; preds = %while.cond
; CHECK-NEXT:   call void @_Z8loop_endv()
; CHECK-NEXT:   %4 = load i32, ptr %res, align 4
; CHECK-NEXT:   ret i32 %4
}

; ===================================================================================================

; COM: Sample with "do_while" loop

; int do_while_loop(int a, int b) {
;   int res = 0;
;   do {
;     res += a - b;
;     b++;
;   } while (a > b);
;   return res;
; }

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local noundef i32 @_Z13do_while_loopii(i32 noundef %a, i32 noundef %b) #1 {
; CHECK-LABEL: define dso_local noundef i32 @_Z13do_while_loopii(i32 noundef %a, i32 noundef %b) #1 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  %res = alloca i32, align 4
  store i32 %a, ptr %a.addr, align 4
  store i32 %b, ptr %b.addr, align 4
  store i32 0, ptr %res, align 4
  br label %do.body

; CHECK-LABEL: entry:
; CHECK-NEXT:   %a.addr = alloca i32, align 4
; CHECK-NEXT:   %b.addr = alloca i32, align 4
; CHECK-NEXT:   %res = alloca i32, align 4
; CHECK-NEXT:   store i32 %a, ptr %a.addr, align 4
; CHECK-NEXT:   store i32 %b, ptr %b.addr, align 4
; CHECK-NEXT:   store i32 0, ptr %res, align 4
; CHECK-NEXT:   call void @_Z10loop_startv()
; CHECK-NEXT:   br label %do.body

do.body:                                          ; preds = %do.cond, %entry
  %0 = load i32, ptr %a.addr, align 4
  %1 = load i32, ptr %b.addr, align 4
  %sub = sub nsw i32 %0, %1
  %2 = load i32, ptr %res, align 4
  %add = add nsw i32 %2, %sub
  store i32 %add, ptr %res, align 4
  %3 = load i32, ptr %b.addr, align 4
  %inc = add nsw i32 %3, 1
  store i32 %inc, ptr %b.addr, align 4
  br label %do.cond

do.cond:                                          ; preds = %do.body
  %4 = load i32, ptr %a.addr, align 4
  %5 = load i32, ptr %b.addr, align 4
  %cmp = icmp sgt i32 %4, %5
  br i1 %cmp, label %do.body, label %do.end, !llvm.loop !10

do.end:                                           ; preds = %do.cond
  %6 = load i32, ptr %res, align 4
  ret i32 %6

; CHECK-LABEL: do.end:                                           ; preds = %do.cond
; CHECK-NEXT:   call void @_Z8loop_endv()
; CHECK-NEXT:   %6 = load i32, ptr %res, align 4
; CHECK-NEXT:   ret i32 %6
}

; ===================================================================================================


; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local void @_Z10loop_startv() #1 {
entry:
  ret void
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local void @_Z8loop_endv() #1 {
entry:
  ret void
}

; ===================================================================================================

; COM: Sample where loop wrapper functions are already called

; void already_exist_loop_wrapper(const int n) {
;   loop_start();
;   for (int i = 0; i < n; ++i) {
;     int a = i;
;   }
;   loop_end();
; }

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local void @_Z26already_exist_loop_wrapperi(i32 noundef %n) #1 {
; CHECK-LABEL: define dso_local void @_Z26already_exist_loop_wrapperi(i32 noundef %n) #1 {
entry:
  %n.addr = alloca i32, align 4
  %i = alloca i32, align 4
  %a = alloca i32, align 4
  store i32 %n, ptr %n.addr, align 4
  call void @_Z10loop_startv()
  store i32 0, ptr %i, align 4
  br label %for.cond

; CHECK-LABEL: entry:
; CHECK-NEXT:   %n.addr = alloca i32, align 4
; CHECK-NEXT:   %i = alloca i32, align 4
; CHECK-NEXT:   %a = alloca i32, align 4
; CHECK-NEXT:   store i32 %n, ptr %n.addr, align 4
; CHECK-NEXT:   call void @_Z10loop_startv()
; CHECK-NEXT:   store i32 0, ptr %i, align 4
; CHECK-NOT:    call void @_Z10loop_startv()
; CHECK-NEXT:   br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %n.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load i32, ptr %i, align 4
  store i32 %2, ptr %a, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %3 = load i32, ptr %i, align 4
  %inc = add nsw i32 %3, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !11

for.end:                                          ; preds = %for.cond
  call void @_Z8loop_endv()
  ret void

; CHECK-LABEL: for.end:                                          ; preds = %for.cond
; CHECK-NEXT:   call void @_Z8loop_endv()
; CHECK-NOT:    call void @_Z8loop_endv()
; CHECK-NEXT:   ret void
}

; ===================================================================================================

; COM: Another one sample with different loops

; int big_test(int a, int b) {
;   int c = 0;
;   if (a < b) {
;     for (int i = 0; i < 10; i++) {
;       c = a + b;
;       if (c % 2 == 0)
;         return c;
;     }
;   } else {
;     while (a > b) {
;       c += 1;
;       a--;
;     }
;   }
;   return c;
; }

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local noundef i32 @_Z8big_testii(i32 noundef %a, i32 noundef %b) #1 {
entry:
  %retval = alloca i32, align 4
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  %c = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %a, ptr %a.addr, align 4
  store i32 %b, ptr %b.addr, align 4
  store i32 0, ptr %c, align 4
  %0 = load i32, ptr %a.addr, align 4
  %1 = load i32, ptr %b.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  store i32 0, ptr %i, align 4
  br label %for.cond

; CHECK-LABEL: if.then:                                          ; preds = %entry
; CHECK-NEXT:   store i32 0, ptr %i, align 4
; CHECK-NEXT:   call void @_Z10loop_startv()
; CHECK-NEXT:   br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.then
  %2 = load i32, ptr %i, align 4
  %cmp1 = icmp slt i32 %2, 10
  br i1 %cmp1, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %3 = load i32, ptr %a.addr, align 4
  %4 = load i32, ptr %b.addr, align 4
  %add = add nsw i32 %3, %4
  store i32 %add, ptr %c, align 4
  %5 = load i32, ptr %c, align 4
  %rem = srem i32 %5, 2
  %cmp2 = icmp eq i32 %rem, 0
  br i1 %cmp2, label %if.then3, label %if.end

if.then3:                                         ; preds = %for.body
  %6 = load i32, ptr %c, align 4
  store i32 %6, ptr %retval, align 4
  br label %return

; CHECK-LABEL: if.then3:                                         ; preds = %for.body
; CHECK-NEXT:   call void @_Z8loop_endv()
; CHECK-NEXT:   %6 = load i32, ptr %c, align 4
; CHECK-NEXT:   store i32 %6, ptr %retval, align 4
; CHECK-NEXT:   br label %return

if.end:                                           ; preds = %for.body
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %7 = load i32, ptr %i, align 4
  %inc = add nsw i32 %7, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !12

for.end:                                          ; preds = %for.cond
  br label %if.end6

; CHECK-LABEL: for.end:                                          ; preds = %for.cond
; CHECK-NEXT:   call void @_Z8loop_endv()
; CHECK-NEXT:   br label %if.end6

if.else:                                          ; preds = %entry
  br label %while.cond

; CHECK-LABEL: if.else:                                          ; preds = %entry
; CHECK-NEXT:   call void @_Z10loop_startv()
; CHECK-NEXT:   br label %while.cond

while.cond:                                       ; preds = %while.body, %if.else
  %8 = load i32, ptr %a.addr, align 4
  %9 = load i32, ptr %b.addr, align 4
  %cmp4 = icmp sgt i32 %8, %9
  br i1 %cmp4, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %10 = load i32, ptr %c, align 4
  %add5 = add nsw i32 %10, 1
  store i32 %add5, ptr %c, align 4
  %11 = load i32, ptr %a.addr, align 4
  %dec = add nsw i32 %11, -1
  store i32 %dec, ptr %a.addr, align 4
  br label %while.cond, !llvm.loop !13

while.end:                                        ; preds = %while.cond
  br label %if.end6

; CHECK-LABEL: while.end:                                        ; preds = %while.cond
; CHECK-NEXT:   call void @_Z8loop_endv()
; CHECK-NEXT:   br label %if.end6

if.end6:                                          ; preds = %while.end, %for.end
  %12 = load i32, ptr %c, align 4
  store i32 %12, ptr %retval, align 4
  br label %return

return:                                           ; preds = %if.end6, %if.then3
  %13 = load i32, ptr %retval, align 4
  ret i32 %13
}

attributes #0 = { mustprogress noinline optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 17.0.6 (https://github.com/Sturmannn/llvm-nnsu-2024.git 4b59e6c09e3fdd446f0c992dd37e478c4f9104c6)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
!13 = distinct !{!13, !7}
