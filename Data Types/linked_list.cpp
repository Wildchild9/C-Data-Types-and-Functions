//
// Created by Noah Wilder on 2022-09-21.
//

#include "linked_list.h"

#include <iostream>
#include <optional>


template<typename T>
linked_list<T>::linked_list() {
    count = 0;
}

template<typename T>
linked_list<T>::linked_list(std::initializer_list<T> list) {
    count = list.size();
    if (count == 0) {
        return;
    }
    const T *it = list.begin();
    node<T> *newNode(new node<T>(*it));
    head = newNode;
    ++it;
    for (; it != list.end(); ++it) {
        newNode->next = new node<T>(*it);
        newNode->next->previous = newNode;
        newNode = newNode->next;
    }
    tail = newNode;
}

template<typename T>
linked_list<T>::~linked_list() {
    delete head;
    delete tail;
}

template<typename T>
bool linked_list<T>::isEmpty() const {
    return count == 0;
}

template<typename T>
int linked_list<T>::size() const {
    return count;
}

template<typename T>
T& linked_list<T>::operator[] (int index) {
    if (index < 0 || index >= count) {
        throw std::runtime_error("Index out of bounds.");
    }
    node<T> *currentNode;
    if (count - index < index) {
        currentNode = tail;
        for (int i = count - 1; i != index; i--) {
            currentNode = currentNode->previous;
        }
    } else {
        currentNode = head;
        for (int i = 0; i != index; i++) {
            currentNode = currentNode->next;
        }
    }
    return currentNode->value;
}

template<typename T>
std::optional<T> linked_list<T>::first() {
    return isEmpty() ? std::nullopt : std::make_optional(head->value);
}

template<typename T>
std::optional<T> linked_list<T>::last() {
    return isEmpty() ? std::nullopt : std::make_optional(tail->value);
}

template<typename T>
void linked_list<T>::append(T element) {
    node<T> *newNode(new node<T>(element));
    if (isEmpty()) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        newNode->previous = tail;
        tail = newNode;
    }
    count++;
}

template <typename T>
template <typename... U, typename>
void linked_list<T>::append(T element, U... elements) {
    node<T> *newNode(new node<T>(element));
    if (isEmpty()) {
        head = newNode;
    } else {
        tail->next = newNode;
        newNode->previous = tail;
    }
    count++;
    for (T currentElement : { elements... }) {
        count++;
        newNode->next = new node<T>(currentElement);
        newNode->next->previous = newNode;
        newNode = newNode->next;
    }
    tail = newNode;
}

template<typename T>
void linked_list<T>::prepend(T element) {
    node<T> *newNode(new node<T>(element));
    if (isEmpty()) {
        head = newNode;
        tail = newNode;
    } else {
        head->previous = newNode;
        newNode->next = head;
        head = newNode;
    }
    count++;
}

template <typename T>
template <typename... U, typename>
void linked_list<T>::prepend(T element, U... elements) {
    node<T> *currentNode(new node<T>(element));
    bool wasEmpty = isEmpty();
    node<T> *firstNode = currentNode;
    count++;
    for (T currentElement : { elements... }) {
        count++;
        currentNode->next = new node<T>(currentElement);
        currentNode->next->previous = currentNode;
        currentNode = currentNode->next;
    }
    if (wasEmpty) {
        head = firstNode;
        tail = currentNode;
    } else {
        currentNode->next = head;
        head->previous = currentNode;
        head = firstNode;
    }
}

template<typename T>
void linked_list<T>::removeAll() {
    if (isEmpty()) {
        return;
    }
    node<T> *currentNode = head;
    for (int i = 0; i < count - 1; i++) {
        node<T> *nextNode = currentNode->next;
        delete currentNode;
        currentNode = nextNode;
    }
    delete currentNode;
    count = 0;
}

template<typename T>
std::optional<T> linked_list<T>::popFirst() {
    if (count == 0) {
        return std::nullopt;
    }
    T value = head->value;
    if (count == 1) {
        delete head;
    } else {
        head = head->next;
        std::free(head->previous);
        head->previous = nullptr;
    }
    return std::make_optional(value);
}

template<typename T>
std::optional<T> linked_list<T>::popLast() {
    if (count == 0) {
        return std::nullopt;
    }
    T value = tail->value;
    if (count == 1) {
        delete tail;
    } else {
        tail = tail->previous;
        std::free(tail->next);
        tail->next = nullptr;
    }
    return std::make_optional(value);
}

template<typename T>
T linked_list<T>::removeFirst() {
    if (count == 0) {
        throw std::runtime_error("Cannot remove the first element of an empty list.");
    }
    T value = head->value;
    if (count == 1) {
        delete head;
    } else {
        head = head->next;
        std::free(head->previous);
        head->previous = nullptr;
    }
    return value;
}

template<typename T>
T linked_list<T>::removeLast() {
    if (count == 0) {
        throw std::runtime_error("Cannot remove the last element of an empty list.");
    }
    T value = tail->value;
    if (count == 1) {
        delete tail;
    } else {
        tail = tail->previous;
        std::free(tail->next);
        tail->next = nullptr;
    }
    return value;
}

template<typename T>
void linked_list<T>::removeFirst(int k) {
    if (k <= 0 || isEmpty()) {
        return;
    } else if (k >= count) {
        removeAll();
        return;
    }
    node<T> *currentNode = head;
    for (int i = 0; i < k; i++) {
        currentNode = currentNode->next;
        std::free(currentNode->previous);
        currentNode->previous = nullptr;
    }
    head = currentNode;
    count -= k;
}

template<typename T>
void linked_list<T>::removeLast(int k) {
    if (k <= 0 || isEmpty()) {
        return;
    } else if (k >= count) {
        removeAll();
        return;
    }
    node<T> *currentNode = tail;
    for (int i = 0; i < k; i++) {
        currentNode = currentNode->previous;
        std::free(currentNode->next);
        currentNode->next = nullptr;
    }
    tail = currentNode;
    count -= k;
}

template<typename T>
void linked_list<T>::removeAt(int index) {
    if (index < 0 || index >= count) {
        return;
    }
    if (count == 1) {
        removeAll();
        return;
    }
    // Use removeFirst
    if (index == 0) {
        removeFirst();
        return;
    }
    // Use removeLast
    if (index == count - 1) {
        removeLast();
        return;
    }
    if (count - index < index) {
        node<T> *currentNode = tail;
        for (int i = count - 1; i != index; i--) {
            currentNode = currentNode->previous;
        }
        currentNode->previous->next = currentNode->next;
        currentNode->next->previous = currentNode->previous;
        delete currentNode;

    } else {
        node<T> *currentNode = head;
        for (int i = 0; i != index; i++) {
            currentNode = currentNode->next;
        }
        currentNode->previous->next = currentNode->next;
        currentNode->next->previous = currentNode->previous;
        delete currentNode;
    }
    count--;
}

template<typename T>
void linked_list<T>::print() {
    if (count == 0) {
        std::cout << "[]" << std::endl;
    } else {
        node<T> *currentNode = head;
        std::cout << "[" << currentNode->value;
        while (currentNode->next != nullptr) {
            currentNode = currentNode->next;
            T value = currentNode->value;
            std::cout << ", " << value;
        }
        std::cout << "]" << std::endl;
    }
}

