#include <cstdio>

enum class Operation { Add, Subtract, Multiply, Divide };

struct Calculator {
public:
  Calculator() : mOperation(Operation::Add) {}
  Calculator(Operation operation) : mOperation(operation) {}

  int calculate(int a, int b) {
    switch (mOperation) {
    case Operation::Add:
      return a + b;
      break;
    case Operation::Subtract:
      return a - b;
      break;
    case Operation::Multiply:
      return a * b;
      break;
    case Operation::Divide:
      return a / b;
      break;
    default:
      break;
    }

    return 0;
  }

private:
  Operation mOperation;
};

int main() {
  Calculator calc{Operation::Multiply};
  int result = calc.calculate(4, 2);
  printf("Result: %d\n", result);

  Calculator calc2 {};
  result = calc2.calculate(4, 2);
  printf("Result: %d\n", result);

  return 0;
}
