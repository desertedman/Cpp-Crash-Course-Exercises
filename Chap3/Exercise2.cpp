#include <cstdio>

// First const means that data at ptr can't be changed
// Second const means that ptr itself can't be changed
char read_from(const char *const ptr, unsigned int size, unsigned int index) {
  // Out of bounds index
  if (index >= size) {
    printf("Out of bounds error\n");
    return -1;
  }

  return ptr[index];
}

int write_to(char *const ptr, unsigned int size, unsigned int index, char c) {
  // Out of bounds index
  if (index >= size) {
    printf("Out of bounds error\n");
    return -1;
  }

  // ptr[index] = c;
  *(ptr + index) = c;
  return 0;
}

int main() {
  char lower[] = "abc?e";
  char upper[] = "ABC?E";
  char *upper_ptr = upper; // Equivalent: &upper[0]

  // lower[3] = 'd';               // lower now contains a b c d e \0
  // upper_ptr[3] = 'D';           // upper now contains A B C D E \0
  // char letter_d = lower[3];     // letter_d equals 'd'
  // char letter_D = upper_ptr[3]; // letter_D equals 'D'

  auto lowerSize = sizeof(lower) / sizeof(char);
  auto upperSize = sizeof(upper) / sizeof(char);

  write_to(lower, lowerSize, 3, 'd');
  write_to(upper_ptr, upperSize, 3, 'D');
  char letter_d = read_from(lower, lowerSize, 3);
  char letter_D = read_from(upper_ptr, upperSize, 3);

  printf("lower: %s\nupper: %s\n", lower, upper);
  printf("letter_d: %c\n", letter_d);
  printf("letter_D: %c\n", letter_D);

  // lower[7] = 'g'; // Super bad. You must never do this.
  write_to(upper_ptr, upperSize, 7, 'g');
}
