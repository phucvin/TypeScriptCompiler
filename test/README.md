cd ..

./prepare_3rdParty.sh

update CMake or change required CMakeLists.txt files to use the current version

cd tsc

./config_tsc_release.sh

./build_tsc_release.sh

Notes:
- Example fib and its compiled LLVM IR, using this [playground](https://godbolt.org/#z:OYLghAFBqd5TKALEBjA9gEwKYFFMCWALugE4A0BIEAZgQDbYB2AhgLbYgDkAjF%2BTXRMiAZVQtGIHgA4BQogFUAztgAKAD24AGfgCsp5eiyagiATwAO2JalIELRcisaoiBIdWaYAwunoBXNiYQACZyLwAZAiZsADlAgCNsUhAAZh5yC3QlYncmXwCg0Mzs3KEomPi2JJT0p2wXNyERIhZSIgLA4LDnbFc8lraiCrjE5LSMpVb2zqKe6eHo0erx9IBKJ3R/UlROLhYEqdIWVwBqVCMlJVOAEWwLIY5hU4BSAHYAIRetAEFT//OQiO/lcZAgFn8CXoBFQp1YHBApyO0WAa1en3eN2%2BP2xANOFjswli7GwEDWiIAbugCJh0V9fni8QTokQIC8QiE7g92k8iHCSYj2SFXiEPqciEgCEoAHTw7BrF6pel/AGY7G4gEHI4nPnM4QAWWw2DcJjJlOpmEVYoA9NbTmx/FNTklTgQ2BZGLzsLToqccHYKd7zpcVEp1W8sb9sRcWFdTj9UBh/MIUVzHsw%2Bdh1EQvNc0zyM3SNf8MExgaDSGSiwzGf8lP4rJWhQmkymTKdjLSfv5CCbgEKFUrTrbAWWiKQQSRSNdff6CIHaTGrtZ7Y6%2BeJ6PQkQ3kmTi%2BjIzia/89URDcaUWbTlSadWVbXT2yOQAVJDYeOJrZt4C3e7p54cMa1zYCcSCnPqQiYCwZgdnyPBaOw0oDla%2B5qlGx6nMAzDJCwOYAEr3GQRBKFeN60u8yq1iehKskKADi2HHH2HafsmzGkIR7QytKSEcoOlGqhG4aHtiNDJv0Qj2iw0RVhR%2B6MHyODckQvKIvmKkZlaw52ugADW4roOcHG4e%2BLCnBxNDJMwuwGR2TAdoc446uKljYPuSn/nyio3HC2AAO4fq2fbqbye5DiOem2bYIE5nZtKxjkwD2WZTBCAAtFqTlnPWCRLmGGEeQWwjSqexIcGFAn/IVGnFae559hV%2B62tVvLSlhMRMdgBFZFxFXaacySkGQiItYWUpwugfLoDQLlWIFX7BX%2BRWOOcxipXyJy7HG7U4fhnHEcJXAbPQ3AAKz8MEXA6OQ6DcM%2BrliHYDinMSbiBkiWw7O%2B7LpPwRDaEdGy6WkqTSqk4MQ5DkMAGyGNwAAs/BsCAbzw2DqTSJjqTw/DbzQ1oWjw%2BQl3XbdXD8EoIBaOQ/1XUd5BwLAKBZn0/hTpQ1BtMASiqMYDQiEg6B%2BZdfDkBg7oMLheS8zE9AC0LJP8OLFgMOMxGoAA%2BloWsa6kp0a8AqBi%2BgEuMKQZV7ErJsq2bADybPy8LAPhOofQ/KQ3PcPwLOoC0hCXfwgjCGIEicDwRNB4oKgaM7%2BgZEYJggOYVg2E9jjQoc8AbOgDh5JT5M3YGpB2DglOQBsSifbsBhTP7Mv84LTu8PwfnHBY3B8MdZ0Xc7ZO4K7qBs2Qpz3VYj32Hyr3zu%2Bvim8kpxaNKi8EtgeunAAUgAks%2BpwQHR3hohAo/YOPz1T4G5CnPgxDDz9PBrH9ANrBsb4sP61Bd1wiPkMjISndK0NoY8GAdDU6p0QipAAJyQKAcTXuXsnBUxpk/IGaR/4Yz1uAngeNIHYLeG8OGXBUg9zpjdBBtMdAbEZsgNA1tVYUCoBAZW9Ck42B1trPWBsjY4ApDCbAAA1Ag/lbZWBFoHBgOZpzUASM7BI0Q2hmA7vwORrBSBmFtgkXQfRaai3FryW2TB6CKNITgBI/hgDeAkPQfOoscBsGMMASQJiCAcX6IGfO10fZsz2KLFkDRnYZ2OGo3wOBnbjjdEojYNAjDc0EcI0RSi5DBw3GHCO8hlBqE0KQuOhgHFJ1cqnCehgCCZ3LjdXOQIEHoCLiXawWd6iNDyJ4JgPg/BdAMJEJYVQagGCyDkJo%2BQ2lFAyH0soTARjdPGJMBo2iBgLFmMEaZjTmgLAmWMFIkx5lDMWU4VZXT1lSArlXMOn9zpwNIWTEeD1bATxelLd6s8bbz0XsvDia8t47z3gfXe18pwinWI/Omz9yCv3fmScgwN0jShCNIdI4d4bSEgajRFWhZAnS/iQ0mCDKbUwofTahEAUAuI5kwuhZsLZ3WuWnO5b0Z5kueUvEqbzTob23rvfeD9wi9jIDSAwkcQ6SHDkkqOmTY41xmRJYIEAvALI6S0tZKwNklH6XkWVIzSgDIVT0pZsyVlDDVQ03VTBBjtC1VM3Z%2Brtk1z2ZUA598aYcWwLyrQpzMX8DJhECI/D9SnE3nhU4cS/Lz2PqfSe9y6Vz1IAvRlK93lsq%2BYfX5t8IEZFOI8%2Bh/z76Asoag8G6MoYFtSIQ7%2BisyEFxxcgoFn8QhurLdmwG5Ai45A8PDIAA%3D%3D):
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
