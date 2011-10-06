/*
 *   Copyright (c) 2011 Higepon(Taro Minowa) <higepon@users.sourceforge.jp>
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef BST_H_
#define BST_H_

#include <assert.h>
#include <stdlib.h>
#ifndef MONA
#include <vector>
#include <algorithm>
#endif

template <class T, class S> class BinarySearchTree {
 public:
  BinarySearchTree() : root_(NULL) {
  }

  virtual ~BinarySearchTree() {
  }

  void Add(const T key, S value) {
    if (root_ == NULL) {
      root_ = new Node(key, value);
      assert(CheckSanity());
      return;
    }
    // N.B.
    // Since this is *NOT* a balanced tree,
    // we use iterative approach instread of recursive one
    // to avoid stack overflow.
    for (Node* current = root_;;) {
      if (current->key == key) {
        current->value = value;
        assert(CheckSanity());
        return;
      } else if (current->key < key) {
        if (current->right == NULL) {
          current->right = new Node(key, value);
          assert(CheckSanity());
          return;
        } else {
          current = current->right;
        }
      } else {
        if (current->left == NULL) {
          current->left = new Node(key, value);
          assert(CheckSanity());
          return;
        } else {
          current = current->left;
        }
      }
    }
  }

  S GetLowerNearest(const T key) const {
    Node* last_node_lt_key = NULL;
    Node* n = root_;

    while (n != NULL) {
      if (n->key == key) {
        return n->value;
      } else if (n->key < key) {
        last_node_lt_key = n;
        n = n->right;
      } else {
        n = n->left;
      }
    }
    if (last_node_lt_key == NULL) {
      return S();
    } else {
      return last_node_lt_key->value;
    }
  }

  S Get(const T key, bool* found = NULL) const {
    for (Node* current = root_;;) {
      if (current == NULL) {
        if (found) {
          *found = false;
        }
        return S();
      } else if (current->key == key) {
        if (found) {
          *found = true;
        }
        return current->value;
      } else if (current->key < key) {
          current = current->right;
      } else {
          current = current->left;
      }
    }
  }

  void Remove(const T key, bool* removed = NULL) {
    Node* parent = NULL;
    for (Node* current = root_;;) {
      if (current == NULL) {
        if (removed) {
          *removed = false;
        }
        assert(CheckSanity());
        return;
      } else if (current->key == key) {
        RemoveNode(parent, current);
        if (removed) {
          *removed = true;
        }
        assert(CheckSanity());
        return;
      } else if (current->key < key) {
        parent = current;
        current = current->right;
      } else {
        parent = current;
        current = current->left;
      }
    }
  }


  bool CheckSanity() const {
#ifdef MONA
    return true;
#else
    std::vector<T> keys;
    GetInOrder(root_, &keys);
    std::vector<T> sorted = keys;
    sort(keys.begin(), keys.end());
    return keys == sorted;
#endif
  }

 private:
  struct Node {
    Node(const T key, S value, Node* left = NULL, Node* right = NULL) :
        key(key),
        value(value),
        left(left),
        right(right) {}

    bool IsLeaf() const {
      return left == NULL && right == NULL;
    }

    bool HasOnlyLeftChild() const {
      return (left != NULL && right == NULL);
    }

    bool HasOnlyRightChild() const {
      return (left == NULL && right != NULL);
    }

    T key;
    S value;
    Node* left;
    Node* right;
  };

  void RemoveNode(Node* parent, Node* node) {
    if (node->IsLeaf()) {
      if (parent == NULL) {
        root_ = NULL;
      } else if (parent->left == node) {
        parent->left = NULL;
      } else {
        parent->right = NULL;
      }
      delete node;
    } else if (node->HasOnlyLeftChild()) {
      if (parent->left == node) {
        parent->left = node->left;
      } else {
        parent->right = node->left;
      }
      delete node;
    } else if (node->HasOnlyRightChild()) {
      if (parent->left == node) {
        parent->left = node->right;
      } else {
        parent->right = node->right;
      }
      delete node;
    } else {
      Node* parentOfLeftMost = node;
      Node* leftMost = node->right;
      while (leftMost->left) {
        parentOfLeftMost = leftMost;
        leftMost = leftMost->left;
      }
      if (parentOfLeftMost->left == leftMost) {
        parentOfLeftMost->left = NULL;
      } else {
        parentOfLeftMost->right = NULL;
      }
      node->key = leftMost->key;
      node->value = leftMost->value;
      delete leftMost;
    }
  }

  void GetInOrder(Node* node, std::vector<T>* keys) const {
    if (node == NULL) {
      return;
    }
    GetInOrder(node->left, keys);
    keys->push_back(node->key);
    GetInOrder(node->right, keys);
  }

  Node* root_;
};

#endif  // BST_H_
