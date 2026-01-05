#ifndef Queue_
#define Queue_

#include <iostream>
#include "linklist.h" 
#include "xcept.h"
using namespace std;

template<class T>
class Queue {
public:
    Queue() {}
    ~Queue() {}

    bool IsEmpty() const { return list.IsEmpty(); }
    T& Front() const; 
    Queue<T>& Enqueue(const T& x); 
    Queue<T>& Dequeue(T& x); 
    int Size() const { return list.Length(); }
    void Clear() { list.Clear(); }
    
    void Display() const; 

private:
    LinkedList<T> list; 
};


template<class T>
T& Queue<T>::Front() const
{
    if (IsEmpty()) throw QueueEmpty();
    return list.Get(0);
}

template<class T>
Queue<T>& Queue<T>::Enqueue(const T& x)
{
    list.InsertBack(x);
    return *this;
}

template<class T>
Queue<T>& Queue<T>::Dequeue(T& x)
{
    if (IsEmpty()) throw QueueEmpty();
    list.DeleteFront(x);
    return *this;
}

template<class T>
void Queue<T>::Display() const
{
    if (IsEmpty()) {
        cout << "\nAntrean kosong!" << endl;
        return;
    }
    
    cout << "\n========== ISI ANTREAN (depan ke belakang) ==========" << endl;
    for (int i = 0; i < list.Length(); i++) {
        try {
            T currentItem = list.Get(i); 
            
            cout << "\nData ke-" << (i + 1) << (i == 0 ? " <-- DEPAN (FRONT)" : "") << endl;
            cout << currentItem << endl;
        }
        catch(OutOfBounds) {
        }
    }
    cout << "=========================================================" << endl;
}

#endif
