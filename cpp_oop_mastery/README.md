# C++ OOP Mastery — SDE-1 Interview Prep

A hands-on, heavily-commented C++ project covering every core OOP concept
asked in SDE-1 interviews, ending with a real-world capstone project that
ties everything together and bridges into System Design.

Every `.cpp` file is **self-contained and runnable on its own** — compile
and run any single file to see that concept in isolation.

## Folder structure

```
cpp_oop_mastery/
├── 01_basics/
│   ├── 01_class_and_objects.cpp        # class vs object, access specifiers
│   └── 02_constructors_destructors.cpp # ctor types, dtor, deep vs shallow copy
├── 02_encapsulation/
│   └── encapsulation.cpp               # BankAccount — data hiding & validation
├── 03_inheritance/
│   └── inheritance_types.cpp           # single/multilevel/hierarchical/multiple + diamond problem
├── 04_polymorphism/
│   ├── 01_compile_time_polymorphism.cpp # function & operator overloading
│   └── 02_runtime_polymorphism.cpp      # virtual functions, vtable, Shape hierarchy
├── 05_abstraction/
│   └── abstraction.cpp                  # abstract classes/interfaces, PaymentMethod (Strategy pattern)
├── 06_advanced/
│   ├── 01_static_members.cpp            # static vars/functions, connection pool example
│   ├── 02_friend_function.cpp           # friend function & friend class
│   └── 03_this_pointer.cpp              # this pointer, method chaining (Builder pattern)
├── 07_real_world_project/
│   └── food_delivery_system.cpp         # CAPSTONE: combines every concept above
├── Makefile
└── README.md
```

## How to compile & run

You need `g++` with C++17 support (check with `g++ --version`).

**Option A — directly with g++:**
```bash
g++ -std=c++17 -Wall 01_basics/01_class_and_objects.cpp -o basics
./basics
```

**Option B — using the provided Makefile (easier):**
```bash
make run FILE=01_basics/01_class_and_objects.cpp
make run FILE=07_real_world_project/food_delivery_system.cpp
```

**Compile-check every file at once** (good after you edit something):
```bash
make all
```

**Clean build artifacts:**
```bash
make clean
```

## How to debug (gdb)

```bash
make debug FILE=07_real_world_project/food_delivery_system.cpp
gdb ./build/food_delivery_system_debug
```

Inside gdb, useful commands:
```
break main              # set a breakpoint at main()
run                     # start execution
next / n                # step over a line
step / s                # step into a function call
print variableName      # inspect a variable
print *shapePtr         # dereference and inspect an object
backtrace / bt          # see the call stack
continue / c            # resume until next breakpoint
```

Tip: set a breakpoint right before a `->describe()` or virtual call in
`02_runtime_polymorphism.cpp` and use `bt`/`print` to actually watch
dynamic dispatch happen — great for cementing how vtables work.

## Suggested study order

1. `01_basics` — classes/objects, then constructors/destructors
2. `02_encapsulation`
3. `03_inheritance`
4. `04_polymorphism` (compile-time first, then runtime — runtime is the
   most heavily tested topic, spend the most time here)
5. `05_abstraction`
6. `06_advanced` — static members, friend, `this`
7. `07_real_world_project` — read this LAST. Every comment references
   back to the concept file it came from. Try covering the file and
   re-deriving the design yourself before reading the solution.

## Adding your own code

- Each folder is independent — add a new `.cpp` file anywhere and run it
  with `make run FILE=<path>`.
- Good exercises to extend this repo yourself (great for interview prep):
  - Add a `Square` class under `04_polymorphism` that inherits `Rectangle`.
  - Add a `SavingsAccount : public BankAccount` with interest logic
    (practice inheritance + encapsulation together).
  - Add a `CarPartner : public DeliveryPartner` to the capstone project.
  - Convert `Order`'s payment step to also support a "Cash on Delivery"
    `PaymentMethod` (practice the Strategy/abstraction pattern).
  - Add unit tests (e.g., using a simple `assert()`-based test file, or
    set up Catch2/GoogleTest once you're comfortable).

## What's next: System Design

This repo intentionally seeds a few ideas you'll formalize next:
- `PaymentMethod` / `DeliveryPartner` abstractions → **Strategy pattern**
- `OrderBuilder` → **Builder pattern**
- Static counter for order IDs → precursor to **ID generation at scale**
  (Snowflake IDs, UUID, DB auto-increment vs distributed counters)
- Single-process demo → next ask yourself: *"How does this survive
  10,000 orders/sec, a crashed server, or a network partition?"* — that's
  where System Design (load balancers, caching, message queues,
  databases, sharding, consistency models) picks up.

Good luck with your SDE-1 prep! 🚀
