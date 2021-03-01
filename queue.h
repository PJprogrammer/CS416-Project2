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
    while (curr->next != tail) {
      if (i == count)
        return curr->data;
      curr = curr->next;
      ++count;
    }
    // failed
    return NULL;
  }

  // print linked list -- provided function must be able to print node->data type
  void func (void (*print)(int)) {
    Node<T> *n = head; 
    while (n != NULL) {
      (*print)(n->data);
        n = n->next;
    }
  }

  int size() {
    /*
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
    */
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
      return; 
    } 

    tail->next = tmp; 
    tail = tmp; 
    ++_size;
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
  }

  // return tcb at front of queue
  T peek() {
    if (head == NULL) {
      return NULL; 
    }
    return head->data;
  } 
}; 

