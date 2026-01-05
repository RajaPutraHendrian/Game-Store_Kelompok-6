#ifndef DoublyCircularLinkedList_
#define DoublyCircularLinkedList_

#include <iostream>
#include "xcept.h"
using namespace std;

template<class T>
class DoublyCircularLinkedList;

template<class T>
class DNode {
    friend class DoublyCircularLinkedList<T>;
private:
    T data;
    DNode<T>* prev;  // Pointer to previous node
    DNode<T>* next;  // Pointer to next node
};

template<class T>
class DoublyCircularLinkedList {
public:
    DoublyCircularLinkedList();
    DoublyCircularLinkedList(const DoublyCircularLinkedList<T>& list);
    ~DoublyCircularLinkedList();
    
    bool IsEmpty() const { return first == 0; }
    int Length() const;
    bool Find(const T& x) const;
    T* FindPtr(const T& x);
    
    // Insert operations
    DoublyCircularLinkedList<T>& InsertFront(const T& x);
    DoublyCircularLinkedList<T>& InsertBack(const T& x);
    DoublyCircularLinkedList<T>& Insert(int k, const T& x);
    
    // Delete operations
    DoublyCircularLinkedList<T>& DeleteFront(T& x);
    DoublyCircularLinkedList<T>& DeleteBack(T& x);
    DoublyCircularLinkedList<T>& Delete(int k, T& x);
    DoublyCircularLinkedList<T>& DeleteByValue(const T& x);
    
    // Access operations
    T& Get(int k) const;
    T& GetFirst() const;
    T& GetLast() const;
    
    // Navigation operations (unique to DLL)
    T& GetPrev(int k) const;  // Get previous element
    T& GetNext(int k) const;  // Get next element
    
    // Utility
    void Clear();
    void Display() const;
    void DisplayReverse() const;  // Display backward (unique to DLL)
    
    DoublyCircularLinkedList<T>& operator=(const DoublyCircularLinkedList<T>& list);
    
private:
    DNode<T>* first;
    DNode<T>* last;   // Pointer to last node (for efficient InsertBack)
};

// Constructor
template<class T>
DoublyCircularLinkedList<T>::DoublyCircularLinkedList()
{
    first = 0;
    last = 0;
}

// Copy constructor
template<class T>
DoublyCircularLinkedList<T>::DoublyCircularLinkedList(const DoublyCircularLinkedList<T>& list)
{
    first = 0;
    last = 0;
    
    if (list.first) {
        DNode<T>* current = list.first;
        do {
            InsertBack(current->data);
            current = current->next;
        } while (current != list.first);  // Circular stop condition
    }
}

// Destructor
template<class T>
DoublyCircularLinkedList<T>::~DoublyCircularLinkedList()
{
    Clear();
}

// Get length
template<class T>
int DoublyCircularLinkedList<T>::Length() const
{
    if (!first) return 0;
    
    DNode<T>* current = first;
    int len = 0;
    do {
        len++;
        current = current->next;
    } while (current != first);  // Circular: stop when back to first
    return len;
}

// Find element
template<class T>
bool DoublyCircularLinkedList<T>::Find(const T& x) const
{
    if (!first) return false;
    
    DNode<T>* current = first;
    do {
        if (current->data == x) return true;
        current = current->next;
    } while (current != first);  // Circular: stop when back to first
    return false;
}

// Find and return pointer
template<class T>
T* DoublyCircularLinkedList<T>::FindPtr(const T& x)
{
    if (!first) return 0;
    
    DNode<T>* current = first;
    do {
        if (current->data == x) {
            return &(current->data);
        }
        current = current->next;
    } while (current != first);  // Circular
    return 0;
}

// Insert at front - DCLL version
template<class T>
DoublyCircularLinkedList<T>& DoublyCircularLinkedList<T>::InsertFront(const T& x)
{
    DNode<T>* newNode = new DNode<T>;
    newNode->data = x;
    
    if (first == 0) {
        // Empty list - node points to itself (circular!)
        first = last = newNode;
        newNode->next = newNode;
        newNode->prev = newNode;
    } else {
        // Insert at front and maintain circular connections
        newNode->next = first;
        newNode->prev = last;  // Connect to last (circular!)
        first->prev = newNode;
        last->next = newNode;   // last->next points to new first (circular!)
        first = newNode;
    }
    
    return *this;
}

// Insert at back - DCLL version
template<class T>
DoublyCircularLinkedList<T>& DoublyCircularLinkedList<T>::InsertBack(const T& x)
{
    DNode<T>* newNode = new DNode<T>;
    newNode->data = x;
    
    if (last == 0) {
        // Empty list - node points to itself (circular!)
        first = last = newNode;
        newNode->next = newNode;
        newNode->prev = newNode;
    } else {
        // Insert at back and maintain circular connections
        newNode->prev = last;
        newNode->next = first;  // Connect to first (circular!)
        last->next = newNode;
        first->prev = newNode;  // first->prev points to new last (circular!)
        last = newNode;
    }
    
    return *this;
}

// Insert at position k
template<class T>
DoublyCircularLinkedList<T>& DoublyCircularLinkedList<T>::Insert(int k, const T& x)
{
    if (k < 0) throw OutOfBounds();
    
    if (k == 0) {
        InsertFront(x);
        return *this;
    }
    
    // Traverse to position k-1
    DNode<T>* current = first;
    for (int i = 0; i < k - 1 && current; i++)
        current = current->next;
    
    if (!current) throw OutOfBounds();
    
    DNode<T>* newNode = new DNode<T>;
    newNode->data = x;
    newNode->next = current->next;
    newNode->prev = current;
    
    if (current->next) {
        current->next->prev = newNode;
    } else {
        last = newNode;  // Inserting at end
    }
    current->next = newNode;
    
    return *this;
}

// Delete from front - DCLL version
template<class T>
DoublyCircularLinkedList<T>& DoublyCircularLinkedList<T>::DeleteFront(T& x)
{
    if (!first) throw OutOfBounds();
    
    DNode<T>* delNode = first;
    x = first->data;
    
    if (first == last) {
        // Only one element
        first = last = 0;
    } else {
        // Multiple elements - maintain circular
        first = first->next;
        first->prev = last;     // Circular: first->prev points to last
        last->next = first;     // Circular: last->next points to first
    }
    
    delete delNode;
    return *this;
}

// Delete from back - DCLL version
template<class T>
DoublyCircularLinkedList<T>& DoublyCircularLinkedList<T>::DeleteBack(T& x)
{
    if (!last) throw OutOfBounds();
    
    DNode<T>* delNode = last;
    x = last->data;
    
    if (first == last) {
        // Only one element
        first = last = 0;
    } else {
        // Multiple elements - maintain circular
        last = last->prev;
        last->next = first;     // Circular: last->next points to first
        first->prev = last;     // Circular: first->prev points to last
    }
    
    delete delNode;
    return *this;
}

// Delete at position k
template<class T>
DoublyCircularLinkedList<T>& DoublyCircularLinkedList<T>::Delete(int k, T& x)
{
    if (!first || k < 0) throw OutOfBounds();
    
    if (k == 0) {
        DeleteFront(x);
        return *this;
    }
    
    // Traverse to position k
    DNode<T>* current = first;
    for (int i = 0; i < k && current; i++)
        current = current->next;
    
    if (!current) throw OutOfBounds();
    
    x = current->data;
    
    // Update links
    if (current->prev) {
        current->prev->next = current->next;
    }
    if (current->next) {
        current->next->prev = current->prev;
    } else {
        last = current->prev;  // Deleting last node
    }
    
    delete current;
    return *this;
}

// Delete by value
template<class T>
DoublyCircularLinkedList<T>& DoublyCircularLinkedList<T>::DeleteByValue(const T& x)
{
    if (!first) throw OutOfBounds();
    
    DNode<T>* current = first;
    while (current) {
        if (current->data == x) {
            // Found it
            if (current->prev) {
                current->prev->next = current->next;
            } else {
                first = current->next;
            }
            
            if (current->next) {
                current->next->prev = current->prev;
            } else {
                last = current->prev;
            }
            
            delete current;
            return *this;
        }
        current = current->next;
    }
    
    throw OutOfBounds();  // Not found
}

// Get element at position k
template<class T>
T& DoublyCircularLinkedList<T>::Get(int k) const
{
    if (k < 0) throw OutOfBounds();
    
    DNode<T>* current = first;
    for (int i = 0; i < k && current; i++)
        current = current->next;
    
    if (!current) throw OutOfBounds();
    
    return current->data;
}

// Get first element
template<class T>
T& DoublyCircularLinkedList<T>::GetFirst() const
{
    if (!first) throw OutOfBounds();
    return first->data;
}

// Get last element
template<class T>
T& DoublyCircularLinkedList<T>::GetLast() const
{
    if (!last) throw OutOfBounds();
    return last->data;
}

// Get previous element (navigate backward)
template<class T>
T& DoublyCircularLinkedList<T>::GetPrev(int k) const
{
    if (k <= 0) throw OutOfBounds();
    return Get(k - 1);
}

// Get next element (navigate forward)
template<class T>
T& DoublyCircularLinkedList<T>::GetNext(int k) const
{
    return Get(k + 1);
}

// Clear all elements
template<class T>
void DoublyCircularLinkedList<T>::Clear()
{
    if (!first) return;
    
    // Break the circle first
    last->next = 0;
    
    DNode<T>* current;
    while (first) {
        current = first;
        first = first->next;
        delete current;
    }
    last = 0;
}

// Display forward (circular)
template<class T>
void DoublyCircularLinkedList<T>::Display() const
{
    if (!first) {
        cout << "Empty List" << endl;
        return;
    }
    
    DNode<T>* current = first;
    cout << "Circular Forward: ";
    do {
        cout << current->data << " -> ";
        current = current->next;
    } while (current != first);  // Stop when back to first
    cout << "[back to first]" << endl;
}

// Display backward (circular)
template<class T>
void DoublyCircularLinkedList<T>::DisplayReverse() const
{
    if (!last) {
        cout << "Empty List" << endl;
        return;
    }
    
    DNode<T>* current = last;
    cout << "Circular Backward: ";
    do {
        cout << current->data << " <- ";
        current = current->prev;
    } while (current != last);  // Stop when back to last
    cout << "[back to last]" << endl;
}

// Assignment operator
template<class T>
DoublyCircularLinkedList<T>& DoublyCircularLinkedList<T>::operator=(const DoublyCircularLinkedList<T>& list)
{
    if (this != &list) {
        Clear();
        
        if (list.first) {
            DNode<T>* current = list.first;
            do {
                InsertBack(current->data);
                current = current->next;
            } while (current != list.first);  // Circular
        }
    }
    return *this;
}

#endif
