#ifndef QUEUE_IMPL_H
#define QUEUE_IMPL_H
#include <cassert>
#include <cstddef>

template <typename T>
Queue<T>::Queue() : sz(0), head(nullptr), tail(nullptr) { }

template <typename T>
Queue<T>::~Queue() {
    while(!empty()) {
        pop();
    }
}

template <typename T>
void Queue<T>::push(T t) {
    Node<T> *new_node = new Node<T>(t);
    assert(new_node != nullptr);
    if (empty()) {
        head = new_node;
        tail = new_node;
    }
    else {
        tail->next = new_node;
        tail=new_node;
    }
    sz += 1;
}

template <typename T>
void Queue<T>::pop() {
    assert(head != nullptr);
    Node<T>* temp = head;
    head = head->next;
    delete temp;
    sz -= 1;
    if (empty())
        tail = nullptr;
}

template <typename T>
T &Queue<T>::front() {
    assert(head != nullptr);
    return head->val;
}

template <typename T>
bool Queue<T>::empty() const {
    return sz == 0;
}

template <typename T>
size_t Queue<T>::size() const {
    return sz;
}
#endif
