#include <cstdio>
#include <type_traits>
#include <unordered_map>

template <typename T> T mode(const T *values, size_t length) {
  static_assert(std::is_arithmetic<T>(), "Must be arithmetic values\n");
  static_assert(!std::is_same<T, char>() && !std::is_same<T, unsigned char>() &&
                    !std::is_same<T, signed char>(),
                "Must not be any char type\n");
  static_assert(!std::is_floating_point<T>(), "Must be integral type\n");

  // No values found
  if (length == 0 || !values) {
    printf("Error: No values input\n");
    return 0;
  }

  // Create map of values
  std::unordered_map<T, int> map;
  for (size_t i = 0; i < length; i++) {
    auto it = map.find(values[i]);

    // No corresponding element found; add to map
    if (it == map.end()) {
      map.insert({values[i], 1});
    }

    // Element found; update count
    else {
      it->second++;
    }
  }

  int mode{};
  size_t maxValue{};
  for (auto it : map) {
    if (it.second > maxValue) {
      maxValue = it.second;
      mode = it.first;
    }
  }

  // Check for multiple modes
  for (auto it : map) {
    if (maxValue == it.second && mode != it.first) {
      printf("Error: multiple modes found\n");
      return 0;
    }
  }

  return mode;
}

int main() {
  int arr1[] = {1, 2, 2, 2, 3, 4};
  auto length1 = sizeof(arr1) / sizeof(int);

  int arr2[] = {};
  auto length2 = sizeof(arr2) / sizeof(int);

  char arr3[] = {'a', 'b', 'c'};

  float arr4[] = {4.0, 5.0, 2.0, 3.0};
  auto length4 = sizeof(arr4) / sizeof(float);

  // auto modeInt = mode(nullptr, length1);
  auto modeValue = mode(arr1, length1);
  // auto modeInt = mode(arr2, length2);
  // auto modeInt = mode(arr3, length1);
  // auto modeValue = mode(arr4, length4);

  printf("Mode: %d\n", modeValue);
}
