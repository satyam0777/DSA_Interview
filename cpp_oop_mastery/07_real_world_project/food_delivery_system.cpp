/*
    FILE: food_delivery_system.cpp
    TOPIC: CAPSTONE PROJECT - A mini Food Delivery System
           (Zomato/Swiggy-style) using EVERY OOP concept from this folder set.

    This is the kind of "design a class for X" question asked in SDE-1
    interviews, and it's ALSO your first taste of System Design thinking
    (entities, relationships, extensibility) before you move to formal
    System Design study (scaling, databases, load balancing, etc.)

    CONCEPTS USED (mapped to the files you already studied):
      - Encapsulation        -> User, Restaurant hide their raw data behind methods
      - Inheritance           -> PaymentMethod hierarchy, User -> subclasses (not shown to keep focus, but structure allows it)
      - Abstraction           -> PaymentMethod & DeliveryPartner are abstract interfaces
      - Runtime Polymorphism  -> Order processes payment via base PaymentMethod*
      - Compile-time Poly.    -> operator<< overload for pretty-printing Order
      - Static members        -> Order::nextOrderId generates unique IDs across ALL orders
      - Friend function       -> OrderAuditor can inspect Order internals
      - `this` pointer        -> method chaining in OrderBuilder
      - Constructors/Dtors    -> proper resource setup/cleanup throughout
      - Smart pointers        -> modern C++ memory safety (no raw new/delete leaks)

    Compile: g++ -std=c++17 food_delivery_system.cpp -o food_delivery
    Run:     ./food_delivery
    Debug:   g++ -std=c++17 -g food_delivery_system.cpp -o food_delivery_debug
             gdb ./food_delivery_debug
*/

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <map>
using namespace std;

// ===========================================================
// ABSTRACTION: PaymentMethod interface (see 05_abstraction)
// ===========================================================
class PaymentMethod {
public:
    virtual bool pay(double amount) = 0;          // pure virtual -> forces override
    virtual string name() const = 0;
    virtual ~PaymentMethod() = default;            // virtual dtor (see 04_polymorphism)
};

class UpiPayment : public PaymentMethod {
    string upiId;
public:
    UpiPayment(string id) : upiId(move(id)) {}
    bool pay(double amount) override {
        cout << "  [UPI] Paid Rs." << amount << " from " << upiId << endl;
        return true;
    }
    string name() const override { return "UPI"; }
};

class CardPayment : public PaymentMethod {
    string maskedCard;
public:
    CardPayment(string card) : maskedCard(move(card)) {}
    bool pay(double amount) override {
        cout << "  [Card] Charged Rs." << amount << " to " << maskedCard << endl;
        return true;
    }
    string name() const override { return "Card"; }
};

// ===========================================================
// ENCAPSULATION: MenuItem & Restaurant hide details, expose safe methods
// ===========================================================
class MenuItem {
private:
    string itemName;
    double price;
    bool available;

public:
    MenuItem(string name, double p, bool avail = true)
        : itemName(move(name)), price(p), available(avail) {}

    string getName() const { return itemName; }
    double getPrice() const { return price; }
    bool isAvailable() const { return available; }

    void setAvailability(bool avail) { available = avail; }  // controlled mutation
};

class Restaurant {
private:
    string restaurantName;
    vector<MenuItem> menu;      // Restaurant OWNS its menu items (composition - "HAS-A")

public:
    Restaurant(string name) : restaurantName(move(name)) {}

    void addMenuItem(const MenuItem &item) {
        menu.push_back(item);
    }

    const vector<MenuItem>& getMenu() const { return menu; }
    string getName() const { return restaurantName; }

    // Finds an item by name; returns nullptr if not found or unavailable.
    const MenuItem* findItem(const string &name) const {
        for (const auto &item : menu) {
            if (item.getName() == name && item.isAvailable()) {
                return &item;
            }
        }
        return nullptr;
    }
};

// ===========================================================
// ABSTRACTION + RUNTIME POLYMORPHISM: DeliveryPartner hierarchy
// ===========================================================
class DeliveryPartner {
protected:
    string partnerName;
    double rating;

public:
    DeliveryPartner(string name, double r) : partnerName(move(name)), rating(r) {}

    virtual double estimateDeliveryTimeMinutes(double distanceKm) const = 0;

    string getName() const { return partnerName; }
    double getRating() const { return rating; }

    virtual ~DeliveryPartner() = default;
};

// Different vehicle types give different speed -> different overrides
// (Runtime polymorphism: exact same interface call, different result per type)
class BikePartner : public DeliveryPartner {
public:
    BikePartner(string name, double r) : DeliveryPartner(move(name), r) {}
    double estimateDeliveryTimeMinutes(double distanceKm) const override {
        const double bikeSpeedKmph = 30.0;
        return (distanceKm / bikeSpeedKmph) * 60.0;
    }
};

class BicyclePartner : public DeliveryPartner {
public:
    BicyclePartner(string name, double r) : DeliveryPartner(move(name), r) {}
    double estimateDeliveryTimeMinutes(double distanceKm) const override {
        const double bicycleSpeedKmph = 12.0;
        return (distanceKm / bicycleSpeedKmph) * 60.0;
    }
};

// ===========================================================
// Order class - the central entity. Uses STATIC members for auto-incrementing
// IDs (see 06_advanced/static_members.cpp) and FRIEND for auditing (see friend_function.cpp)
// ===========================================================
class OrderAuditor;  // forward declare for friend

class Order {
private:
    int orderId;
    string restaurantName;
    vector<pair<string, double>> items;   // (itemName, price) snapshot at order time
    double totalAmount;
    unique_ptr<PaymentMethod> paymentMethod;   // Order OWNS the payment method used
    bool paid;

    static int nextOrderId;   // shared across ALL Order objects - generates unique IDs

    // Only OrderAuditor can peek at raw internals for a "support/admin" view,
    // without us exposing public getters for everything.
    friend class OrderAuditor;

public:
    Order(string restName) : restaurantName(move(restName)), totalAmount(0), paid(false) {
        orderId = ++nextOrderId;   // pre-increment: first order gets ID 1
    }

    void addItem(const MenuItem &item) {
        items.push_back({item.getName(), item.getPrice()});
        totalAmount += item.getPrice();
    }

    // Accepts ANY PaymentMethod (base class pointer) - this function doesn't
    // care if it's UPI or Card - RUNTIME POLYMORPHISM in action.
    bool checkout(unique_ptr<PaymentMethod> method) {
        paymentMethod = move(method);
        paid = paymentMethod->pay(totalAmount);
        return paid;
    }

    int getId() const { return orderId; }
    double getTotal() const { return totalAmount; }
    bool isPaid() const { return paid; }

    // COMPILE-TIME POLYMORPHISM: operator<< overload for clean printing.
    // Must be a friend because left operand is ostream, not Order.
    friend ostream& operator<<(ostream &out, const Order &o) {
        out << "Order #" << o.orderId << " from " << o.restaurantName << ":\n";
        for (const auto &[name, price] : o.items) {
            out << "   - " << name << " : Rs." << price << "\n";
        }
        out << "  Total: Rs." << o.totalAmount
            << " | Paid: " << (o.paid ? "Yes" : "No")
            << (o.paid ? (" via " + o.paymentMethod->name()) : "") << "\n";
        return out;
    }
};

int Order::nextOrderId = 0;   // static member definition (see 06_advanced)

// A friend class that can access Order's private members directly, e.g.
// for an internal admin/support dashboard that needs raw data access
// without polluting Order's public API with getters nobody else needs.
class OrderAuditor {
public:
    void auditRawItems(const Order &o) const {
        cout << "[AUDIT] Order #" << o.orderId << " raw item count: "
             << o.items.size() << " | restaurant=" << o.restaurantName << endl;
    }
};

// ===========================================================
// THIS POINTER: Fluent OrderBuilder (method chaining) to assemble an order
// ===========================================================
class OrderBuilder {
private:
    unique_ptr<Order> order;
    const Restaurant *restaurant;

public:
    OrderBuilder(const Restaurant &r) : restaurant(&r) {
        order = make_unique<Order>(r.getName());
    }

    // Each method returns *this so calls can be chained fluently.
    OrderBuilder& add(const string &itemName) {
        const MenuItem *item = restaurant->findItem(itemName);
        if (item) {
            order->addItem(*item);
        } else {
            cout << "  (warning) '" << itemName << "' not available at "
                 << restaurant->getName() << ", skipping.\n";
        }
        return *this;   // `this` pointer dereferenced -> enables chaining
    }

    unique_ptr<Order> build() {
        return move(order);
    }
};

int main() {
    cout << "======================================\n";
    cout << "   MINI FOOD DELIVERY SYSTEM DEMO\n";
    cout << "======================================\n\n";

    // --- Set up a restaurant and its menu (Encapsulation + Composition) ---
    Restaurant biryaniHouse("Biryani House");
    biryaniHouse.addMenuItem(MenuItem("Chicken Biryani", 250));
    biryaniHouse.addMenuItem(MenuItem("Veg Biryani", 200));
    biryaniHouse.addMenuItem(MenuItem("Raita", 40));
    biryaniHouse.addMenuItem(MenuItem("Gulab Jamun", 60, false)); // currently unavailable

    // --- Build an order fluently using `this`-based chaining ---
    cout << "--- Building order #1 ---\n";
    unique_ptr<Order> order1 = OrderBuilder(biryaniHouse)
                                    .add("Chicken Biryani")
                                    .add("Raita")
                                    .add("Gulab Jamun")   // will warn: unavailable
                                    .build();

    cout << *order1;   // uses operator<< overload

    // --- Checkout using runtime polymorphism (any PaymentMethod works) ---
    cout << "\n--- Checking out order #1 with UPI ---\n";
    order1->checkout(make_unique<UpiPayment>("aditi@okhdfc"));
    cout << *order1;

    // --- Second order, paid with a card, to show static ID counter incrementing ---
    cout << "\n--- Building & paying order #2 ---\n";
    unique_ptr<Order> order2 = OrderBuilder(biryaniHouse)
                                    .add("Veg Biryani")
                                    .build();
    order2->checkout(make_unique<CardPayment>("**** **** **** 4242"));
    cout << *order2;

    // --- Delivery partner assignment (runtime polymorphism on distance calc) ---
    cout << "\n--- Assigning delivery partners ---\n";
    vector<unique_ptr<DeliveryPartner>> partners;
    partners.push_back(make_unique<BikePartner>("Rahul", 4.7));
    partners.push_back(make_unique<BicyclePartner>("Sameer", 4.5));

    double distanceToCustomer = 6.0; // km
    for (const auto &partner : partners) {
        // Same method call, DIFFERENT result per concrete type -> runtime polymorphism
        double eta = partner->estimateDeliveryTimeMinutes(distanceToCustomer);
        cout << partner->getName() << " (rating " << partner->getRating()
             << ") ETA for " << distanceToCustomer << "km: " << eta << " minutes\n";
    }

    // --- Friend class usage: admin auditing without exposing public getters ---
    cout << "\n--- Admin audit (friend class access) ---\n";
    OrderAuditor auditor;
    auditor.auditRawItems(*order1);
    auditor.auditRawItems(*order2);

    cout << "\nTotal orders placed so far (static counter): "
         << order2->getId() << endl;   // order2 was the 2nd order created

    return 0;
}

/*
    WHERE TO GO NEXT (bridge into System Design):
    ------------------------------------------------
    This single-process demo already contains the SEEDS of real system
    design questions you'll study next:
      - "How would you make Restaurant/Order persist in a database?"
           -> repository pattern, ORM
      - "How would 10,000 orders/sec be handled?"
           -> load balancing, message queues (Kafka), horizontal scaling
      - "How do you avoid double-charging a customer?"
           -> idempotency keys, distributed transactions
      - "How do you assign the NEAREST delivery partner?"
           -> geospatial indexing (quad-trees / geohashing), matching algorithms
      - "PaymentMethod abstraction" you built here IS the Strategy pattern;
        "OrderBuilder" IS the Builder pattern - both are foundational
        Design Patterns you'll formalize before/alongside System Design.

    Solid OOP fundamentals (this whole folder) + Design Patterns + basic
    System Design building blocks (caching, databases, queues, load
    balancers) = a complete SDE-1 interview prep pipeline.
*/
