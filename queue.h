#pragma once

#include <iostream>

template <typename T>
struct Node { 
  T data; 
  Node *next; 
  Node(T d) {
    data = d; 
    next = NULL; 
  } 
}; 

template <typename T>
struct Queue { 
  Node<T> *head, *tail; 
  int _size = 0;

  Queue() {
    head = tail = NULL; 
  } 

  // check if queue is empty
  bool empty() {
    return head == NULL;
  }

  // return item at index
  T get(int i) {
    int count = 0;
    Node<T> *curr = head;
    while (curr->next != NULL) {
      if (i == count)
        return curr->data;
      curr = curr->next;
      ++count;
    }
    if(i == count) {
      return tail->data;
    }

    return NULL;
  }

  // print linked list -- provided function must be able to print node->data type
  template<typename Func>
  void debug_print (Func f) {
    Node<T> *n = head;
    write(1, "[", 1);
    while (n != NULL) {
      f(n->data);
      n = n->next;
      write(1, ", ", 2);
    }
    write(1, "]", 1);
  }

  int size() {
    return _size;
  }

  // return last element in runqueue
  T get_tail() {
    if (tail == NULL) {
      return NULL;
    }
    return tail->data;
  }

  // add tcb to back of queue
  void enqueue(T x) {
    if(x == NULL)  {
        return;
    }

    Node<T> *tmp = new Node<T>(x); 

    if (tail == NULL) { 
      head = tail = tmp;
      ++_size;
      return;
    } 

    tail->next = tmp; 
    tail = tmp;
    tail->next = NULL;
    ++_size;
  }

  // remove and return tcb from front of queue
  T dequeue() {
    if (head == NULL) {
      return NULL; 
    }

    Node<T> *tmp = head; 
    T res = tmp->data;
    head = head->next; 

    if (head == NULL) {
      tail = NULL; 
    }

    delete (tmp); 
    --_size;
    return res;
  }

  void clear() {
     /* deref head_ref to get the real head */
    Node<T> *curr = head;
    Node<T> *next = NULL;

    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }

    head = tail = NULL;

    _size = 0;
  }

  // return tcb at front of queue
  T peek() {
    if (head == NULL) {
      return NULL; 
    }
    return head->data;
  } 
};

