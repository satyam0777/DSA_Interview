/*
    FILE: 01_class_and_objects.cpp
    TOPIC: Classes & Objects (the foundation of OOP)

    THEORY (read this first):
    --------------------------------
    - A CLASS is a user-defined blueprint/template. It does not occupy memory
      by itself. It just describes what data (members) and behavior (methods)
      an object of that type will have.
    - An OBJECT is an actual instance of a class. Memory is allocated only
      when you create an object.
    - ACCESS SPECIFIERS control visibility of members:
        public    -> accessible from anywhere (outside the class too)
        private   -> accessible only inside the class (default for class)
        protected -> accessible inside the class AND its derived classes
    - By default, everything in a `class` is private.
      (In a `struct`, everything is public by default. That's the ONLY
       real difference between struct and class in C++.)

    Compile:  g++ -std=c++17 01_class_and_objects.cpp -o basics
    Run:      ./basics
*/

#include <iostream>
#include <string>
using namespace std;

// ---------------------------------------------------------
// A simple class representing an Employee (common interview example)
// ---------------------------------------------------------
class Employee {

private:
    // Private data members -> "state" of the object.
    // Kept private so nobody outside can directly corrupt them
    // (this is the seed of ENCAPSULATION, covered in detail in folder 02).
    int empId;
    string name;
    double salary;

public:
    // Public methods -> "behavior" of the object.
    // These are the only way the outside world interacts with our data.

    // A method to initialize object data (manual "setup" - constructors
    // do this automatically, see 02_constructors_destructors.cpp)
    void setDetails(int id, string empName, double sal) {
        empId = id;
        name = empName;
        salary = sal;
    }

    // A method to read data (like a "getter")
    void display() const {   // const -> promises this method won't modify the object
        cout << "Employee[" << empId << "] " << name
             << " | Salary: " << salary << endl;
    }

    // A method that changes state based on business logic
    void giveRaise(double percent) {
        salary += salary * (percent / 100.0);
    }

    double getSalary() const {
        return salary;
    }
};

int main() {
    // Creating OBJECTS (instances) of the Employee class.
    // Each object gets its OWN copy of empId, name, salary.
    Employee e1;
    Employee e2;

    // Calling public methods on each object using the dot operator.
    e1.setDetails(101, "Aditi Sharma", 55000);
    e2.setDetails(102, "Rahul Verma", 62000);

    e1.display();
    e2.display();

    // Business logic: give e1 a 10% raise
    e1.giveRaise(10);
    cout << "\nAfter raise:\n";
    e1.display();

    // NOTE: e1.empId = 999;  <-- This line would NOT compile.
    // Uncomment it to see the compiler error - proves 'empId' is private
    // and cannot be touched directly from main() / outside the class.

    cout << "\nDirect salary read via getter: " << e1.getSalary() << endl;

    return 0;
}

/*
    KEY TAKEAWAYS FOR INTERVIEWS:
    - Class = blueprint (compile-time concept, no memory).
    - Object = instance (runtime concept, occupies memory).
    - Access specifiers enforce data hiding, the base of encapsulation.
    - `this` (not used heavily here) refers to the current object -
      see 06_advanced/this_pointer.cpp for a deep dive.
*/
