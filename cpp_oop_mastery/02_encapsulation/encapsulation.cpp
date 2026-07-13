/*
    FILE: encapsulation.cpp
    TOPIC: Encapsulation

    THEORY:
    --------------------------------
    Encapsulation = bundling data (attributes) + methods that operate on
    that data into a single unit (class), AND restricting direct access to
    the internal state (data hiding).

    Why it matters (real reason, not just theory):
    - Prevents invalid state. E.g., a bank balance should never become
      negative just because someone did `account.balance = -500;`
    - Lets you change the internal implementation later WITHOUT breaking
      code that uses the class (as long as the public interface stays same).
    - Centralizes validation logic in ONE place (the setter/method),
      instead of scattering "if balance < 0" checks everywhere in your app.

    REAL EXAMPLE: A Bank Account system.
    Compile: g++ -std=c++17 encapsulation.cpp -o encapsulation
    Run:     ./encapsulation
*/

#include <iostream>
#include <string>
#include <stdexcept>   // for throwing exceptions on invalid operations
using namespace std;

class BankAccount {
private:
    // All state is PRIVATE. The outside world can never do
    // `account.balance = 999999;` directly. This is the core of encapsulation.
    string accountHolder;
    string accountNumber;
    double balance;
    static int totalAccountsCreated;   // static = shared across ALL objects (see 06_advanced)

public:
    // Parameterized constructor validates input right at creation time.
    BankAccount(string holder, string accNum, double initialDeposit) {
        if (initialDeposit < 0)
            throw invalid_argument("Initial deposit cannot be negative");

        accountHolder = holder;
        accountNumber = accNum;
        balance = initialDeposit;
        totalAccountsCreated++;
    }

    // ---- Controlled access via public methods (the "encapsulation gate") ----

    // Deposits must be positive - business rule enforced HERE, in one place.
    void deposit(double amount) {
        if (amount <= 0)
            throw invalid_argument("Deposit amount must be positive");
        balance += amount;
        cout << "Deposited " << amount << " | New balance: " << balance << endl;
    }

    // Withdrawals must not overdraw the account - another business rule.
    void withdraw(double amount) {
        if (amount <= 0)
            throw invalid_argument("Withdrawal amount must be positive");
        if (amount > balance)
            throw runtime_error("Insufficient funds! Balance: " + to_string(balance));

        balance -= amount;
        cout << "Withdrew " << amount << " | New balance: " << balance << endl;
    }

    // A "getter" - read-only access to balance. Notice there's NO "setBalance()"
    // method - we deliberately don't expose a way to directly overwrite balance.
    // This is what encapsulation buys you: the ONLY ways to change balance
    // are deposit() and withdraw(), both of which enforce business rules.
    double getBalance() const {
        return balance;
    }

    string getAccountInfo() const {
        return accountNumber + " (" + accountHolder + ")";
    }

    static int getTotalAccounts() {
        return totalAccountsCreated;
    }
};

// Static members must be defined outside the class once (in a .cpp file).
int BankAccount::totalAccountsCreated = 0;

int main() {
    try {
        BankAccount acc1("Aditi Sharma", "ACC1001", 1000.0);
        BankAccount acc2("Rahul Verma", "ACC1002", 500.0);

        acc1.deposit(200);
        acc1.withdraw(150);

        cout << "\nFinal balance for " << acc1.getAccountInfo()
             << ": " << acc1.getBalance() << endl;

        cout << "Total bank accounts created: "
             << BankAccount::getTotalAccounts() << endl;

        // This next call is INTENTIONALLY going to throw - demonstrates
        // how encapsulation + exceptions protect account integrity.
        acc2.withdraw(10000);   // more than balance -> throws runtime_error

    } catch (const exception &ex) {
        // Because we encapsulated validation inside the class, ALL invalid
        // operations funnel through exceptions we can catch in ONE place.
        cout << "Transaction failed: " << ex.what() << endl;
    }

    return 0;
}

/*
    INTERVIEW TALKING POINT:
    "Encapsulation isn't just 'making variables private' - it's about
    exposing a minimal, safe, well-defined interface (deposit/withdraw)
    while hiding HOW balance is stored/validated internally. Tomorrow I
    could change `balance` to be stored in cents as an int, or fetched
    from a database, and no calling code would need to change."
*/
