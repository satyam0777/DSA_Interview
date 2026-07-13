/*
    FILE: 01_static_members.cpp
    TOPIC: Static Members (variables & functions)

    THEORY:
    --------------------------------
    - A `static` member VARIABLE is SHARED across ALL objects of a class.
      There's only ONE copy of it in memory, no matter how many objects
      you create. Great for counters, shared config, singletons, etc.
    - A `static` member FUNCTION:
        - Can be called WITHOUT creating an object: ClassName::function()
        - Can ONLY access other static members (it has no `this` pointer,
          because it isn't tied to any specific object).
    - Static members must be DEFINED once outside the class (unless they're
      `inline static` / `constexpr`, a C++17 feature).

    REAL EXAMPLE: A connection pool counter (very common in backend systems
    - tracking active DB connections, active user sessions, etc.)

    Compile: g++ -std=c++17 01_static_members.cpp -o static_members
    Run:     ./static_members
*/

#include <iostream>
using namespace std;

class DatabaseConnection {
private:
    int connectionId;

    // static -> ONE shared counter for the WHOLE class, not per-object.
    static int activeConnections;
    static const int MAX_CONNECTIONS = 5;   // static const -> compile-time constant

public:
    DatabaseConnection() {
        if (activeConnections >= MAX_CONNECTIONS) {
            throw runtime_error("Connection pool exhausted! Max: " + to_string(MAX_CONNECTIONS));
        }
        activeConnections++;
        connectionId = activeConnections;
        cout << "Connection #" << connectionId << " opened. Active: " << activeConnections << endl;
    }

    ~DatabaseConnection() {
        activeConnections--;
        cout << "Connection #" << connectionId << " closed. Active: " << activeConnections << endl;
    }

    // Static function - called on the CLASS, not an object.
    // Notice it can access `activeConnections` (a static var) but could NOT
    // access something like `connectionId` (a per-object/instance variable),
    // because static functions have no `this` and aren't tied to one object.
    static int getActiveConnectionCount() {
        return activeConnections;
    }
};

// Definition of the static member - required ONCE outside the class,
// otherwise you'll get a linker error ("undefined reference").
int DatabaseConnection::activeConnections = 0;

int main() {
    // Calling a static function WITHOUT any object -> ClassName::function()
    cout << "Active connections before any object: "
         << DatabaseConnection::getActiveConnectionCount() << endl;

    {
        DatabaseConnection c1;
        DatabaseConnection c2;
        DatabaseConnection c3;

        // All three objects SHARE the same `activeConnections` counter.
        cout << "Active connections now: "
             << DatabaseConnection::getActiveConnectionCount() << endl;

    } // c1, c2, c3 destructors fire here (reverse order) -> counter decrements

    cout << "Active connections after scope ends: "
         << DatabaseConnection::getActiveConnectionCount() << endl;

    // Demonstrate the pool limit being enforced via a static counter
    try {
        DatabaseConnection conns[6];   // 6th one exceeds MAX_CONNECTIONS(5) -> throws
    } catch (const exception &ex) {
        cout << "Error: " << ex.what() << endl;
    }

    return 0;
}

/*
    INTERVIEW TALKING POINT:
    "Static members model 'class-level' state/behavior, as opposed to
    'object-level' state. A classic real use-case is a Singleton pattern
    (exactly ONE instance of a class allowed app-wide) - implemented using
    a static instance pointer + a private constructor. You'll meet
    Singleton formally in System Design as a creational design pattern."
*/
