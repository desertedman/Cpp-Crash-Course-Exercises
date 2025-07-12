#include <cstdio>

template <size_t Index, typename T, size_t Length>

T &get(T (&arr)[Length]) {
  static_assert(Index < Length, "Out-of-bounds access");
  return arr[Index];
}

int main() {
  int fib[]{1, 1, 2, 0};
  printf("%d %d %d ", get<0>(fib), get<1>(fib), get<2>(fib));
  get<3>(fib) = get<1>(fib) + get<2>(fib);
  printf("%d", get<3>(fib));
  // printf("%d", get<4>(fib));
}
