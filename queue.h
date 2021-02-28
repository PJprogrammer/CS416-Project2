#include <iostream>
#include "rpthread.h"

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

  Queue() {
    head = tail = NULL; 
  } 

  // check if queue is empty
  bool empty() {
    return head == NULL;
  }

  int size() {
    if (tail == NULL || head == NULL) {
      return 0;
    }
    if (tail == head) {
      return 1;
    }
  
    int res = 0;
    Node<T> *tmp = head; 
    while (tmp->next != tail) {
      tmp = tmp->next;
      ++res;
    }
    
    return res;

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
    Node<T> *tmp = new Node<T>(x); 

    if (tail == NULL) { 
      head = tail = tmp; 
      return; 
    } 

    tail->next = tmp; 
    tail = tmp; 
  } 

  // add tcb to front of queue
  void push(T x) {
    Node<T> *tmp = new Node<T>(x); 

    if (tail == NULL) { 
      head = tail = tmp; 
      return; 
    }

    tmp->next = head;
    head = tmp;
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
    return res;
  }

  // remove and return tcb at back of queue
  T pop_back() {
    if (head == NULL) {
      return NULL; 
    }

    if (head->next == NULL) {
      head = tail = NULL;
    }

    Node<T> *tmp = head; 
    while (tmp->next != tail) {
      tmp = tmp->next;
    }
    
    T res = tail->data;
    tmp->next = NULL;
    tail = tmp;

    return res;
  }

  // return tcb at front of queue
  T peek() {
    if (head == NULL) {
      return NULL; 
    }
    return head->data;
  } 
}; 

