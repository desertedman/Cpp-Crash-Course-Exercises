#include <cstddef>
#include <cstdio>

template <typename T, size_t N> T mean(const T (&arr)[N]) {
  T result{};

  for (size_t i{}; i < N; i++) {
    result += arr[i];
  }

  return result / N;
}

int main() {
  const double nums_d[]{1.0, 2.0, 3.0, 4.0};
  const auto result1 = mean(nums_d);
  printf("double: %f\n", result1);

  const float nums_f[]{1.0f, 2.0f, 3.0f, 4.0f};
  const auto result2 = mean(nums_f);
  printf("float: %f\n", result2);

  const size_t nums_c[]{1, 2, 3, 4};
  const auto result3 = mean(nums_c);
  printf("size_t: %zd\n", result3);
}
