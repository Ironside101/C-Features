#pragma once
#include <memory>
#include <algorithm>
#include <cmath>

template <class T>
class Vector {
private:
    size_t v_size = 0, v_capacity = 0;
    // for custom types, optionally pass a user defined null/zero value for that specific type to the constructor for use in functions like return_zero()
    T custom_type_zero;

    // double the capacity of the vector
    // multiplier specifies how many times to double the size
    void double_capacity(size_t multiplier = 1);

    // set all the elements above start equal to 0
    void set_all_zero(size_t start) { for (int i = start; i < v_capacity; i++) ptr[i] = return_zero(); }

    bool full() {
        if (v_size == v_capacity) return true;
        else return false;
    }

    // returns 0 in respective type
    // e.g. "0" for strings, 0 for ints, 0.0f for floats
    T return_zero();

public:
    std::unique_ptr<T[]> ptr;
    Vector(T t_zero = 0) : custom_type_zero(t_zero) {
        ptr =  std::make_unique<T[]>(1);
        v_size = 0;
        v_capacity = 1;
    }

    Vector(size_t size, T t_zero = 0) : custom_type_zero(t_zero)  {
        ptr = std::make_uniqe<T>(size);
        v_size = 0;
        c_s = size;

        set_all_zero();
    }

    ~Vector<T>() {
        ptr.reset();
    };


    // return number of elements
    int size() const { return v_size; };

    // return memory reserved for vector
    int capacity() const { return v_capacity; };

    // return true if empty
    bool is_empty() {
        if (size() == 0) return true;
        else return false;
    };

    // return refrence to first element
    T& front() { return ptr[0]; }

    // returns a refrence to last element by derefrencing the pointer to last element
    T& back() { return *this->last(); }

    // return pointer to underlying array
    T* data() { return ptr.get(); }

    // returns pointer to last element
    T* last();



    // make sure vector can store at least n elements
    void reserve(size_t size);

    // changes the size of vector
    void resize(size_t size);
    // fills any extra space with place_holder
    void resize(size_t size, T place_holder);

    // add data to end of array
    void push_back(const T& val);

    // reduce capacity to be equal to size
    void shrink_to_fit(); // NOT DONE
    
    // removes last element of vector
    void pop_back();

    // insert element at i
    void insert(const T& val, size_t index);

    // erases element at index i
    void erase(size_t i); // NOT DONE

    // return value at ith element of array but also check bounds
    T at(size_t i);


    // print entire vector
    void print();
    // print all elements in the given range
    void print(size_t lower_bound, size_t upper_bound);

    // uses quicksort to find element
    int find(T val);

    // swap this vector with v
    void swap(Vector<T>& v);



    // operator to retrieve the ith element
    const T& operator[] (size_t i) const { return ptr[i]; };

    // operator for when trying to assign a value to ith element
    T& operator[] (size_t i) {
        // if adding an element beyond the current vector size, then the vectors size = the index of the new element
        if (i >= v_size) v_size = i;

        return ptr[i];
    };

    // assignment operator
    Vector<T>& operator= (const Vector<T>& v);


};

template <class T>
void Vector<T>::reserve(size_t size) {
    // dont continue if vector is already large enough
    if (size <= v_capacity) return;

    // this variable will be used to set all new elements to zero
    int old_size = v_size;

    int i = 1;
    // find how many times the current capacity must be double before it is large enough
    while (v_capacity * pow(2, i) < size) {
        i++;
    };

    // this will double the vectors capacity i times
    double_capacity(i);

    // set all the newly created elements to zero
    set_all_zero(old_size);

    return;
}

template <class T>
T* Vector<T>::last() {
    // get the underlying pointer
    T* data_ptr = this->data();
    // add the amonut of elements onto that address then return
    return data_ptr + this->size();
}

template <class T>
void Vector<T>::push_back(const T& val) {
    // see weather the vector needs to be increased in size to add another element
    if (this->full()) {
        double_capacity();
    }
    
    ptr[v_size] = val;
    v_size++;

    return;
}

template <class T>
void Vector<T>::double_capacity(size_t multiplier = 1) {
    // create new pointer and allocate twice the space of the underlying pointer
    std::unique_ptr<T[]> temp_ptr(new T[v_capacity * pow(2, multiplier)]);
    
    // copy data from the vectors underlying pointer to the new pointer
    std::copy(this->data(), this->last(), temp_ptr.get());

    // transfer ownership from temp_ptr to the vectors underlying pointer
    ptr = std::move(temp_ptr);

    v_capacity *= pow(2, multiplier);
    
    return;
}

template <class T>
void Vector<T>::print(size_t lower_bound, size_t upper_bound) {
    // first check bounds
    if (lower_bound < 0) { printf("Lower bound less than 0\n"); return; }
    if (upper_bound > v_capacity) { printf("Upper bound exceeding vector limits\n"); return; }

    for (int i = lower_bound; i <= upper_bound; i++) std::cout << i << ": " << ptr[i] << "\n";

    return;
}

template <class T>
void Vector<T>::print() {
    for (int i = 0; i < v_size; i++) std::cout << i << ": " << ptr[i] << "\n";

    return;
}

template <class T>
void Vector<T>::pop_back() {
    ptr[v_size - 1] = 0;
    v_size--;

    return;
}

template <class T>
Vector<T>& Vector<T>::operator= (const Vector<T>& v) {
    // check for self assignment
    if (this == &v) return *this;

    this->reserve(v.size());

    // copy elements
    for (int i = 0; i < v.size(); i++){
        (*this)[i] = v[i];
    }

    return *this;
}

template <class T>
T Vector<T>::at(size_t i) {
    if (i >= v_capacity) {
        printf("ERROR: Vector.at() ATTEMPTING TO READ AN INDEX OUT OF BOUNDS AT: %d\n", i);
        return return_zero();
    }
    
    else if (i >= v_size){
        printf("WARNING: Vector.at() ATTEMPTING TO READ AN INDEX UN-ASSIGNED DATA AT: %d\n", i);
        return return_zero();
    }

    return (*this)[i];
}

template <class T>
void Vector<T>::insert(const T& val, size_t index) {
    if (index >= v_capacity || index == 0) {
        printf("ERROR: Vector.insert() ATTEMPTING TO INSERT OUT OF BOUNDS AT: %d\n", index);
        return;
    }

    if (index >= v_size) {
        (*this)[index] = val;
        v_size++;
        return;
    }

    else {
        for (int i = v_size; i >= index; i--) {
            (*this)[i] = (*this)[i + 1];
        }

        v_size++;

        return;
    }
}

template <class T>
T Vector<T>::return_zero() {
    if (std::is_same<T, int>()) return 0;
    else if (std::is_same<T, float>()) return 0.0f;
    else if (std::is_same<T, char>()) return '0';
    else if (std::is_same<T, bool>()) return false;

    // if the user is using a custom type return custom_type_zero
    return custom_type_zero;
}

template <class T>
void Vector<T>::shrink_to_fit() {
    // allocate new array that is just big enough to fit all the vectors elements
    std::unique_ptr<T[]> temp_ptr(new T[v_size]);
    
    for (int i = 0; i < v_size; i++)
        temp_ptr[i] = (*this)[i];

    // transfer ownership back to ptr
    ptr = std::move(temp_ptr);

    v_capacity = v_size;

    return;
}

template <class T>
void Vector<T>::erase(size_t i) {
    // dont erase if its above vectors size
    if (i >= v_size) return;

    // shift data to left starting at i, therefore writing over it
    for (int j = i; j < v_size - 1; j++)
        (*this)[j] = (*this)[j + 1];

    (*this)[v_size - 1] = 0;

    v_size--;

    return;
}

template <class T>
int Vector<T>::find(T val) {
    for (int i = 0; i < v_size; i++) {
        auto it = ptr[i];

        if (it == val) return i;
    }

    // failed the search
    return -1;
}

template <class T>
void Vector<T>::resize(size_t size) {
    // if the vector should be increased in size
    if (size > v_size) {
        // may need to incease the capacity of vector
        if (size > v_capacity) reserve(size);


        // assign the new spaces to zero's
        for (int i = v_size; i <= size; i++) (*this)[i] = return_zero();

        (*this).print();
        // update the size of vector
        v_size = size;

        return;
    }

    // if vector must be shrunk down
    else {
        for (int i = size; i < v_size; i++) (*this)[i] = return_zero();

        v_size = size;

        return;
    }   
}

template <class T>
void Vector<T>::resize(size_t size, T place_holder) {
    // if the vector should be increased in size
    if (size > v_size) {
        // may need to incease the capacity of vector
        if (size > v_capacity) reserve(size);


        // assign the new spaces to zero's
        for (int i = v_size; i <= size; i++) (*this)[i] = place_holder;

        (*this).print();
        // update the size of vector
        v_size = size;

        return;
    }

    // if vector must be shrunk down
    else {
        for (int i = size; i < v_size; i++) (*this)[i] = place_holder;

        v_size = size;

        return;
    }   
}

template <class T>
void Vector<T>::swap(Vector<T>& v) {
    std::swap(v.ptr, this->ptr);
    return;
}
