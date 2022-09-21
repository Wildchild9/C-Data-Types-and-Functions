//
// Created by Noah Wilder on 2022-09-21.
//

#include "stack.h"
#include <iostream>

template<typename T>
stack<T>::stack() {
    count = 0;
    allocatedSize = 0;
    arr = nullptr;
}

template<typename T>
bool stack<T>::isEmpty() const {
    return count == 0;
}

template<typename T>
int stack<T>::length() const {
    return count;
}

template<typename T>
void stack<T>::push(T element) {
    if (allocatedSize == 0) {
        allocatedSize = 2;
        arr = static_cast<T*>(malloc(allocatedSize * sizeof(T)));
    } else if (allocatedSize == count) {
        allocatedSize *= 2;
        arr = static_cast<T*>(realloc(arr, allocatedSize * sizeof(T)));
    }
    count++;
    arr[count - 1] = element;
}

template<typename T>
T stack<T>::pop() {
    if (isEmpty()) {
        exit(1);
    }
    count--;
    T element = arr[count];
    return element;
}

template<typename T>
void stack<T>::print() {
    if (count == 0) {
        std::cout << "[]" << std::endl;
    } else {
        std::cout << "[" << arr[0];
        for (int i = 1; i < count; i++) {
            std::cout << ", " << arr[i];
        }
        std::cout << "]" << std::endl;
    }
}