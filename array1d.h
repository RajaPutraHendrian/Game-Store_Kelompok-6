#ifndef Array1D_
#define Array1D_

#include <iostream>
#include "xcept.h"
using namespace std;

template<class T>
class Array1D {
   template<class U>
   friend ostream& operator<<(ostream&, const Array1D<U>&);
public:
   Array1D(int size = 0);
   Array1D(const Array1D<T>& v);
   ~Array1D() {delete [] element;}
   T& operator[](int i) const;
   int Size() const {return size;}
   int Count() const {return count;}
   Array1D<T>& operator=(const Array1D<T>& v);
   
   T* Find(const T& x);
   Array1D<T>& InsertFront(const T& x);
   Array1D<T>& InsertBack(const T& x);
   Array1D<T>& Insert(int index, const T& x);
   Array1D<T>& DeleteFront(T& x);
   Array1D<T>& DeleteBack(T& x);
   Array1D<T>& DeleteByValue(const T& x);
   Array1D<T>& Delete(int index, T& x);
   Array1D<T>& ReSize(int sz);
   void Clear() {count = 0;}
   bool IsEmpty() const {return count == 0;}
   
private:
   int size;
   int count;
   T *element;
};

template<class T>
Array1D<T>::Array1D(int sz)
{
   if (sz < 0) throw BadInitializers();
   size = sz;
   count = 0;
   element = new T[sz];
}

template<class T>
Array1D<T>::Array1D(const Array1D<T>& v)
{
   size = v.size;
   count = v.count;
   element = new T[size];
   for (int i = 0; i < count; i++)
      element[i] = v.element[i];
}

template<class T>
T& Array1D<T>::operator[](int i) const
{
   if (i < 0 || i >= count) throw OutOfBounds();
   return element[i];
}

template<class T>
Array1D<T>& Array1D<T>::operator=(const Array1D<T>& v)
{
   if (this != &v) {
      size = v.size;
      count = v.count;
      delete [] element;
      element = new T[size];
      for (int i = 0; i < count; i++)
         element[i] = v.element[i];
      }
   return *this;
}

template<class T>
T* Array1D<T>::Find(const T& x)
{
    for (int i = 0; i < count; i++) {
        if (element[i] == x) {
            return &element[i];
        }
    }
    return 0;
}

template<class T>
Array1D<T>& Array1D<T>::InsertFront(const T& x)
{
    if (count >= size) {
        int newSize = (size == 0) ? 1 : size * 2;
        ReSize(newSize);
    }
    
    for (int i = count; i > 0; i--) {
        element[i] = element[i-1];
    }
    element[0] = x;
    count++;
    
    return *this;
}

template<class T>
Array1D<T>& Array1D<T>::InsertBack(const T& x)
{
    if (count >= size) {
        int newSize = (size == 0) ? 1 : size * 2;
        ReSize(newSize);
    }
    
    element[count] = x;
    count++;
    
    return *this;
}

template<class T>
Array1D<T>& Array1D<T>::Insert(int index, const T& x)
{
    if (index < 0 || index > count) throw OutOfBounds();
    
    if (count >= size) {
        int newSize = (size == 0) ? 1 : size * 2;
        ReSize(newSize);
    }
    
    for (int i = count; i > index; i--) {
        element[i] = element[i-1];
    }
    element[index] = x;
    count++;
    
    return *this;
}

template<class T>
Array1D<T>& Array1D<T>::DeleteFront(T& x)
{
    if (count == 0) throw OutOfBounds();
    
    x = element[0];
    for (int i = 0; i < count - 1; i++) {
        element[i] = element[i+1];
    }
    count--;
    
    return *this;
}

template<class T>
Array1D<T>& Array1D<T>::DeleteBack(T& x)
{
    if (count == 0) throw OutOfBounds();
    
    count--;
    x = element[count];
    
    return *this;
}

template<class T>
Array1D<T>& Array1D<T>::DeleteByValue(const T& x)
{
    int index = -1;
    for (int i = 0; i < count; i++) {
        if (element[i] == x) {
            index = i;
            break;
        }
    }
    
    if (index == -1) throw OutOfBounds();
    
    for (int i = index; i < count - 1; i++) {
        element[i] = element[i + 1];
    }
    
    count--;
    return *this;
}

template<class T>
Array1D<T>& Array1D<T>::Delete(int index, T& x)
{
    if (index < 0 || index >= count) throw OutOfBounds();
    
    x = element[index];
    for (int i = index; i < count - 1; i++) {
        element[i] = element[i+1];
    }
    count--;
    
    return *this;
}

template<class T>
ostream& operator<<(ostream& out, const Array1D<T>& x)
{
   for (int i = 0; i < x.count; i++)
      out << x.element[i] << "  ";
   return out;
}

template<class T>
Array1D<T>& Array1D<T>::ReSize(int sz)
{
   if (sz < 0) throw BadInitializers();
   
   T* newElement = new T[sz];
   int copyCount = (sz < count) ? sz : count;
   
   for (int i = 0; i < copyCount; i++) {
      newElement[i] = element[i];
   }
   
   delete [] element;
   element = newElement;
   size = sz;
   count = copyCount;
   
   return *this;
}

#endif
