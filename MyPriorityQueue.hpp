#ifndef __PROJ5_PRIORITY_QUEUE_HPP
#define __PROJ5_PRIORITY_QUEUE_HPP

#include <vector>

#include "runtimeexcept.hpp"

class PriorityQueueEmptyException : public RuntimeException {
 public:
  PriorityQueueEmptyException(const std::string& err) : RuntimeException(err) {}
};

template <typename Object>
class MyPriorityQueue {
 private:
  std::vector<Object> min_heap;

 public:
  MyPriorityQueue() {}
  ~MyPriorityQueue() {}
  size_t size() const noexcept;
  bool isEmpty() const noexcept;
  void insert(const Object& elem);

  const Object& min() const;

  void extractMin();
};

template <typename Object>
size_t MyPriorityQueue<Object>::size() const noexcept {
  return min_heap.size();
}

template <typename Object>
bool MyPriorityQueue<Object>::isEmpty() const noexcept {
  return min_heap.empty();
}

template <typename Object>
void MyPriorityQueue<Object>::insert(const Object& elem) {
  min_heap.push_back(elem);
  if (min_heap.size() == 1) return;
  unsigned child = min_heap.size() - 1;
  unsigned parent = (child - 1) / 2;
  while (child != 0 && elem < min_heap[parent]) {
    std::swap(min_heap[parent], min_heap[child]);
    child = parent;
    parent = (child - 1) / 2;
  }
}

template <typename Object>
const Object& MyPriorityQueue<Object>::min() const {
  if (min_heap.empty()) throw PriorityQueueEmptyException("Empty");
  return min_heap[0];
}

template <typename Object>
void MyPriorityQueue<Object>::extractMin() {
  if (min_heap.empty()) throw PriorityQueueEmptyException("Empty");
  std::swap(min_heap[0], min_heap[min_heap.size() - 1]);
  min_heap.pop_back();
  if (min_heap.size() <= 1)
    return;
  else if (min_heap.size() == 2) {
    if (min_heap[1] < min_heap[0]) {
      std::swap(min_heap[1], min_heap[0]);
    }
  } else {
    unsigned parent = 0;
    unsigned lchild = (2 * parent) + 1;
    unsigned rchild = (2 * parent) + 2;
    while (lchild < min_heap.size()) {
      // End the loop when parent becomes a leaf node.
      if (rchild < min_heap.size()) {
        // Right node exists.
        if ((min_heap[lchild] <= min_heap[rchild]) &&
            (min_heap[lchild] < min_heap[parent])) {
          // Swap with smaller child.
          std::swap(min_heap[parent], min_heap[lchild]);
          parent = lchild;
        } else if ((min_heap[rchild] < min_heap[lchild]) &&
                   (min_heap[rchild] < min_heap[parent])) {
          std::swap(min_heap[parent], min_heap[rchild]);
          parent = rchild;
        } else {
          break;
        }
      } else {
        if (min_heap[lchild] < min_heap[parent]) {
          std::swap(min_heap[parent], min_heap[lchild]);
          parent = lchild;
        } else {
          break;
        }
      }
      // Update child indices.
      lchild = (2 * parent) + 1;
      rchild = (2 * parent) + 2;
    }
  }
}

#endif
