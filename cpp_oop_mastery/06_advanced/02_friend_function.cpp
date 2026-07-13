/*
    FILE: 02_friend_function.cpp
    TOPIC: Friend Functions & Friend Classes

    THEORY:
    --------------------------------
    Normally, private members of a class can ONLY be accessed from within
    that class. A `friend` declaration grants a SPECIFIC external function
    or class special permission to access private/protected members.

    - friend function : a free (non-member) function, or a member function
                         of ANOTHER class, given access.
    - friend class     : an ENTIRE other class is given access to all
                         private/protected members.

    IMPORTANT NUANCES:
    - Friendship is NOT mutual. If A declares B as friend, B can access A's
      privates, but A CANNOT access B's privates unless B also declares A as friend.
    - Friendship is NOT inherited by derived classes.
    - Use sparingly! Overusing friend breaks encapsulation. It's meant for
      tightly-coupled helper cases (like operator<< overloads, or a
      Builder class that needs to construct a complex object piece by piece).

    REAL EXAMPLE: A Box class where a separate "BoxInspector" utility
    (imagine: a unit-testing/auditing tool) needs raw access to private
    dimensions without exposing public getters to everyone else.

    Compile: g++ -std=c++17 02_friend_function.cpp -o friend_demo
    Run:     ./friend_demo
*/

#include <iostream>
using namespace std;

class Box; // forward declaration needed because BoxInspector mentions Box before it's defined

// A free function that will be granted friendship
class BoxInspector {
public:
    void auditDimensions(const Box &b);   // declared here, defined after Box (needs Box's members)
};

class Box {
private:
    double length, width, height;

    // Only this specific free function can access Box's private members.
    friend double calculateVolumeDirectly(const Box &b);

    // Grant an ENTIRE class friendship - BoxInspector can now touch
    // length/width/height freely, as if they were its own.
    friend class BoxInspector;

public:
    Box(double l, double w, double h) : length(l), width(w), height(h) {}

    // Normal public way to get volume (the "proper" API)
    double volume() const {
        return length * width * height;
    }
};

// Free function - NOT a member of Box, yet can access Box's private data
// because it was declared `friend` inside Box.
double calculateVolumeDirectly(const Box &b) {
    // Accessing private members directly, bypassing volume()/getters.
    return b.length * b.width * b.height;
}

// Definition of BoxInspector's method - since BoxInspector is a friend
// class of Box, this method can read Box's private members.
void BoxInspector::auditDimensions(const Box &b) {
    cout << "[Audit] length=" << b.length << " width=" << b.width
         << " height=" << b.height << " (accessed via friend class)\n";
}

int main() {
    Box box(2.0, 3.0, 4.0);

    cout << "Volume via public API: " << box.volume() << endl;
    cout << "Volume via friend function: " << calculateVolumeDirectly(box) << endl;

    BoxInspector inspector;
    inspector.auditDimensions(box);   // friend class accessing privates

    // box.length;  <-- would NOT compile here in main() - main() is not a friend!
    // Friendship must be explicitly granted; it's not "if one function can, all can".

    return 0;
}

/*
    INTERVIEW TALKING POINT:
    "The most common REAL-WORLD use of `friend` is overloading operator<<
    for printing a custom object with cout, because the left-hand operand
    of << is an ostream, not your class - so it can't be a member function.
    You saw this already in 04_polymorphism/01_compile_time_polymorphism.cpp
    with `friend ostream& operator<<(...)` inside Vector2D."
*/
