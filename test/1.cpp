/*************************************************************************
    > File Name: 1.cpp
    > Author: zxy
    > Mail: zhaoxueyan131@gmail.com 
    > Created Time: Tue Jul 28 22:48:21 2020
 ************************************************************************/
#include <iostream>
#include <gtest/gtest.h>

int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return a - b;
}

// case1
TEST(test, c1) {
    EXPECT_EQ(3, add(1, 2));  // pass
    EXPECT_EQ(12, add(2, 6));  // fail
}

// case2
TEST(test, c2) {
    EXPECT_EQ(-1, sub(1, 2));  // pass
}

GTEST_API_ int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}