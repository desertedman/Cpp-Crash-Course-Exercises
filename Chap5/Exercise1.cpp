#include <cstdio>
#include <exception>
#include <stdexcept>
#include <unordered_map>

// Interface
struct AccountDatabase {
public:
  virtual ~AccountDatabase() = default;

  virtual double getAmount(long id) const = 0;
  virtual void setAmount(long id, double amount) = 0;
  virtual void printDatabase() const noexcept = 0;

  // Interfaces should not contain variables; only functions
  // private:
  //   std::unordered_map<long, float> idMap;
};

struct InMemoryAccountDatabase : AccountDatabase {
public:
  /* Get amount from corresponding ID. If account does not exist, throw
   * exception */
  double getAmount(long id) const override {
    for (auto it : idMap) {
      if (it.first == id) {
        return it.second;
      }
    }

    throw std::runtime_error{"Account does not exist."};
  }

  /* Set amount in corresponding ID. If the amount is negative, throw exception
   */
  void setAmount(long id, double amount) override {
    if (amount < 0.00) {
      throw std::runtime_error{
          "Insufficient balance; amount cannot be negative."};
    }

    idMap.insert_or_assign(id, amount);
  }

  void printDatabase() const noexcept override {
    for (auto &it : idMap) {
      printf("ID %lu: $%.2f\n", it.first, it.second);
    }
  }

private:
  std::unordered_map<long, double> idMap;
};

struct Logger {
public:
  virtual ~Logger() = default;
  virtual void log_transfer(long from, long to, double amount) = 0;
};

struct ConsoleLogger : Logger {
public:
  ConsoleLogger(const char *string) : introString(string) {}

  void log_transfer(long from, long to, double amount) override {
    printf("%s %ld -> %ld: %.2f\n", introString, from, to, amount);
  }

private:
  const char *introString;
};

struct FileLogger : Logger {
public:
  void log_transfer(long from, long to, double amount) override {
    printf("[file] %ld,%ld,%.2f\n", from, to, amount);
  }
};

struct Bank {
public:
  Bank(Logger *logger, AccountDatabase &accountDatabase)
      : logger{logger}, accountDatabase(accountDatabase) {};

  void set_logger(Logger *new_logger) { logger = new_logger; }

  /* Sets the desired amount in an ID. If ID is not found, an exception is
   * thrown. */
  void setAmount(long id, double amount) {
    try {
      accountDatabase.setAmount(id, amount);
    }

    catch (std::exception &e) {
      printf("Exception: %s\n", e.what());
    }
  }

  /* Returns amount from ID. If ID is not found, an exception is thrown. */
  double getAmount(long id) const {
    try {
      return accountDatabase.getAmount(id);
    }

    catch (std::exception &e) {
      printf("Exception: %s\n", e.what());

      return -1.0;
    }
  }

  void make_transfer(long from, long to, double amount) {
    try {
      auto fromBalance = accountDatabase.getAmount(from) - amount;
      auto toBalance = accountDatabase.getAmount(to) + amount;

      if (fromBalance < 0.0) {
        throw std::runtime_error{"Insufficient balance of from account."};
      }

      accountDatabase.setAmount(from, fromBalance);
      accountDatabase.setAmount(to, toBalance);

      if (logger)
        logger->log_transfer(from, to, amount);
    }

    catch (std::exception &e) {
      printf("Exception: %s\n", e.what());
    }
  }

  void printDatabase() const { accountDatabase.printDatabase(); }

private:
  Logger *logger;
  AccountDatabase &accountDatabase;
};

int main() {
  ConsoleLogger console{"[test]"};
  FileLogger file;
  InMemoryAccountDatabase db;
  Bank bank{&console, db};

  // Add some accounts to db...
  bank.setAmount(1, 100.00);
  bank.setAmount(2, 250.00);
  bank.setAmount(3, 1000.00);
  bank.setAmount(3, -200.00); // Throws exception

  bank.printDatabase();

  // Make transfers
  bank.make_transfer(1, 2, 10000.00);
  bank.make_transfer(1, 2, 100.00);
  bank.make_transfer(1, 2, 100.00);
  bank.make_transfer(3, 1, 500.00);
  bank.make_transfer(3, 2, 250.00);
  bank.make_transfer(4, 2, 20000.00); // Throws exception

  bank.printDatabase();

  return 0;
}
