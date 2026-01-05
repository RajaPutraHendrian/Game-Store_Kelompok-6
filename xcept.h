#ifndef Xcept_
#define Xcept_

#include <new>
#include <stdexcept>
using namespace std;

// bad initializers
class BadInitializers {
public:
    BadInitializers() {}
};

// insufficient memory
class NoMem {
public:
    NoMem() {}
};

class QueueEmpty {
public:
    QueueEmpty() {}
};

// improper array, find, insert, or delete index
// or deletion from empty structure
class OutOfBounds {
public:
    OutOfBounds() {}
};

// use when operands should have matching size
class SizeMismatch {
public:
    SizeMismatch() {}
};

// use when zero was expected
class MustBeZero {
public:
    MustBeZero() {}
};

// invalid input
class BadInput {
public:
    BadInput() {}
};

// change new to throw NoMem instead of bad_alloc
void my_new_handler();

// deklarasi eksternal handler lama
extern std::new_handler Old_Handler_;

#endif

