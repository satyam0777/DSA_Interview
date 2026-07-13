/*
    FILE: inheritance_types.cpp
    TOPIC: Inheritance (all major types)

    THEORY:
    --------------------------------
    Inheritance lets a class (derived/child) reuse and extend the data
    and behavior of another class (base/parent). It models an "IS-A"
    relationship (Car IS-A Vehicle).

    Access specifier during inheritance (`class Derived : public Base`)
    controls how Base's members are seen inside Derived:
      public inheritance    -> public stays public, protected stays protected (MOST COMMON)
      protected inheritance -> public & protected members become protected
      private inheritance   -> public & protected members become private
    (99% of interview/real code uses `public` inheritance.)

    TYPES OF INHERITANCE:
      1. Single       : one base  -> one derived
      2. Multilevel   : A -> B -> C (chain)
      3. Hierarchical : one base  -> many derived
      4. Multiple     : derived from 2+ base classes
      5. Hybrid       : combination of above (often causes the Diamond Problem)

    Compile: g++ -std=c++17 inheritance_types.cpp -o inheritance
    Run:     ./inheritance
*/

#include <iostream>
#include <string>
using namespace std;

// ===========================================================
// 1) SINGLE INHERITANCE : Vehicle -> Car
// ===========================================================
class Vehicle {
protected:                      // protected -> visible to derived classes, not outside
    string brand;
    int topSpeed;

public:
    Vehicle(string b, int speed) : brand(b), topSpeed(speed) {}

    void honk() const {
        cout << brand << " goes Beep Beep!\n";
    }
};

class Car : public Vehicle {    // Car IS-A Vehicle
private:
    int numDoors;

public:
    Car(string b, int speed, int doors)
        : Vehicle(b, speed),    // must call base ctor explicitly to init base part
          numDoors(doors) {}

    void displayInfo() const {
        // brand & topSpeed are inherited from Vehicle (protected -> accessible here)
        cout << brand << " | Top speed: " << topSpeed
             << "km/h | Doors: " << numDoors << endl;
    }
};

// ===========================================================
// 2) MULTILEVEL INHERITANCE : Vehicle -> Car -> ElectricCar
// ===========================================================
class ElectricCar : public Car {
private:
    int batteryRangeKm;

public:
    ElectricCar(string b, int speed, int doors, int range)
        : Car(b, speed, doors), batteryRangeKm(range) {}

    void displayBattery() const {
        cout << brand << " has a range of " << batteryRangeKm << " km\n";
    }
};

// ===========================================================
// 3) HIERARCHICAL INHERITANCE : Vehicle -> {Car, Bike}
// ===========================================================
class Bike : public Vehicle {
private:
    bool hasGear;

public:
    Bike(string b, int speed, bool gear) : Vehicle(b, speed), hasGear(gear) {}

    void displayInfo() const {
        cout << brand << " | Top speed: " << topSpeed
             << "km/h | Has gear: " << (hasGear ? "Yes" : "No") << endl;
    }
};

// ===========================================================
// 4) MULTIPLE INHERITANCE : a class derived from 2+ bases
//    Real example: an AmphibiousVehicle needs behavior from BOTH
//    a "Drivable" thing and a "Floatable" thing.
// ===========================================================
class Drivable {
public:
    void drive() const { cout << "Driving on road...\n"; }
};

class Floatable {
public:
    void floatOnWater() const { cout << "Floating on water...\n"; }
};

// AmphibiousCar inherits from BOTH Drivable and Floatable
class AmphibiousCar : public Drivable, public Floatable {
public:
    void demo() const {
        drive();
        floatOnWater();
    }
};

int main() {
    cout << "--- Single Inheritance ---\n";
    Car myCar("Toyota", 180, 4);
    myCar.honk();          // inherited from Vehicle
    myCar.displayInfo();   // defined in Car

    cout << "\n--- Multilevel Inheritance ---\n";
    ElectricCar tesla("Tesla", 250, 4, 500);
    tesla.honk();            // from Vehicle (grandparent)
    tesla.displayInfo();     // from Car (parent)
    tesla.displayBattery();  // from ElectricCar (self)

    cout << "\n--- Hierarchical Inheritance ---\n";
    Bike myBike("Royal Enfield", 120, true);
    myBike.honk();          // inherited from same base as Car
    myBike.displayInfo();

    cout << "\n--- Multiple Inheritance ---\n";
    AmphibiousCar amphi;
    amphi.demo();            // uses methods from 2 unrelated base classes

    return 0;
}

/*
    INTERVIEW GOTCHA - THE DIAMOND PROBLEM (Hybrid inheritance):
    If class D inherits from B and C, and both B and C inherit from A,
    then D ends up with TWO copies of A's members -> ambiguous access.
    Fixed using VIRTUAL INHERITANCE:

        class B : virtual public A {};
        class C : virtual public A {};
        class D : public B, public C {};   // now only ONE copy of A exists in D

    This is a classic follow-up question after discussing multiple inheritance.
*/
