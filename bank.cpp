#include <iostream>
#include <iomanip>

class Account {
protected:
    std::string accountNumber;
    std::string accountHolder;
    double balance;

public:
    Account(const std::string& number, const std::string& holder, double initialBalance)
        : accountNumber(number), accountHolder(holder), balance(initialBalance) {}

    virtual void displayDetails() const {
        std::cout << "Account Details for " << accountHolder << "'s Account (ID: " << accountNumber << "):\n"
                  << "   Holder: " << accountHolder << "\n"
                  << "   Balance: $" << std::fixed << std::setprecision(2) << balance << "\n";
    }

    virtual void deposit(double amount) {
        balance += amount;
    }

    virtual void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
        } else {
            std::cout << "Insufficient funds for withdrawal.\n";
        }
    }

    double getBalance() const {
        return balance;
    }
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
        : Account(number, holder, initialBalance), interestRate(rate) {}

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Interest Rate: " << std::fixed << std::setprecision(2) << interestRate * 100 << "%\n";
    }

    void withdraw(double amount) override {
        const double minBalance = 1000; // Minimum balance requirement for savings account
        if (balance - amount >= minBalance) {
            balance -= amount;
        } else {
            std::cout << "Withdrawal failed. Minimum balance requirement not met.\n";
        }
    }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double limit)
        : Account(number, holder, initialBalance), overdraftLimit(limit) {}

    void displayDetails() const override {
        Account::displayDetails();
        std::cout << "   Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraftLimit << "\n";
    }

    void withdraw(double amount) override {
        if (amount <= balance + overdraftLimit) {
            balance -= amount;
        } else {
            std::cout << "Withdrawal failed. Overdraft limit exceeded.\n";
        }
    }
};

// Overload + operator for transferring money between accounts
CurrentAccount operator+(CurrentAccount current, SavingsAccount& savings) {
    double transferAmount = 300;
    if (savings.getBalance() >= transferAmount) {
        current.deposit(transferAmount);
        savings.withdraw(transferAmount);
    } else {
        std::cout << "Transfer failed. Insufficient funds in the savings account.\n";
    }

    return current;
}

// Overload << operator to display account details
std::ostream& operator<<(std::ostream& os, const Account& account) {
    account.displayDetails();
    return os;
}

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    std::cout << savings;
    std::cout << current;

    savings.deposit(500);
    current.withdraw(1000);

    std::cout << "Account Details after deposit and withdrawal:\n";
    std::cout << savings;
    std::cout << current;

    current = current + savings;

    std::cout << "Account Details after transfer:\n";
    std::cout << savings;
    std::cout << current;

    return 0;
}
