// XFAIL: Linux
// RUN: %cxxamp -Xclang -fhsa-ext %s -o %t.out && %t.out

// Parallel STL headers
#include <coordinate>
#include <experimental/algorithm>
#include <experimental/numeric>
#include <experimental/execution_policy>

#define _DEBUG (0)
#include "test_base.h"


template<typename T, size_t SIZE>
bool test(void) {

  // test kernel
  auto f = [](const T& v1, const T& v2)
  {
    return v1 * v2+1;
  };
  auto binary_op = std::plus<T>();
  auto init = T{};

  using namespace std::experimental::parallel;

  bool ret = true;
  bool eq = true;
  ret &= run<T, SIZE>([init, binary_op, f, &eq]
                      (T (&input1)[SIZE], T (&input2)[SIZE], T (&output1)[SIZE],
                                                             T (&output2)[SIZE]) {
    auto expected = std::inner_product(std::begin(input1), std::end(input1), std::begin(input2), init, binary_op, f);
    auto result =   inner_product(par, std::begin(input1), std::end(input1), std::begin(input2), init, binary_op, f);

    eq = EQ(expected, result);

  }, false);
  ret &= eq;

  return ret;
}

int main() {
  bool ret = true;

  ret &= test<int, TEST_SIZE>();
  ret &= test<unsigned, TEST_SIZE>();
  ret &= test<float, TEST_SIZE>();
  ret &= test<double, TEST_SIZE>();

  return !(ret == true);
}

