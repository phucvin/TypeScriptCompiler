cd ..

cd 3rdParty

git clone --depth 1 https://github.com/llvm/llvm-project.git

cd ..

sudo apt install ninja-build python-yaml

install CMake https://askubuntu.com/a/865294

./prepare_3rdParty_release.sh

cd tsc

./config_tsc_release.sh

./build_tsc_release.sh


Try wasm:

./scripts/config_llvm_wasm_release.sh

./scripts/build_llvm_wasm_release.sh

Notes:
- Example fib and its compiled LLVM IR, optimized LLVM IR (https://godbolt.org/z/c7MfKTKqe):
```
function fib(n: number): number {
    if (n < 2) return n;
    return fib(n-1) + fib(n-2);
}

function main() {
    print(fib(40));
}
```
```
...
@frmt_16786877787734454590 = internal constant [3 x i8] c"%g\00"

declare ptr @malloc(i64)

declare void @free(ptr)

declare i32 @puts(ptr)

declare i32 @snprintf(ptr, i32, ptr, ...)

define double @fib(double %0) {
  %2 = alloca double, align 8
  %3 = alloca double, align 8
  store double %0, ptr %2, align 8
  %4 = load double, ptr %2, align 8
  %5 = fcmp olt double %4, 2.000000e+00
  br i1 %5, label %6, label %8

6:
  %7 = load double, ptr %2, align 8
  store double %7, ptr %3, align 8
  br label %16

8:
  %9 = load double, ptr %2, align 8
  %10 = fsub double %9, 1.000000e+00
  %11 = call double @fib(double %10)
  %12 = load double, ptr %2, align 8
  %13 = fsub double %12, 2.000000e+00
  %14 = call double @fib(double %13)
  %15 = fadd double %11, %14
  store double %15, ptr %3, align 8
  br label %16

16:
  %17 = load double, ptr %3, align 8
  ret double %17
}

define void @main() {
  %1 = call double @fib(double 4.000000e+01)
  %2 = call ptr @malloc(i64 50)
  %3 = call i32 (ptr, i32, ptr, ...) @snprintf(ptr %2, i32 50, ptr @frmt_16786877787734454590, double %1)
  %4 = call i32 @puts(ptr %2)
  ret void
}

...
```
```
source_filename = "LLVMDialectModule"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

declare noalias noundef ptr @malloc(i64 noundef) local_unnamed_addr #0

declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #1

declare i32 @snprintf(ptr, i32, ptr, ...) local_unnamed_addr

define double @fib(double %0) local_unnamed_addr #2 {
  %2 = fcmp olt double %0, 2.000000e+00
  br i1 %2, label %common.ret6, label %3

common.ret6:                                      ; preds = %1, %3
  %common.ret6.op = phi double [ %8, %3 ], [ %0, %1 ]
  ret double %common.ret6.op

3:                                                ; preds = %1
  %4 = fadd double %0, -1.000000e+00
  %5 = tail call double @fib(double %4)
  %6 = fadd double %0, -2.000000e+00
  %7 = tail call double @fib(double %6)
  %8 = fadd double %5, %7
  br label %common.ret6
}

define void @main() local_unnamed_addr {
  %1 = tail call double @fib(double 4.000000e+01)
  %2 = tail call dereferenceable_or_null(50) ptr @malloc(i64 50)
  %3 = tail call i32 (ptr, i32, ptr, ...) @snprintf(ptr %2, i32 50, ptr nonnull @frmt_16786877787734454590, double %1)
  %4 = tail call i32 @puts(ptr nonnull dereferenceable(1) %2)
  ret void
}

attributes #0 = { mustprogress nofree nounwind willreturn allockind("alloc,uninitialized") allocsize(0) memory(inaccessiblemem: readwrite) "alloc-family"="malloc" }
attributes #1 = { nofree nounwind }
attributes #2 = { nofree nosync nounwind memory(none) }
```
Wasm (https://compiler-explorer.com/z/f7n99dd8Y):
```
fib:                                    # @fib
        global.get      __stack_pointer
        i32.const       16
        i32.sub 
        local.tee       1
        global.set      __stack_pointer
        local.get       1
        local.get       0
        f64.store       8
        block           
        block           
        local.get       0
        f64.const       0x1p1
        f64.lt  
        i32.eqz
        br_if           0                               # 0: down to label1
        local.get       1
        local.get       1
        f64.load        8
        f64.store       0
        br              1                               # 1: down to label0
        end_block                               # label1:
        local.get       1
        local.get       1
        f64.load        8
        f64.const       -0x1p0
        f64.add 
        call    fib
        local.get       1
        f64.load        8
        f64.const       -0x1p1
        f64.add 
        call    fib
        f64.add 
        f64.store       0
        end_block                               # label0:
        local.get       1
        f64.load        0
        local.set       0
        local.get       1
        i32.const       16
        i32.add 
        global.set      __stack_pointer
        local.get       0
        end_function
main:                                   # @main
        global.get      __stack_pointer
        i32.const       16
        i32.sub 
        local.tee       0
        global.set      __stack_pointer
        f64.const       0x1.4p5
        call    fib
        local.set       1
        i64.const       50
        call    malloc
        local.set       2
        local.get       0
        local.get       1
        f64.store       0
        local.get       2
        i32.const       50
        i32.const       frmt_16786877787734454590
        local.get       0
        call    snprintf
        drop
        local.get       2
        call    puts
        drop
        local.get       0
        i32.const       16
        i32.add 
        global.set      __stack_pointer
        end_function
frmt_16786877787734454590:
        .asciz  "%g"
```

- Example closure:
```
function c1() {
    let x = 1;
    function f() {
        return ++x;
    }
    return f() + f();
}

function main() {
    print(c1());
}
```
```
@frmt_4606706533159743250 = internal constant [3 x i8] c"%d\00"

declare ptr @malloc(i64)

declare void @free(ptr)

declare i32 @puts(ptr)

declare i32 @snprintf(ptr, i32, ptr, ...)

define i32 @.f_c1.f(ptr %0) {
  %2 = alloca i32, align 4
  %3 = getelementptr { ptr }, ptr %0, i32 0, i32 0
  %4 = load ptr, ptr %3, align 8
  %5 = load i32, ptr %4, align 4
  %6 = add i32 %5, 1
  store i32 %6, ptr %4, align 4
  store i32 %6, ptr %2, align 4
  %7 = load i32, ptr %2, align 4
  ret i32 %7
}

define i32 @c1() {
  %1 = alloca i32, align 4
  %2 = call ptr @malloc(i64 ptrtoint (ptr getelementptr (i32, ptr null, i64 1) to i64))
  store i32 1, ptr %2, align 4
  %3 = call ptr @malloc(i64 ptrtoint (ptr getelementptr ({ ptr }, ptr null, i64 1) to i64))
  %4 = getelementptr { ptr }, ptr %3, i32 0, i32 0
  store ptr %2, ptr %4, align 8
  %5 = call i32 @.f_c1.f(ptr %3)
  %6 = call ptr @malloc(i64 ptrtoint (ptr getelementptr ({ ptr }, ptr null, i64 1) to i64))
  %7 = getelementptr { ptr }, ptr %6, i32 0, i32 0
  store ptr %2, ptr %7, align 8
  %8 = call i32 @.f_c1.f(ptr %6)
  %9 = add i32 %5, %8
  store i32 %9, ptr %1, align 4
  %10 = load i32, ptr %1, align 4
  ret i32 %10
}

define void @main() {
  %1 = call i32 @c1()
  %2 = call ptr @malloc(i64 50)
  %3 = call i32 (ptr, i32, ptr, ...) @snprintf(ptr %2, i32 50, ptr @frmt_4606706533159743250, i32 %1)
  %4 = call i32 @puts(ptr %2)
  ret void
}
```
Wasm (https://compiler-explorer.com/z/9vq3c8WEe):
```
.f_c1.f:                                # @.f_c1.f
        global.get      __stack_pointer
        local.set       1
        local.get       0
        i32.load        0
        local.tee       0
        local.get       0
        i32.load        0
        i32.const       1
        i32.add 
        local.tee       0
        i32.store       0
        local.get       1
        i32.const       16
        i32.sub 
        local.get       0
        i32.store       12
        local.get       0
        end_function
c1:                                     # @c1
        global.get      __stack_pointer
        i32.const       16
        i32.sub 
        local.tee       0
        global.set      __stack_pointer
        i64.const       4
        call    malloc
        local.tee       1
        i32.const       1
        i32.store       0
        i64.const       4
        call    malloc
        local.tee       2
        local.get       1
        i32.store       0
        local.get       2
        call    .f_c1.f
        local.set       2
        i64.const       4
        call    malloc
        local.tee       3
        local.get       1
        i32.store       0
        local.get       0
        local.get       2
        local.get       3
        call    .f_c1.f
        i32.add 
        local.tee       1
        i32.store       12
        local.get       0
        i32.const       16
        i32.add 
        global.set      __stack_pointer
        local.get       1
        end_function
main:                                   # @main
        global.get      __stack_pointer
        i32.const       16
        i32.sub 
        local.tee       0
        global.set      __stack_pointer
        call    c1
        local.set       1
        i64.const       50
        call    malloc
        local.set       2
        local.get       0
        local.get       1
        i32.store       0
        local.get       2
        i32.const       50
        i32.const       frmt_4606706533159743250
        local.get       0
        call    snprintf
        drop
        local.get       2
        call    puts
        drop
        local.get       0
        i32.const       16
        i32.add 
        global.set      __stack_pointer
        end_function
frmt_4606706533159743250:
        .asciz  "%d"
```


Notes:
- https://godbolt.org/z/h4naoa1Kc