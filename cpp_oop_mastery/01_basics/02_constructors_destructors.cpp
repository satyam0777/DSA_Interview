/*
    FILE: 02_constructors_destructors.cpp
    TOPIC: Constructors & Destructors

    THEORY:
    --------------------------------
    - CONSTRUCTOR: special method, same name as class, NO return type.
      Called automatically when an object is created. Used to initialize state.
      Types:
        1) Default constructor        -> takes no arguments
        2) Parameterized constructor  -> takes arguments to init with custom values
        3) Copy constructor           -> initializes an object using another
                                          object of the same class
    - DESTRUCTOR: ~ClassName(), no args, no return type, called automatically
      when object goes out of scope / is deleted. Used to release resources
      (memory, file handles, network sockets, DB connections, etc.)
    - CONSTRUCTOR OVERLOADING: you can have multiple constructors with
      different signatures (this is compile-time polymorphism, see folder 04).
    - INITIALIZER LIST (member init list) is the preferred way to init
      members - it's more efficient than assigning inside the constructor body,
      especially for const members, references, and objects without a
      default constructor.

    Compile: g++ -std=c++17 02_constructors_destructors.cpp -o ctor_dtor
    Run:     ./ctor_dtor
*/

#include <iostream>
using namespace std;

class Rectangle {
private:
    double length;
    double width;

public:
    // 1) DEFAULT CONSTRUCTOR
    Rectangle() {
        length = 1.0;
        width = 1.0;
        cout << "[Default Ctor] Rectangle created with 1x1\n";
    }

    // 2) PARAMETERIZED CONSTRUCTOR
    // Using a member-initializer-list: `: length(l), width(w)`
    // This directly initializes the members (faster & required for
    // const/reference members) instead of default-constructing then assigning.
    Rectangle(double l, double w) : length(l), width(w) {
        cout << "[Param Ctor] Rectangle created with " << l << "x" << w << "\n";
    }

    // 3) COPY CONSTRUCTOR
    // Called when a NEW object is created FROM an existing object, e.g.:
    //     Rectangle r2 = r1;   OR   Rectangle r2(r1);
    // Default (compiler generated) copy ctor does a member-wise copy,
    // which is fine here since we have no pointers/dynamic memory.
    // We write it explicitly just to show WHERE it fires.
    Rectangle(const Rectangle &other) : length(other.length), width(other.width) {
        cout << "[Copy Ctor] Copied a rectangle -> " << length << "x" << width << "\n";
    }

    double area() const { return length * width; }

    // DESTRUCTOR
    // In this simple example we hold no heap memory, so there's nothing to
    // manually free - but in real code (see BankAccount / real world project)
    // this is where you'd do `delete`, close files, release locks, etc.
    ~Rectangle() {
        cout << "[Dtor] Rectangle " << length << "x" << width << " destroyed\n";
    }
};

// A class demonstrating WHY destructors matter: manual heap memory.
class DynamicArray {
private:
    int *data;
    int size;

public:
    DynamicArray(int n) : size(n) {
        data = new int[size];              // manual heap allocation
        for (int i = 0; i < size; i++) data[i] = 0;
        cout << "[Ctor] Allocated array of size " << size << " on heap\n";
    }

    // Deep copy constructor - CRITICAL when a class owns raw pointers.
    // Without this, the compiler's default copy ctor would copy the POINTER
    // (shallow copy), causing two objects to point to the same memory ->
    // double free / dangling pointer bugs when both destructors run.
    DynamicArray(const DynamicArray &other) : size(other.size) {
        data = new int[size];
        for (int i = 0; i < size; i++) data[i] = other.data[i];
        cout << "[Copy Ctor] Deep-copied array of size " << size << "\n";
    }

    ~DynamicArray() {
        delete[] data;                     // release heap memory - avoids leak
        cout << "[Dtor] Freed heap array of size " << size << "\n";
    }
};

int main() {
    cout << "--- Constructors in action ---\n";
    Rectangle r1;                 // default ctor
    Rectangle r2(4.0, 5.0);       // parameterized ctor
    Rectangle r3 = r2;            // copy ctor (NOT assignment - r3 is being created)

    cout << "Area of r2: " << r2.area() << endl;

    cout << "\n--- Why deep copy matters ---\n";
    {
        DynamicArray a1(5);
        DynamicArray a2 = a1;   // deep copy ctor runs -> a2 has its OWN array
        // If we hadn't written a custom copy ctor, a1 and a2 would share
        // the same `data` pointer. When both go out of scope, `delete[]`
        // would run TWICE on the same memory -> undefined behavior (crash).
    } // <- a2 destroyed first (reverse order), then a1. Both free their OWN memory safely.

    cout << "\n--- End of main: watch destructors fire in REVERSE order of creation ---\n";
    return 0;
} // r3, r2, r1 destructors fire here in reverse order (r3 -> r2 -> r1)

/*
    INTERVIEW GOTCHAS:
    - Rule of Three: if you define ANY of (destructor, copy ctor, copy
      assignment operator), you almost certainly need to define all three,
      because the default ones won't handle owned resources correctly.
    - Destructors should generally be `virtual` in a base class if you plan
      to delete derived objects through a base pointer (see 04_polymorphism).
    - Objects are destroyed in REVERSE order of construction (stack unwind).
*/
