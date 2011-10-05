/*
 *   Copyright (c) 2011  Higepon(Taro Minowa)  <higepon@users.sourceforge.jp>
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
#include <stdint.h>
#include <gtest/gtest.h>
#include <algorithm>
#include "./bst.h"

namespace {

class BinarySearchTreeTest : public ::testing::Test {
 protected:
  BinarySearchTree<int, int> bst_;
 public:
  BinarySearchTreeTest()  {
  }

  virtual ~BinarySearchTreeTest() {
  }
};

TEST_F(BinarySearchTreeTest, NotFoundWhenEmpty) {
  bool found = true;
  EXPECT_EQ(0, bst_.Get(1, &found));
  EXPECT_FALSE(found);
}

TEST_F(BinarySearchTreeTest, CanAddAndGet) {
  bool found = false;
  bst_.Add(1, 2);
  EXPECT_EQ(2, bst_.Get(1, &found));
  EXPECT_TRUE(found);
}

TEST_F(BinarySearchTreeTest, AddTwoValuesGetBoth) {
  // 1
  //   3
  bst_.Add(1, 2);
  bst_.Add(3, 4);
  bool found = false;
  EXPECT_EQ(2, bst_.Get(1, &found));
  EXPECT_TRUE(found);

  bool found2 = false;
  EXPECT_EQ(4, bst_.Get(3, &found2));
  EXPECT_TRUE(found2);
}

TEST_F(BinarySearchTreeTest, AddTwoValuesGetBoth2) {
  //   3
  // 1
  bst_.Add(3, 4);
  bst_.Add(1, 2);
  bool found = false;
  EXPECT_EQ(2, bst_.Get(1, &found));
  EXPECT_TRUE(found);

  bool found2 = false;
  EXPECT_EQ(4, bst_.Get(3, &found2));
  EXPECT_TRUE(found2);
}

TEST_F(BinarySearchTreeTest, OverWriteValue) {
  bst_.Add(3, 4);
  EXPECT_EQ(4, bst_.Get(3));
  bst_.Add(3, 5);
  EXPECT_EQ(5, bst_.Get(3));
}

TEST_F(BinarySearchTreeTest, ComplexTreeAndGet) {
  //      8
  //   5     10
  // 4   7 9    11
  bst_.Add(8, 8);
  bst_.Add(10, 10);
  bst_.Add(5, 5);
  bst_.Add(11, 11);
  bst_.Add(9, 9);
  bst_.Add(4, 4);
  bst_.Add(7, 7);

  EXPECT_EQ(4, bst_.Get(4));
  EXPECT_EQ(5, bst_.Get(5));
  EXPECT_EQ(7, bst_.Get(7));
  EXPECT_EQ(8, bst_.Get(8));
  EXPECT_EQ(9, bst_.Get(9));
  EXPECT_EQ(10, bst_.Get(10));
  EXPECT_EQ(11, bst_.Get(11));
}

TEST_F(BinarySearchTreeTest, RemoveFailWhenEmpty) {
  bool removed = true;
  bst_.Remove(8, &removed);
  EXPECT_FALSE(removed);
}

TEST_F(BinarySearchTreeTest, RemoveOnlyOneRoot) {
  // 8
  bst_.Add(8, 8);
  bool removed = false;
  bst_.Remove(8, &removed);

  bool found = true;
  EXPECT_EQ(0, bst_.Get(8, &found));
  EXPECT_FALSE(found);
  EXPECT_TRUE(removed);
}

TEST_F(BinarySearchTreeTest, RemoveLeaf) {
  //   8
  // 5   10
  bst_.Add(8, 8);
  bst_.Add(10, 10);
  bst_.Add(5, 5);
  bool removed = false;
  bst_.Remove(5, &removed);
  EXPECT_TRUE(removed);

  bool found = true;
  EXPECT_EQ(0, bst_.Get(5, &found));
  EXPECT_FALSE(found);
}

TEST_F(BinarySearchTreeTest, RemoveLeaf2) {
  //   8
  // 5   10
  bst_.Add(8, 8);
  bst_.Add(10, 10);
  bst_.Add(5, 5);
  bool removed = false;
  bst_.Remove(10, &removed);
  EXPECT_TRUE(removed);

  bool found = true;
  EXPECT_EQ(0, bst_.Get(10, &found));
  EXPECT_FALSE(found);
}

TEST_F(BinarySearchTreeTest, RemoveNodeWithOneChild) {
  //   8
  //     10
  //        11
  bst_.Add(8, 8);
  bst_.Add(10, 10);
  bst_.Add(11, 11);
  bool removed = false;
  bst_.Remove(10, &removed);
  EXPECT_TRUE(removed);

  bool found = true;
  EXPECT_EQ(0, bst_.Get(10, &found));
  EXPECT_FALSE(found);

  bool found2 = false;
  EXPECT_EQ(11, bst_.Get(11, &found2));
  EXPECT_TRUE(found2);
}

TEST_F(BinarySearchTreeTest, RemoveNodeWithOneChild2) {
  //   8
  //      10
  //    9
  bst_.Add(8, 8);
  bst_.Add(10, 10);
  bst_.Add(9, 9);
  bool removed = false;
  bst_.Remove(10, &removed);
  EXPECT_TRUE(removed);

  bool found = true;
  EXPECT_EQ(0, bst_.Get(10, &found));
  EXPECT_FALSE(found);

  bool found2 = false;
  EXPECT_EQ(9, bst_.Get(9, &found2));
  EXPECT_TRUE(found2);
}

TEST_F(BinarySearchTreeTest, RemoveNodeWithOneChild3) {
  //   8
  //  5
  // 4
  bst_.Add(8, 8);
  bst_.Add(5, 5);
  bst_.Add(4, 4);
  bool removed = false;
  bst_.Remove(5, &removed);
  EXPECT_TRUE(removed);

  bool found = true;
  EXPECT_EQ(0, bst_.Get(5, &found));
  EXPECT_FALSE(found);

  bool found2 = false;
  EXPECT_EQ(4, bst_.Get(4, &found2));
  EXPECT_TRUE(found2);
}

TEST_F(BinarySearchTreeTest, RemoveNodeWithOneChild4) {
  //     8
  //   5
  //     6
  bst_.Add(8, 8);
  bst_.Add(5, 5);
  bst_.Add(6, 6);
  bool removed = false;
  bst_.Remove(5, &removed);
  EXPECT_TRUE(removed);

  bool found = true;
  EXPECT_EQ(0, bst_.Get(5, &found));
  EXPECT_FALSE(found);

  bool found2 = false;
  EXPECT_EQ(6, bst_.Get(6, &found2));
  EXPECT_TRUE(found2);
}

TEST_F(BinarySearchTreeTest, RemoveNodeWithTwoChildren) {
  //     3
  // 1         5
  //        4     7
  //            6
  bst_.Add(3, 3);
  bst_.Add(1, 1);
  bst_.Add(5, 5);
  bst_.Add(4, 4);
  bst_.Add(7, 7);
  bst_.Add(6, 6);
  bool removed = false;
  bst_.Remove(5, &removed);
  EXPECT_TRUE(removed);

  bool found = true;
  EXPECT_EQ(0, bst_.Get(5, &found));
  EXPECT_FALSE(found);

  bool found2 = false;
  EXPECT_EQ(6, bst_.Get(6, &found2));
  EXPECT_TRUE(found2);
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
