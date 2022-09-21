//
// Created by Noah Wilder on 2022-09-21.
//

#ifndef C___DATA_TYPES_AND_FUNCTIONS_LINKED_LIST_H
#define C___DATA_TYPES_AND_FUNCTIONS_LINKED_LIST_H


#include <iostream>
#include <optional>

template<typename T>
class linked_list {
private:

    template<typename U>
    struct node {
        U value;
        node<U> *next = nullptr;
        node<U> *previous = nullptr;

        explicit node(U value) {
            this->value = value;
        }
    };

    node<T> *head;
    node<T> *tail;
    int count;

public:

    linked_list();
    linked_list(std::initializer_list<T> list);

    ~linked_list();

    bool isEmpty() const;
    int size() const;

    T& operator[] (int index);

    std::optional<T> first();
    std::optional<T> last();

    void append(T element);
    template <typename... U, typename = std::enable_if_t<std::conjunction_v<std::is_same<T, U>...>>>
    void append(T element, U... elements);

    void prepend(T element);
    template <typename... U, typename = std::enable_if_t<std::conjunction_v<std::is_same<T, U>...>>>
    void prepend(T element, U... elements);

    void removeAll();

    std::optional<T> popFirst();
    std::optional<T> popLast();

    T removeFirst();
    T removeLast();

    void removeFirst(int k);
    void removeLast(int k);

    void removeAt(int index);

    void print();
};



#endif //C___DATA_TYPES_AND_FUNCTIONS_LINKED_LIST_H
