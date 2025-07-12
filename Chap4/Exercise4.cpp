#include <chrono>
#include <cstdio>
#include <cstring>
#include <sys/time.h>
#include <thread>

struct TimerClass {
public: // Methods
  // Constructor
  TimerClass(const char *name) noexcept
      : timestamp{std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch())} {
    // Initialize name string
    auto len = strlen(name) + 1;
    this->name = new char[len];
    std::strncpy(this->name, name, len);

    printf("%s creation time: %ld ms\n", name, timestamp.count());
  }

  // Copy constructor
  TimerClass(const TimerClass &other) noexcept : timestamp{other.timestamp} {
    // Initialize name string
    auto len = strlen(other.name) + 1;
    this->name = new char[len];
    std::strncpy(name, other.name, len);

    printf("%s creation time: %ld ms\n", name, timestamp.count());
  }

  // Copy assignment
  TimerClass &operator=(const TimerClass &other) {
    if (this == &other) {
      return *this;
    }

    // Copy other name into our name
    if (name) {
      // First free our current name
      delete[] name;
      name = nullptr;
    }
    auto len = strlen(other.name) + 1;
    name = new char[len];
    std::strncpy(name, other.name, len);

    timestamp = other.timestamp;
    return *this;
  }

  // Move constructor
  TimerClass(TimerClass &&other) noexcept : name{other.name}, timestamp{other.timestamp} {
    other.timestamp = std::chrono::milliseconds::zero();

    other.name = nullptr;
  }

  // Move assignment
  TimerClass &operator=(TimerClass &&other) noexcept {
    printf("Move assignment invoked\n");
    if (this == &other) {
      return *this;
    }

    timestamp = other.timestamp;
    other.timestamp = std::chrono::milliseconds::zero();

    if (name) {
      delete[] name;
      name = nullptr;
    }

    name = other.name;
    other.name = nullptr;

    return *this;
  }

  ~TimerClass() {
    if (name) {
      auto destructionTimestamp{
          std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch())};

      printf("%s destruction time: %ld ms\n", name,
             destructionTimestamp.count());
      printf("%s age of object: %ld ms\n", name,
             (destructionTimestamp - timestamp).count());

      // Delete name
      delete[] name;
      name = nullptr;
    }
  }

public: // Variables
  std::chrono::milliseconds timestamp;

private:
  char *name;
};

int main() {
  printf("Creating t1\n");
  TimerClass t1{"t1"};
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  printf("Creating t2\n");
  TimerClass t2{"t2"}; // New time; should be different from t1

  // Move semantics
  // printf("Moving t2 into t1\n");
  // t2 = std::move(t1);
  //
  // printf("Move constructing t3\n");
  // TimerClass t3(std::move(t2));

  // Copy semantics
  // Copy constructor
  printf("Creating t3 copy of t1\n");
  // TimerClass t3{t1};
  TimerClass t3{t1};

  // Copy assignment
  printf("Creating t4\n");
  TimerClass t4{"t4"};
  printf("Copying t1 into t4...\n");
  t4 = t1;
  printf("t4 time: %ld ms\n", t4.timestamp.count());

  return 0;
}
