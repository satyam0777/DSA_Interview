/*
    FILE: 03_this_pointer.cpp
    TOPIC: The `this` pointer

    THEORY:
    --------------------------------
    - `this` is an IMPLICIT pointer available inside every non-static member
      function. It points to the CURRENT object the method was called on.
    - Type of `this` inside a member function of class X is `X* const`
      (a const pointer to X - meaning you can't reassign `this` itself,
      but you CAN modify the object it points to, unless the method is
      also `const`, in which case `this` is `const X* const`).
    - Common uses:
        1. Disambiguate between a member variable and a parameter with
           the SAME name (e.g., constructor parameter `name` vs member `name`)
        2. Return `*this` (dereferenced) to enable METHOD CHAINING
           (fluent interfaces, e.g., `builder.setX(1).setY(2).build();`)
        3. Compare if two pointers refer to the SAME object (`this == &other`)

    REAL EXAMPLE: A fluent "QueryBuilder" (method chaining) - a pattern
    used constantly in real backend code (think: building SQL queries,
    or configuring HTTP requests step by step).

    Compile: g++ -std=c++17 03_this_pointer.cpp -o this_pointer
    Run:     ./this_pointer
*/

#include <iostream>
#include <string>
using namespace std;

// ===========================================================
// PART A: `this` to disambiguate variable names
// ===========================================================
class Person {
private:
    string name;
    int age;

public:
    // Parameter names are DELIBERATELY the same as member names.
    // Without `this->`, `name = name;` would just assign the parameter
    // to itself and never touch the member - a classic beginner bug.
    Person(string name, int age) {
        this->name = name;   // this->name = the MEMBER, name = the PARAMETER
        this->age = age;
    }

    void printInfo() const {
        cout << this->name << " is " << this->age << " years old\n";
    }

    // Using `this` to compare identity (are these literally the SAME object?)
    bool isSameObject(const Person &other) const {
        return this == &other;   // compares memory addresses
    }
};

// ===========================================================
// PART B: `this` for method chaining (fluent interface)
// Real, practical pattern used in query builders, HTTP request
// builders, UI layout builders, etc.
// ===========================================================
class SqlQueryBuilder {
private:
    string table;
    string whereClause;
    string orderByClause;
    int limitValue = -1;

public:
    // Each method modifies state, then returns `*this` (a reference to the
    // CURRENT object) so the next method can be called immediately on the
    // same object -> enables chaining: builder.from().where().limit()
    SqlQueryBuilder& from(string tableName) {
        this->table = tableName;
        return *this;   // return the current object BY REFERENCE
    }

    SqlQueryBuilder& where(string condition) {
        this->whereClause = condition;
        return *this;
    }

    SqlQueryBuilder& orderBy(string column) {
        this->orderByClause = column;
        return *this;
    }

    SqlQueryBuilder& limit(int n) {
        this->limitValue = n;
        return *this;
    }

    string build() const {
        string query = "SELECT * FROM " + table;
        if (!whereClause.empty()) query += " WHERE " + whereClause;
        if (!orderByClause.empty()) query += " ORDER BY " + orderByClause;
        if (limitValue != -1) query += " LIMIT " + to_string(limitValue);
        return query;
    }
};

int main() {
    cout << "--- `this` to disambiguate names ---\n";
    Person p1("Aditi", 24);
    Person p2("Rahul", 27);
    p1.printInfo();
    p2.printInfo();
    cout << "p1 same object as p2? " << (p1.isSameObject(p2) ? "yes" : "no") << endl;
    cout << "p1 same object as itself? " << (p1.isSameObject(p1) ? "yes" : "no") << endl;

    cout << "\n--- `this` enabling method chaining ---\n";
    SqlQueryBuilder qb;
    // Each call returns *this, so we can chain calls in ONE fluent statement:
    string query = qb.from("employees")
                      .where("salary > 50000")
                      .orderBy("salary DESC")
                      .limit(10)
                      .build();

    cout << "Generated query:\n" << query << endl;

    return 0;
}

/*
    INTERVIEW TALKING POINT:
    "Method chaining via `this` is the backbone of the Builder Design
    Pattern, which you'll formalize in System Design when constructing
    complex objects step-by-step (e.g., building an HTTP request, or an
    immutable config object) without a giant constructor with 10 parameters."
*/
