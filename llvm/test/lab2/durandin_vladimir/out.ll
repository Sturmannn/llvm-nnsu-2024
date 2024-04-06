; ModuleID = '/home/sturmannn/llvm-nnsu-2024/llvm/test/lab2/durandin_vladimir/test.ll'
source_filename = "./llvm/test/lab2/durandin_vladimir/test.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local void @_Z4funcv() #0 {
entry:
  %a = alloca i32, align 4
  store i32 0, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  %add = add nsw i32 %0, 1
  store i32 %add, ptr %a, align 4
  ret void
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local noundef i32 @_Z3fooi(i32 noundef %num) #0 {
entry.split:
  %num.addr = alloca i32, align 4
  store i32 %num, ptr %num.addr, align 4
  br label %0

0:                                                ; preds = %entry.split
  %1 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  %2 = load i32, ptr %1, align 4
  %3 = add nsw i32 %2, 1
  store i32 %3, ptr %1, align 4
  br label %entry

entry:                                            ; preds = %0
  %4 = load i32, ptr %num.addr, align 4
  ret i32 %4
}

attributes #0 = { mustprogress noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 17.0.6 (https://github.com/Sturmannn/llvm-nnsu-2024.git 45fa684ae500720b84b7f2a1c946dbffbc80d514)"}
