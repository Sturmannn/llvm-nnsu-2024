; ModuleID = '/home/sturmannn/llvm-nnsu-2024/llvm/test/lab3/durandin_vladimir/source_file.cpp'
source_filename = "/home/sturmannn/llvm-nnsu-2024/llvm/test/lab3/durandin_vladimir/source_file.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@ic = dso_local global i64 0, align 8

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local void @_Z10empty_funcv() #0 {
entry:
  ret void
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local noundef i32 @_Z9main_funciii(i32 noundef %a, i32 noundef %b, i32 noundef %c) #0 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  %c.addr = alloca i32, align 4
  %d = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %a, ptr %a.addr, align 4
  store i32 %b, ptr %b.addr, align 4
  store i32 %c, ptr %c.addr, align 4
  store i64 0, ptr @ic, align 8
  store i32 0, ptr %d, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %a.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load i32, ptr %d, align 4
  %3 = load i32, ptr %b.addr, align 4
  %cmp1 = icmp slt i32 %2, %3
  br i1 %cmp1, label %if.then, label %if.end

if.then:                                          ; preds = %for.body
  %4 = load i32, ptr %c.addr, align 4
  %5 = load i32, ptr %d, align 4
  %add = add nsw i32 %5, %4
  store i32 %add, ptr %d, align 4
  br label %if.end

if.end:                                           ; preds = %if.then, %for.body
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %6 = load i32, ptr %i, align 4
  %inc = add nsw i32 %6, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !6

for.end:                                          ; preds = %for.cond
  %7 = load i32, ptr %d, align 4
  ret i32 %7
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define dso_local noundef i32 @_Z11nested_funci(i32 noundef %n) #0 {
entry:
  %n.addr = alloca i32, align 4
  %res = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 %n, ptr %n.addr, align 4
  store i32 0, ptr %res, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc5, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %n.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end7

for.body:                                         ; preds = %for.cond
  store i32 0, ptr %j, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %2 = load i32, ptr %j, align 4
  %3 = load i32, ptr %n.addr, align 4
  %cmp2 = icmp slt i32 %2, %3
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %4 = load i32, ptr %i, align 4
  %5 = load i32, ptr %res, align 4
  %add = add nsw i32 %5, %4
  store i32 %add, ptr %res, align 4
  %6 = load i32, ptr %j, align 4
  %7 = load i32, ptr %res, align 4
  %add4 = add nsw i32 %7, %6
  store i32 %add4, ptr %res, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %8 = load i32, ptr %j, align 4
  %inc = add nsw i32 %8, 1
  store i32 %inc, ptr %j, align 4
  br label %for.cond1, !llvm.loop !8

for.end:                                          ; preds = %for.cond1
  br label %for.inc5

for.inc5:                                         ; preds = %for.end
  %9 = load i32, ptr %i, align 4
  %inc6 = add nsw i32 %9, 1
  store i32 %inc6, ptr %i, align 4
  br label %for.cond, !llvm.loop !9

for.end7:                                         ; preds = %for.cond
  %10 = load i32, ptr %res, align 4
  ret i32 %10
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
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
