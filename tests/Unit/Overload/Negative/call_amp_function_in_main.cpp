// RUN: %amp_device -D__GPU__ %s -m32 -emit-llvm -c -S -O2 -o %t.ll
// RUN: mkdir -p %t
// RUN: %clamp-device %t.ll %t/kernel.cl
// RUN: pushd %t && %embed_kernel kernel.cl %t/kernel.o && popd
// RUN: %cxxamp %link %t/kernel.o %s -o %t.out 2>&1 | FileCheck --strict-whitespace %s

//////////////////////////////////////////////////////////////////////////////////
// Do not delete or add any line; it is referred to by absolute line number in the
// FileCheck lines below
//////////////////////////////////////////////////////////////////////////////
#include <amp.h>
using namespace concurrency;

int foo() restrict(amp)
{
  return 1;
}

int main()
{
  foo();
  return 1; // Should not compile
}
// CHECK: call_amp_function_in_main.cpp:[[@LINE-3]]:6: error:  'foo':  no overloaded function has restriction specifiers that are compatible with the ambient context 'main'
// CHECK-NEXT:  foo();
// CHECK-NEXT:     ^



