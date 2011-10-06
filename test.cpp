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
#include <string>
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

  void BuildBST(const std::string& s) {
    for (std::string::const_iterator it = s.begin(); it != s.end(); ++it) {
      if (*it == ' ') {
        continue;
      }
      int v = ((*it) - '0') + 0;
      bst_.Add(v, v);
    }
  }

  void CheckRemoved(int key_to_remove, int key_should_be_found) {
    bool removed = false;
    bst_.Remove(key_to_remove, &removed);
    EXPECT_TRUE(removed);

    bool found = true;
    EXPECT_EQ(0, bst_.Get(key_to_remove, &found));
    EXPECT_FALSE(found);

    bool found2 = false;
    EXPECT_EQ(key_should_be_found, bst_.Get(key_should_be_found, &found2));
    EXPECT_TRUE(found2);
  }

  void BuildAndCheckRemoved(const std::string& tree,
                    int key_to_remove, int key_should_be_found) {
    BuildBST(tree);
    CheckRemoved(key_to_remove, key_should_be_found);
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
  BuildBST("1 3");
  bool found = false;
  EXPECT_EQ(1, bst_.Get(1, &found));
  EXPECT_TRUE(found);

  bool found2 = false;
  EXPECT_EQ(3, bst_.Get(3, &found2));
  EXPECT_TRUE(found2);
}

TEST_F(BinarySearchTreeTest, AddTwoValuesGetBoth2) {
  //   3
  // 1
  BuildBST("3 1");
  bool found = false;
  EXPECT_EQ(1, bst_.Get(1, &found));
  EXPECT_TRUE(found);

  bool found2 = false;
  EXPECT_EQ(3, bst_.Get(3, &found2));
  EXPECT_TRUE(found2);
}

TEST_F(BinarySearchTreeTest, OverWriteValue) {
  bst_.Add(3, 4);
  EXPECT_EQ(4, bst_.Get(3));
  bst_.Add(3, 5);
  EXPECT_EQ(5, bst_.Get(3));
}

TEST_F(BinarySearchTreeTest, ComplexTreeAndGet) {
  //      6
  //   4     8
  // 3   5 7    9
  BuildBST("6 4 8 3 5 7 9");

  EXPECT_EQ(3, bst_.Get(3));
  EXPECT_EQ(4, bst_.Get(4));
  EXPECT_EQ(5, bst_.Get(5));
  EXPECT_EQ(7, bst_.Get(7));
  EXPECT_EQ(8, bst_.Get(8));
  EXPECT_EQ(9, bst_.Get(9));
}

TEST_F(BinarySearchTreeTest, RemoveFailWhenEmpty) {
  bool removed = true;
  bst_.Remove(8, &removed);
  EXPECT_FALSE(removed);
}

TEST_F(BinarySearchTreeTest, RemoveOnlyOneRoot) {
  // 8
  BuildBST("8");
  bool removed = false;
  bst_.Remove(8, &removed);

  bool found = true;
  EXPECT_EQ(0, bst_.Get(8, &found));
  EXPECT_FALSE(found);
  EXPECT_TRUE(removed);
}

TEST_F(BinarySearchTreeTest, RemoveLeaf) {
  //   2
  // 1   3
  int key_to_remove = 1;
  int key_should_be_found = 2;
  BuildAndCheckRemoved("2 1 3", key_to_remove, key_should_be_found);
}

TEST_F(BinarySearchTreeTest, RemoveLeaf2) {
  //   2
  // 1   3
  int key_to_remove = 3;
  int key_should_be_found = 1;
  BuildAndCheckRemoved("2 1 3", key_to_remove, key_should_be_found);
}

TEST_F(BinarySearchTreeTest, RemoveNodeWithOneChild) {
  //   1
  //     2
  //       3
  int key_to_remove = 2;
  int key_should_be_found = 3;
  BuildAndCheckRemoved("1 2 3", key_to_remove, key_should_be_found);
}

TEST_F(BinarySearchTreeTest, RemoveNodeWithOneChild2) {
  //   6
  //      9
  //    7
  int key_to_remove = 9;
  int key_should_be_found = 7;
  BuildAndCheckRemoved("6 9 7", key_to_remove, key_should_be_found);
}

TEST_F(BinarySearchTreeTest, RemoveNodeWithOneChild3) {
  //   8
  //  5
  // 4
  int key_to_remove = 5;
  int key_should_be_found = 4;
  BuildAndCheckRemoved("8 5 4", key_to_remove, key_should_be_found);
}

TEST_F(BinarySearchTreeTest, RemoveNodeWithOneChild4) {
  //     8
  //   5
  //     6
  int key_to_remove = 5;
  int key_should_be_found = 6;
  BuildAndCheckRemoved("8 5 6", key_to_remove, key_should_be_found);
}

TEST_F(BinarySearchTreeTest, RemoveNodeWithTwoChildren) {
  //     3
  // 1         5
  //        4     7
  //            6
  int key_to_remove = 5;
  int key_should_be_found = 6;
  BuildAndCheckRemoved("3 1 5 4 7 6", key_to_remove, key_should_be_found);
}

TEST_F(BinarySearchTreeTest, RemoveAll) {
  //     3
  // 1         5
  //        4     7
  //            6
  BuildBST("3 1 5 4 7 6");
  SCOPED_TRACE("");
  CheckRemoved(3, 1);
  SCOPED_TRACE("");
  CheckRemoved(5, 1);
  SCOPED_TRACE("");
  CheckRemoved(4, 7);
  SCOPED_TRACE("");
  CheckRemoved(6, 7);
  SCOPED_TRACE("");
  CheckRemoved(1, 7);
  bst_.Remove(7);
  EXPECT_TRUE(bst_.CheckSanity());
}

TEST_F(BinarySearchTreeTest, GetLowerNearest) {
  //     3
  // 1         5
  //        4     7
  //            6
  BuildBST("3 1 5 4 7 6");
  EXPECT_EQ(3, bst_.GetLowerNearest(3));
  EXPECT_EQ(1, bst_.GetLowerNearest(2));
  EXPECT_EQ(7, bst_.GetLowerNearest(8));
  EXPECT_EQ(0, bst_.GetLowerNearest(-1));
}

TEST_F(BinarySearchTreeTest, StringAsKey) {
  BinarySearchTree<std::string, std::string> bst;
  bst.Add("abc", "abc");
  bst.Add("zzz", "zzz");
  EXPECT_EQ(std::string("abc"), bst.GetLowerNearest("abd2"));
  EXPECT_EQ(std::string("zzz"), bst.GetLowerNearest("zzzz"));
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
