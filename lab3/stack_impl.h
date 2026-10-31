#ifndef STACK_IMPL_H
#define STACK_IMPL_H
#include <cassert>
#include <cstddef>

template <typename T>
Stack<T>::Stack() : sz(0), head(nullptr) { }

template <typename T>
Stack<T>::~Stack() {
    while (!empty()) {
        pop();
    }
}

template <typename T>
void Stack<T>::push(T t) {
    Node<T>* new_head = new Node<T>(t);
    assert(new_head != nullptr);
    new_head->next = head;
    head = new_head;
    sz += 1;
}

template <typename T>
void Stack<T>::pop() {
    assert(head != nullptr);
    Node<T>* temp = head;
    head = head->next;
    delete temp;
    sz -= 1;
}

template <typename T>
T& Stack<T>::top() {
    assert(head != nullptr);
    return head->val;
}

template <typename T>
bool Stack<T>::empty() const {
    return sz == 0;
}

template <typename T>
size_t Stack<T>::size() const {
    return sz;
}

#endif
