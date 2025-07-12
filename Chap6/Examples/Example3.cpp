#include <iostream>

template <typename T, int N> void print_fixed(T (&arr)[N]) {
  for (int i = 0; i < N; ++i)
    std::cout << arr[i] << " ";
}

// Not OK if T is not deducible:
template <typename T, typename U> void combine(U second) { return; }

int main() {
  // OK, both T and N deduced:
  int a[]{1, 2, 3};
  print_fixed(a);

  combine<int, double>(42.0); // must specify both or neither
}
