/*
    FILE: 02_runtime_polymorphism.cpp
    TOPIC: Runtime (Dynamic) Polymorphism - THE most asked OOP topic in interviews

    THEORY:
    --------------------------------
    Runtime polymorphism lets code call a function on a BASE CLASS POINTER/
    REFERENCE, and the ACTUAL function that runs is decided at RUNTIME based
    on the real (derived) object type. Achieved using:
        - virtual functions
        - function overriding (same signature in derived class)
        - base class pointers/references

    HOW IT WORKS UNDER THE HOOD (the classic follow-up question):
    - Every class with at least one virtual function gets a hidden
      "vptr" (virtual pointer) added to each object.
    - The vptr points to a "vtable" (virtual table) - an array of function
      pointers, one per virtual function, specific to that class.
    - When you call `basePtr->speak()`, the compiler generates code that:
        1. Follows basePtr's vptr to find the correct vtable
        2. Looks up speak()'s slot in that vtable
        3. Calls whatever function pointer is stored there (the derived
           class's override)
      This lookup happens at RUNTIME - hence "runtime polymorphism".

    PURE VIRTUAL FUNCTIONS & ABSTRACT CLASSES:
    - `virtual void speak() = 0;` -> pure virtual function. A class with
      ANY pure virtual function becomes an ABSTRACT CLASS - you CANNOT
      instantiate it directly. It's used purely as an interface/base
      that forces derived classes to provide their own implementation.
      (Deep dive in 05_abstraction/abstraction.cpp)

    REAL EXAMPLE: A Shape hierarchy (extremely common interview question:
    "Design a Shape class with Circle, Rectangle, Triangle computing area/perimeter")

    Compile: g++ -std=c++17 02_runtime_polymorphism.cpp -o runtime_poly
    Run:     ./runtime_poly
*/

#include <iostream>
#include <vector>
#include <memory>     // for smart pointers (unique_ptr) - modern C++, avoids leaks
using namespace std;

// ===========================================================
// BASE CLASS with virtual functions
// ===========================================================
class Shape {
public:
    // virtual -> tells compiler "derived classes may override this;
    // decide which version to call at RUNTIME based on actual object type"
    virtual double area() const {
        return 0.0;   // default implementation (can be overridden)
    }

    virtual double perimeter() const {
        return 0.0;
    }

    // Pure virtual-ish usage pattern: even non-pure virtuals should be
    // overridden; we'll show a proper pure virtual example separately.
    virtual void describe() const {
        cout << "A generic shape with area=" << area()
             << " and perimeter=" << perimeter() << endl;
    }

    // *** CRITICAL RULE ***
    // ALWAYS make the base class destructor virtual if you intend to
    // delete derived objects through a Base* pointer. Without `virtual`
    // here, `delete basePtr;` would only call ~Shape() and SKIP the
    // derived class destructor -> resource leak (if derived class
    // allocated heap memory) - a very common interview trap question.
    virtual ~Shape() {
        cout << "Shape destructor\n";
    }
};

// ===========================================================
// DERIVED CLASSES - each OVERRIDES area()/perimeter()/describe()
// ===========================================================
class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : radius(r) {}

    // `override` keyword (C++11+) is optional but HIGHLY recommended:
    // the compiler will error out if this doesn't actually match a
    // virtual function signature in the base class - catches typos.
    double area() const override {
        return 3.14159265 * radius * radius;
    }

    double perimeter() const override {
        return 2 * 3.14159265 * radius;
    }

    void describe() const override {
        cout << "Circle(r=" << radius << ") -> area=" << area()
             << ", perimeter=" << perimeter() << endl;
    }

    ~Circle() override {
        cout << "Circle destructor\n";
    }
};

class Rectangle : public Shape {
private:
    double length, width;
public:
    Rectangle(double l, double w) : length(l), width(w) {}

    double area() const override {
        return length * width;
    }

    double perimeter() const override {
        return 2 * (length + width);
    }

    void describe() const override {
        cout << "Rectangle(" << length << "x" << width << ") -> area=" << area()
             << ", perimeter=" << perimeter() << endl;
    }

    ~Rectangle() override {
        cout << "Rectangle destructor\n";
    }
};

class Triangle : public Shape {
private:
    double base, height, a, b, c;   // a,b,c = side lengths for perimeter
public:
    Triangle(double base, double height, double a, double b, double c)
        : base(base), height(height), a(a), b(b), c(c) {}

    double area() const override {
        return 0.5 * base * height;
    }

    double perimeter() const override {
        return a + b + c;
    }

    void describe() const override {
        cout << "Triangle(base=" << base << ", height=" << height
             << ") -> area=" << area() << ", perimeter=" << perimeter() << endl;
    }

    ~Triangle() override {
        cout << "Triangle destructor\n";
    }
};

// A function that works with ANY shape - the heart of polymorphism.
// It doesn't know or care if it's a Circle, Rectangle, or Triangle.
void printShapeInfo(const Shape &s) {
    s.describe();   // virtual dispatch happens here at RUNTIME
}

int main() {
    cout << "--- Runtime Polymorphism via base class pointers ---\n";

    // Using smart pointers (unique_ptr) instead of raw `new`/`delete`.
    // vector<Shape*> would also work, but unique_ptr auto-releases memory
    // (RAII) - modern, interview-preferred C++ style.
    vector<unique_ptr<Shape>> shapes;
    shapes.push_back(make_unique<Circle>(5));
    shapes.push_back(make_unique<Rectangle>(4, 6));
    shapes.push_back(make_unique<Triangle>(6, 4, 5, 5, 6));

    double totalArea = 0;
    for (const auto &shapePtr : shapes) {
        // shapePtr is declared as unique_ptr<Shape>, but at runtime it
        // actually points to a Circle/Rectangle/Triangle object.
        // Calling ->describe() triggers VIRTUAL DISPATCH: the vtable
        // lookup picks the correct override for the REAL object type.
        shapePtr->describe();
        totalArea += shapePtr->area();
    }

    cout << "\nTotal area of all shapes: " << totalArea << endl;

    cout << "\n--- Passing by reference also uses dynamic dispatch ---\n";
    Circle c(3);
    printShapeInfo(c);   // 'Shape&' parameter, but Circle's describe() runs

    cout << "\n--- Watch destructors fire correctly (virtual destructor) ---\n";
    // when `shapes` vector goes out of scope at end of main, each
    // unique_ptr calls `delete` on its Shape*, which - because ~Shape()
    // is virtual - correctly calls Circle::~Circle(), Rectangle::~Rectangle()
    // etc. FIRST, then ~Shape(). If ~Shape() were NOT virtual, only
    // ~Shape() would run for each, leaking any derived-class resources.

    return 0;
}

/*
    INTERVIEW GOTCHAS:
    1. "What happens if you delete a derived object through a base pointer
        with a NON-virtual destructor?" -> Undefined behavior / resource leak,
        because only the base destructor runs.
    2. "Function overriding vs overloading?"
         - Overriding: same name+signature, DIFFERENT classes (base/derived),
           resolved at RUNTIME via vtable.
         - Overloading: same name, DIFFERENT parameters, SAME class,
           resolved at COMPILE time.
    3. "What is the size overhead of making a class polymorphic?"
         -> Each object gets one hidden vptr (typically 8 bytes on 64-bit
            systems), and there's one vtable per class (not per object).
*/
