#include <algorithm>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>

constexpr uint8_t NUM_BITS = 8;
constexpr uint8_t BYTE_MAX = 0xFF;

class UnsignedBigInteger {
public:
  UnsignedBigInteger(unsigned long value) : data{} {
    // Size of value in bytes
    auto size{sizeof(unsigned long)};

    // Bitshift over a byte every iteration
    for (auto i{0}; i < size; i++) {
      auto byte{value >> (NUM_BITS * i)};
      data.push_back(byte);
    }
  }

  UnsignedBigInteger operator+(const unsigned long &other) const {
    auto sizeOther{sizeof(unsigned long)};

    std::vector<uint8_t> otherData{};

    // Push individual bytes in order of lowest order to highest
    for (auto i{0}; i < sizeOther; i++) {
      // Bitshift a byte over every iteration
      auto byte{other >> (NUM_BITS * i)};
      otherData.push_back(byte);
    }

    auto bytesToIterate{std::max(otherData.size(), data.size())};

    // Add padding to vector so they match size
    if (bytesToIterate != otherData.size()) {
      otherData.resize((bytesToIterate));
    }

    std::vector<uint8_t> resultData(bytesToIterate);

    size_t resultIndex{0};
    auto itOther = otherData.begin();
    auto itData = data.begin();

    uint8_t carry{0};
    while (itOther != otherData.end() && itData != data.end()) {
      // Resize container if we need more bytes
      if (resultIndex >= resultData.size()) {
        resultData.resize(resultData.size() + 1);
      }

      // Sum up values in otherData and data
      uint16_t sum = *itOther + *itData + carry;

      // Save overflow from bit addition
      // Ex. 0b1111 + 0b0001
      // This will overflow, so save the carry bit
      carry = (sum > BYTE_MAX) ? 1 : 0;

      resultData[resultIndex] = static_cast<uint8_t>(sum & BYTE_MAX);

      resultIndex++;
      itOther++;
      itData++;
    }

    if (carry) {
      resultData.push_back(carry);
    }

    uint64_t returnValue{0};

    // Perform final calculation
    for (auto i{0}; i < resultData.size(); i++) {
      uint64_t value = static_cast<uint64_t>(resultData[i]) << (NUM_BITS * i);
      returnValue += value;
    }

    UnsignedBigInteger returnInt{returnValue};
    return returnInt;
  }

  uint64_t getValue() const {
    uint64_t returnValue{0};

    // Perform final calculation
    for (size_t i{0}; i < data.size(); i++) {
      // Must cast to a uint64_t because bitwise operators promote to signed
      // arithmetic
      uint64_t value = static_cast<uint64_t>(data[i]) << (NUM_BITS * i);
      returnValue += value;
    }

    return returnValue;
  }

  void printDebug() const {
    std::cout << "Value: " << getValue() << std::endl;
    std::cout << "Size: " << data.size() << std::endl;
  };

private:
  std::vector<uint8_t> data;
};

int main() {
  UnsignedBigInteger a{INT_MAX};
  UnsignedBigInteger b{UINT32_MAX};
  UnsignedBigInteger c{9223372036854775808u}; // 1 greater than a long
  UnsignedBigInteger sum{b + 300'000};
  UnsignedBigInteger expected{4295267295};

  std::cout << "Max value of int: " << INT_MAX << std::endl;
  a.printDebug();

  std::cout << "Max value of uint32: " << UINT32_MAX << std::endl;
  b.printDebug();

  std::cout << "Max value of long: " << LONG_MAX << std::endl;
  c.printDebug();

  sum.printDebug();
  expected.printDebug();
}
