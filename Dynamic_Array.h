#pragma once
#include <iostream>

using namespace std;

template <typename T>
class Dynamic_array {
private:
    T* arr;
    int len;
    int cap;

    void double_capacity() {
        int new_cap = (cap == 0) ? 1 : cap * 2;
        T* new_arr = nullptr;

        try {
            new_arr = new T[new_cap];
        }
        catch (bad_alloc& e) {
            throw runtime_error("Memory allocation failed during resize.");
        }

        for (int i = 0; i < len; i++) {
            new_arr[i] = arr[i];
        }

        delete[] arr;
        arr = new_arr;
        cap = new_cap;
    }

public:
    Dynamic_array() {
        arr = nullptr;
        len = 0;
        cap = 0;
    }

    Dynamic_array(int count, T value) {
        if (count < 0)
            throw invalid_argument("Array size cannot be negative.");

        len = count;
        cap = count;

        try {
            arr = new T[cap];
        }
        catch (bad_alloc& e) {
            throw runtime_error("Memory allocation failed in constructor.");
        }

        for (int i = 0; i < len; i++) arr[i] = value;
    }

    Dynamic_array(const Dynamic_array& other) {
        len = other.len;
        cap = other.cap;

        try {
            arr = new T[cap];
        }
        catch (bad_alloc& e) {
            throw runtime_error("Memory allocation failed in copy constructor.");
        }

        for (int i = 0; i < len; i++) arr[i] = other.arr[i];
    }

    ~Dynamic_array() {
        delete[] arr;
    }

    Dynamic_array& operator=(const Dynamic_array& other) {
        if (this != &other) {
            T* new_arr = nullptr;
            try {
                new_arr = new T[other.cap];
            }
            catch (bad_alloc& e) {
                throw runtime_error("Memory allocation failed in assignment.");
            }

            for (int i = 0; i < other.len; i++) new_arr[i] = other.arr[i];

            delete[] arr;
            arr = new_arr;
            len = other.len;
            cap = other.cap;
        }
        return *this;
    }

    // Accessors
    T& operator[](int index) {
        if (index < 0 || index >= len)
            throw out_of_range("Index out of bounds.");
        return arr[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= len)
            throw out_of_range("Index out of bounds.");
        return arr[index];
    }

    T& at(int index) {
        if (index < 0 || index >= len)
            throw out_of_range("Invalid index access.");
        return arr[index];
    }

    const T& at(int index) const {
        if (index < 0 || index >= len)
            throw out_of_range("Invalid index access.");
        return arr[index];
    }

    T& front() {
        if (len == 0)
            throw underflow_error("Array is empty.");
        return arr[0];
    }

    const T& front() const {
        if (len == 0)
            throw underflow_error("Array is empty.");
        return arr[0];
    }

    T& back() {
        if (len == 0)
            throw underflow_error("Array is empty.");
        return arr[len - 1];
    }

    const T& back() const {
        if (len == 0)
            throw underflow_error("Array is empty.");
        return arr[len - 1];
    }

    // Modifiers
    void push(T value) {
        if (len >= cap) double_capacity();
        arr[len++] = value;
    }

    void pop() {
        if (len == 0)
            throw underflow_error("Cannot pop from empty array.");
        len--;
    }

    void insert_at(int index, T value) {
        if (index < 0 || index > len)
            throw out_of_range("Insert index out of bounds.");

        if (len >= cap) double_capacity();

        for (int i = len; i > index; i--)
            arr[i] = arr[i - 1];

        arr[index] = value;
        len++;
    }

    void delete_at(int index) {
        if (index < 0 || index >= len)
            throw out_of_range("Delete index out of bounds.");

        for (int i = index; i < len - 1; i++)
            arr[i] = arr[i + 1];

        len--;
    }

    void clear() {
        len = 0;
    }

    void reverse() {
        for (int i = 0; i < len / 2; i++) {
            T temp = arr[i];
            arr[i] = arr[len - i - 1];
            arr[len - i - 1] = temp;
        }
    }

    // Utility
    bool empty() const {
        return len == 0;
    }

    int size() const {
        return len;
    }

    int capacity() const {
        return cap;
    }

    int find(T value) const {
        for (int i = 0; i < len; i++) {
            if (arr[i] == value) return i;
        }
        return -1;
    }

    void print_array() const {
        cout << "Array elements: ";
        for (int i = 0; i < len; ++i) {
            cout << arr[i] << " ";
        }
        cout << endl;
    }
};
