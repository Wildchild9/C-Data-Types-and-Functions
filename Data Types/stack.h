//
// Created by Noah Wilder on 2022-09-21.
//

#ifndef C___DATA_TYPES_AND_FUNCTIONS_STACK_H
#define C___DATA_TYPES_AND_FUNCTIONS_STACK_H



template <typename T>
class stack {
private:
    int count;
    int allocatedSize;
    T* arr;

public:
    /// Initializes an empty stack.
    stack();

    /// Returns a Boolean indicating whether the stack does not contains elements.
    [[nodiscard]] bool isEmpty() const;

    /// The length of the stack.
    [[nodiscard]] int length() const;

    /// Adds an element to the top of the stack.
    ///
    /// Stacks increase their allocated capacity using an exponential
    /// strategy, so pushing a single element to a stack is an O(1) operation
    /// when averaged over many calls to the `push()` method. When a stack
    /// has additional capacity pushing an element is O(1). When a stack needs to
    /// reallocate storage before pushing, pushing is O(*n*), where *n* is
    /// the length of the stack.
    void push(T element);

    /// Pops an element from the stack. If the stack is empty, the program will exit with error code 1.
    T pop();

    /// Prints the contents of the stack.
    void print();
};


#endif //C___DATA_TYPES_AND_FUNCTIONS_STACK_H
