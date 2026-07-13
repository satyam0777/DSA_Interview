/*
    FILE: abstraction.cpp
    TOPIC: Abstraction

    THEORY:
    --------------------------------
    Abstraction = hiding COMPLEX IMPLEMENTATION details and showing only
    the ESSENTIAL features/interface to the user.
    ("What it does" vs "How it does it")

    In C++, abstraction is primarily achieved with:
      1) Abstract classes  - classes with at least one PURE VIRTUAL function
                              (`virtual ret func() = 0;`). Cannot be instantiated.
      2) Interfaces        - C++ has no `interface` keyword like Java; we
                              simulate it with a class that has ONLY pure
                              virtual functions (no data, no implementation).

    Abstraction vs Encapsulation (common interview confusion):
      - Encapsulation is about DATA HIDING (bundling data + restricting access).
      - Abstraction is about HIDING IMPLEMENTATION COMPLEXITY, exposing only
        a clean interface/contract. E.g., you call `car.start()` without
        knowing HOW the ignition system, fuel injection etc. work internally.

    REAL EXAMPLE: A Payment Gateway system (classic system-design-adjacent
    interview question: "Design a payment system supporting UPI, Card, Wallet")
    This is ALSO the "Strategy Design Pattern" - abstraction is the foundation
    of almost every design pattern you'll study next in System Design.

    Compile: g++ -std=c++17 abstraction.cpp -o abstraction
    Run:     ./abstraction
*/

#include <iostream>
#include <memory>
#include <vector>
using namespace std;

// ===========================================================
// ABSTRACT CLASS / INTERFACE
// PaymentMethod defines WHAT every payment method must be able to do,
// but not HOW. Each derived class provides its own "how".
// ===========================================================
class PaymentMethod {
public:
    // Pure virtual functions -> no implementation here, just a CONTRACT.
    // Any class inheriting PaymentMethod is FORCED to implement these,
    // otherwise IT ALSO becomes abstract and can't be instantiated.
    virtual bool authenticate() = 0;
    virtual bool processPayment(double amount) = 0;
    virtual string getMethodName() const = 0;

    // A concrete (non-pure) method can still exist in an abstract class -
    // shared logic that all derived classes can reuse as-is.
    void printReceipt(double amount) const {
        cout << "[Receipt] Paid " << amount << " via " << getMethodName() << endl;
    }

    virtual ~PaymentMethod() = default;   // virtual destructor - see 04_polymorphism
};

// Trying to do `PaymentMethod pm;` in main() would NOT compile -
// "cannot instantiate abstract class" - this enforces the abstraction:
// you MUST use a concrete implementation.

// ===========================================================
// CONCRETE IMPLEMENTATIONS - the messy "how" is hidden inside each
// ===========================================================
class UpiPayment : public PaymentMethod {
private:
    string upiId;
public:
    UpiPayment(string id) : upiId(id) {}

    bool authenticate() override {
        cout << "Sending OTP to UPI app for " << upiId << "...\n";
        return true;   // pretend OTP verified
    }

    bool processPayment(double amount) override {
        cout << "Debiting " << amount << " from UPI ID " << upiId << " via NPCI network...\n";
        return true;
    }

    string getMethodName() const override { return "UPI"; }
};

class CreditCardPayment : public PaymentMethod {
private:
    string cardNumberMasked;
public:
    CreditCardPayment(string maskedNum) : cardNumberMasked(maskedNum) {}

    bool authenticate() override {
        cout << "Verifying CVV & OTP for card " << cardNumberMasked << "...\n";
        return true;
    }

    bool processPayment(double amount) override {
        cout << "Charging " << amount << " to card " << cardNumberMasked
             << " via card network (Visa/Mastercard)...\n";
        return true;
    }

    string getMethodName() const override { return "Credit Card"; }
};

class WalletPayment : public PaymentMethod {
private:
    string walletProvider;
    double walletBalance;
public:
    WalletPayment(string provider, double balance)
        : walletProvider(provider), walletBalance(balance) {}

    bool authenticate() override {
        cout << "Validating wallet PIN for " << walletProvider << "...\n";
        return true;
    }

    bool processPayment(double amount) override {
        if (amount > walletBalance) {
            cout << "Insufficient wallet balance!\n";
            return false;
        }
        walletBalance -= amount;
        cout << "Deducted " << amount << " from " << walletProvider
             << " wallet. Remaining: " << walletBalance << endl;
        return true;
    }

    string getMethodName() const override { return walletProvider + " Wallet"; }
};

// ===========================================================
// CHECKOUT SYSTEM - depends only on the ABSTRACTION (PaymentMethod),
// never on concrete classes. This is the "Dependency Inversion" idea
// you'll meet formally in SOLID principles during system design.
// ===========================================================
class Checkout {
public:
    // Note the parameter type: PaymentMethod& (the interface), NOT
    // UpiPayment or CreditCardPayment. Checkout doesn't need to know
    // or care which one it is - THAT is abstraction in action.
    static void pay(PaymentMethod &method, double amount) {
        cout << "\n--- Checkout using " << method.getMethodName() << " ---\n";
        if (method.authenticate()) {
            if (method.processPayment(amount)) {
                method.printReceipt(amount);
            }
        } else {
            cout << "Authentication failed!\n";
        }
    }
};

int main() {
    UpiPayment upi("aditi@okhdfc");
    CreditCardPayment card("**** **** **** 4242");
    WalletPayment wallet("Paytm", 2000);

    // Checkout::pay() treats all three identically through the abstraction,
    // even though internally each does something completely different.
    Checkout::pay(upi, 499.0);
    Checkout::pay(card, 1500.0);
    Checkout::pay(wallet, 2500.0);   // will fail - amount > balance

    // We can also store different payment methods in a single collection
    // via the common abstraction - very useful for things like
    // "let the user pick any saved payment method at checkout"
    vector<unique_ptr<PaymentMethod>> savedMethods;
    savedMethods.push_back(make_unique<UpiPayment>("rahul@okicici"));
    savedMethods.push_back(make_unique<WalletPayment>("PhonePe", 1000));

    cout << "\n--- Iterating saved payment methods (polymorphism + abstraction) ---\n";
    for (auto &m : savedMethods) {
        Checkout::pay(*m, 300.0);
    }

    return 0;
}

/*
    INTERVIEW TALKING POINT:
    "This is literally the Strategy Design Pattern: PaymentMethod is the
    strategy interface, UpiPayment/CreditCardPayment/WalletPayment are
    concrete strategies, and Checkout is the context that uses whichever
    strategy is passed in - without knowing its internal details. This
    same abstraction technique is how real payment gateways (Stripe/Razorpay)
    let you 'plug in' new payment methods without touching checkout code -
    a nice bridge into System Design's Open-Closed Principle."
*/
