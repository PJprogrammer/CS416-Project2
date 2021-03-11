// File:	hashmap.h

// List all group member's name: Marko Elez (me470), Paul John (pj242)
// username of iLab:
// iLab Server: cd.cs.rutgers.edu

#pragma once

#include <iostream>

template<typename K, typename V> 
class HNode {
  public: 
    V value; 
    K key; 

    HNode(K key, V value) {
      this->value = value; 
      this->key = key; 
    } 
}; 

template<typename K, typename V> 
class HashMap {

  HNode<K,V> **arr; 
  int capacity; 
  int msize; 
  HNode<K,V> *dummy; 

  public: 
  HashMap() {
    capacity = 205;
    msize=0; 
    arr = new HNode<K,V>*[capacity]; 
    for (int i=0 ; i < capacity ; i++) {
      arr[i] = NULL;
    }
    dummy = new HNode<K,V>(-1, NULL); 
  } 
  
  int hash(K key) {
    return key % capacity; 
  } 

  void put(K key, V value) {
    HNode<K,V> *tmp = new HNode<K,V>(key, value); 
    int hash_idx = hash(key); 

    while (arr[hash_idx] != NULL && arr[hash_idx]->key != key && arr[hash_idx]->key != -1) {
      ++hash_idx; 
      hash_idx %= capacity; 
    } 

    if (arr[hash_idx] == NULL || arr[hash_idx]->key == -1) {
      msize++;
    }
    arr[hash_idx] = tmp; 
  } 

  V remove(int key) {
    int hash_idx = hash(key); 

    while (arr[hash_idx] != NULL) {
      if (arr[hash_idx]->key == key) {
        HNode<K,V> *tmp = arr[hash_idx]; 
        arr[hash_idx] = dummy; 
        --msize; 
        return tmp->value; 
      } 
      ++hash_idx;
      hash_idx %= capacity; 
    } 
    return NULL; 
  } 

  V get(int key) {
    int hash_idx = hash(key); 
    int counter=0; 

    while (arr[hash_idx] != NULL) {
      int counter = 0; 
      if (counter++ > capacity) {
        return NULL;
      }

      if (arr[hash_idx]->key == key) {
        return arr[hash_idx]->value; 
      }

      hash_idx++; 
      hash_idx %= capacity; 
    } 

    return NULL; 
  } 

  int size() {
    return msize; 
  } 

  bool isEmpty() {
    return msize == 0; 
  } 

  // Print all items in hashmap --> for debugging
  void print() {
    for (int i = 0; i < capacity; ++i) {
      if (arr[i] != NULL && arr[i]->key != -1) {
        std::cout << "K: " << arr[i]->key <<" V: "<< arr[i]->value << std::endl;
      }
    } 
  }

}; 

