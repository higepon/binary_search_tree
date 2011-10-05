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

#include <stdlib.h>

template <class T, class S> class BinarySearchTree {
 public:
  BinarySearchTree() : root_(NULL) {
  }

  virtual ~BinarySearchTree() {
  }

  void Add(const T key, S value) {
    if (root_ == NULL) {
      root_ = new Node(key, value);
      return;
    }
    // N.B.
    // Since this is *NOT* a balanced tree,
    // we use iterative approach instread of recursive one
    // to avoid stack overflow.
    for (Node* current = root_;;) {
      if (current->key == key) {
        current->value = value;
        return;
      } else if (current->key < key) {
        if (current->right == NULL) {
          current->right = new Node(key, value);
          return;
        } else {
          current = current->right;
        }
      } else {
        if (current->left == NULL) {
          current->left = new Node(key, value);
          return;
        } else {
          current = current->left;
        }
      }
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
        return;
      } else if (current->key == key) {
        RemoveNode(parent, current);
        if (removed) {
          *removed = true;
        }
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
      parentOfLeftMost->left = NULL;
      node->key = leftMost->key;
      node->value = leftMost->value;
    }
  }

  Node* root_;
};

#endif  // BST_H_
