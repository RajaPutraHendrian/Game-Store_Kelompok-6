#ifndef LinkedList_
#define LinkedList_
#include <iostream>
#include "xcept.h"
using namespace std;

template<class T>
class LinkedList;

template<class T>
class Node {
    friend class LinkedList<T>;
private:
    T data;
    Node<T>* link;
};

template<class T>
class LinkedList {
    template<class U>
    friend ostream& operator<<(ostream&, const LinkedList<U>&);
public:
    LinkedList();
    LinkedList(const LinkedList<T>& list);
    ~LinkedList();
    
    bool IsEmpty() const {return first == 0;}
    int Length() const;
    bool Find(const T& x) const;
    
    LinkedList<T>& Insert(int k, const T& x);
    LinkedList<T>& Delete(int k, T& x);
    LinkedList<T>& InsertFront(const T& x);
    LinkedList<T>& InsertBack(const T& x);
    LinkedList<T>& DeleteFront(T& x);
    LinkedList<T>& DeleteBack(T& x);
    LinkedList<T>& DeleteByValue(const T& x);
    
    T* FindPtr(const T& x);
    T& Get(int k) const;
    void Clear();
    
    LinkedList<T>& operator=(const LinkedList<T>& list);
    LinkedList<T> operator+(const LinkedList<T>& list) const;
    
private:
    Node<T>* first;
    Node<T>* last;
};

template<class T>
LinkedList<T>::LinkedList()
{
    first = last = 0;
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T>& list)
{
    first = last = 0;
    Node<T>* current = list.first;
    
    while (current) {
        InsertBack(current->data);
        current = current->link;
    }
}

template<class T>
LinkedList<T>::~LinkedList()
{
    Node<T>* current;
    while (first) {
        current = first;
        first = first->link;
        delete current;
    }
}

template<class T>
int LinkedList<T>::Length() const
{
    Node<T>* current = first;
    int len = 0;
    while (current) {
        len++;
        current = current->link;
    }
    return len;
}

template<class T>
bool LinkedList<T>::Find(const T& x) const
{
    Node<T>* current = first;
    while (current) {
        if (current->data == x) return true;
        current = current->link;
    }
    return false;
}

template<class T>
LinkedList<T>& LinkedList<T>::InsertFront(const T& x)
{
    Node<T>* newNode = new Node<T>;
    newNode->data = x;
    newNode->link = first;
    first = newNode;
    if (!last) last = first;
    return *this;
}

template<class T>
LinkedList<T>& LinkedList<T>::InsertBack(const T& x)
{
    Node<T>* newNode = new Node<T>;
    newNode->data = x;
    newNode->link = 0;
    
    if (first) {
        last->link = newNode;
        last = newNode;
    }
    else {
        first = last = newNode;
    }
    return *this;
}

template<class T>
LinkedList<T>& LinkedList<T>::Insert(int k, const T& x)
{
    if (k < 0) throw OutOfBounds();
    
    if (k == 0 && !first) {
        InsertFront(x);
        return *this;
    }
    
    Node<T>* current = first;
    for (int i = 0; i < k && current; i++)
        current = current->link;
    
    if (!current) throw OutOfBounds();
    
    Node<T>* newNode = new Node<T>;
    newNode->data = x;
    newNode->link = current->link;
    current->link = newNode;
    
    if (!newNode->link) last = newNode;
    
    return *this;
}

template<class T>
LinkedList<T>& LinkedList<T>::DeleteFront(T& x)
{
    if (!first) throw OutOfBounds();
    
    Node<T>* delNode = first;
    x = first->data;
    first = first->link;
    delete delNode;
    
    if (!first) last = 0;
    
    return *this;
}

template<class T>
LinkedList<T>& LinkedList<T>::DeleteBack(T& x)
{
    if (!first) throw OutOfBounds();
    
    if (first == last) {
        x = first->data;
        delete first;
        first = last = 0;
        return *this;
    }
    
    Node<T>* current = first;
    while (current->link != last)
        current = current->link;
    
    x = last->data;
    delete last;
    last = current;
    last->link = 0;
    
    return *this;
}

template<class T>
LinkedList<T>& LinkedList<T>::DeleteByValue(const T& x)
{
    if (!first) throw OutOfBounds();
    
    if (first->data == x) {
        Node<T>* temp = first;
        first = first->link;
        delete temp;
        
        if (!first) last = 0;
        
        return *this;
    }
    
    Node<T>* current = first;
    while (current->link) {
        if (current->link->data == x) {
            Node<T>* delNode = current->link;
            current->link = delNode->link;
            
            if (delNode == last) last = current;
            
            delete delNode;
            return *this;
        }
        current = current->link;
    }
    
    throw OutOfBounds();
}

template<class T>
LinkedList<T>& LinkedList<T>::Delete(int k, T& x)
{
    if (!first || k < 0) throw OutOfBounds();
    
    if (k == 0) {
        DeleteFront(x);
        return *this;
    }
    
    Node<T>* current = first;
    for (int i = 0; i < k - 1 && current; i++)
        current = current->link;
    
    if (!current || !current->link) throw OutOfBounds();
    
    Node<T>* delNode = current->link;
    x = delNode->data;
    current->link = delNode->link;
    
    if (delNode == last) last = current;
    
    delete delNode;
    return *this;
}

template<class T>
T* LinkedList<T>::FindPtr(const T& x)
{
    Node<T>* current = first;
    while (current) {
        if (current->data == x) {
            return &(current->data);
        }
        current = current->link;
    }
    return 0;
}

template<class T>
T& LinkedList<T>::Get(int k) const
{
    if (k < 0) throw OutOfBounds();
    
    Node<T>* current = first;
    for (int i = 0; i < k && current; i++)
        current = current->link;
    
    if (!current) throw OutOfBounds();
    
    return current->data;
}

template<class T>
void LinkedList<T>::Clear()
{
    Node<T>* current;
    while (first) {
        current = first;
        first = first->link;
        delete current;
    }
    last = 0;
}

template<class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& list)
{
    if (this != &list) {
        Clear();
        
        Node<T>* current = list.first;
        while (current) {
            InsertBack(current->data);
            current = current->link;
        }
    }
    return *this;
}

template<class T>
LinkedList<T> LinkedList<T>::operator+(const LinkedList<T>& list) const
{
    LinkedList<T> result(*this);
    
    Node<T>* current = list.first;
    while (current) {
        result.InsertBack(current->data);
        current = current->link;
    }
    
    return result;
}

template<class T>
ostream& operator<<(ostream& out, const LinkedList<T>& list)
{
    Node<T>* current = list.first;
    while (current) {
        out << current->data << "  ";
        current = current->link;
    }
    return out;
}

#endif
