/*
    FILE: 01_compile_time_polymorphism.cpp
    TOPIC: Compile-Time (Static) Polymorphism

    THEORY:
    --------------------------------
    "Poly-morphism" = many forms. Compile-time polymorphism means the
    compiler decides WHICH function/operator to call at COMPILE time,
    based on the number/type of arguments. Achieved via:
      1) Function Overloading   - same name, different parameter lists
      2) Operator Overloading   - redefine what +, -, ==, << etc. do for
                                   your own class
      3) Templates (generic programming - brief mention)

    This is different from runtime polymorphism (virtual functions),
    which is resolved at RUN time via the vtable - see 02_runtime_polymorphism.cpp

    REAL EXAMPLE: A Vector2D math class (used heavily in games/graphics/physics
    engines, and a common interview ask: "overload operators for a Point/Vector class")

    Compile: g++ -std=c++17 01_compile_time_polymorphism.cpp -o compile_time_poly
    Run:     ./compile_time_poly
*/

#include <iostream>
using namespace std;

// ===========================================================
// PART A: FUNCTION OVERLOADING
// ===========================================================
class Calculator {
public:
    // Same function name `add`, different parameter types/counts.
    // Compiler picks the right one based on arguments passed - resolved
    // at COMPILE time (no runtime cost/decision).
    int add(int a, int b) {
        return a + b;
    }

    double add(double a, double b) {
        return a + b;
    }

    int add(int a, int b, int c) {
        return a + b + c;
    }

    // Overloading also works with different argument TYPES entirely
    string add(string a, string b) {
        return a + b;   // string concatenation
    }
};

// ===========================================================
// PART B: OPERATOR OVERLOADING (very common interview question)
// ===========================================================
class Vector2D {
public:
    double x, y;

    Vector2D(double x = 0, double y = 0) : x(x), y(y) {}

    // Overload '+' so we can write: v3 = v1 + v2  (instead of a method call)
    Vector2D operator+(const Vector2D &other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    // Overload '-'
    Vector2D operator-(const Vector2D &other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    // Overload '*' for scalar multiplication: v2 = v1 * 3.0
    Vector2D operator*(double scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    // Overload '==' for value comparison instead of pointer comparison
    bool operator==(const Vector2D &other) const {
        return x == other.x && y == other.y;
    }

    // Overload '<<' so we can do: cout << myVector;
    // Must be a `friend` (or free function) because the LEFT operand is
    // an ostream (cout), not a Vector2D - see 06_advanced/friend_function.cpp
    friend ostream& operator<<(ostream &out, const Vector2D &v) {
        out << "(" << v.x << ", " << v.y << ")";
        return out;
    }
};

int main() {
    cout << "--- Function Overloading ---\n";
    Calculator calc;
    cout << "int add:    " << calc.add(2, 3) << endl;
    cout << "double add: " << calc.add(2.5, 3.5) << endl;
    cout << "3-arg add:  " << calc.add(1, 2, 3) << endl;
    cout << "string add: " << calc.add(string("Hello, "), string("World!")) << endl;

    cout << "\n--- Operator Overloading ---\n";
    Vector2D v1(2, 3);
    Vector2D v2(4, 1);

    Vector2D v3 = v1 + v2;     // calls operator+
    Vector2D v4 = v1 - v2;     // calls operator-
    Vector2D v5 = v1 * 2.0;    // calls operator*

    cout << "v1 = " << v1 << endl;
    cout << "v2 = " << v2 << endl;
    cout << "v1 + v2 = " << v3 << endl;
    cout << "v1 - v2 = " << v4 << endl;
    cout << "v1 * 2  = " << v5 << endl;

    cout << "v1 == v2? " << (v1 == v2 ? "true" : "false") << endl;

    return 0;
}

/*
    INTERVIEW TALKING POINT:
    "Compile-time polymorphism has ZERO runtime overhead - the compiler
    resolves everything during compilation (name mangling picks the right
    overload). Runtime polymorphism (virtual functions) has a small cost
    (vtable pointer lookup) but gives you flexibility to decide behavior
    based on the ACTUAL object type at runtime, which is essential for things
    like plugin systems, or processing a list of `Shape*` where each element
    might really be a Circle, Square, etc."
*/
