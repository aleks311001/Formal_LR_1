#include <iostream>
#include "Algo.h"
#include <gtest/gtest.h>
#include "Test.h"

TEST_F(TestGrammar, testAddSymbol) {
    testAddTerminalSymbol();
    testAddNonTerminalSymbol();
}

TEST_F(TestGrammar, testSetStartSymbol) {
    testSetStartSymbol();
}

TEST_F(TestGrammar, testAddRule) {
    testAddRule();
}

TEST(TestSituation, testOperators) {
    Situation<char> situation1 = {.left = 'S',
            .right = "SaSb",
            .point = 0,
            .first = 'a'};

    ASSERT_EQ(situation1, situation1);

    Situation<char> situation2 = {.left = 'S',
            .right = "SbSb",
            .point = 0,
            .first = 'a'};

    ASSERT_NE(situation1, situation2);
    ASSERT_LT(situation1, situation2);

    situation2.right = situation1.right;
    situation1.point = 1;
    ASSERT_GT(situation1, situation2);
}

TEST(TestSituation, testIsEnd) {
    Situation<char> situation = {.left = 'S',
                                 .right = "SaSb",
                                 .point = 0,
                                 .first = 'a'};

    ASSERT_FALSE(situation.isEnd());
    situation.point = 4;
    ASSERT_TRUE(situation.isEnd());
}

TEST(TestSituation, testGetNext) {
    Situation<char> situation = {.left = 'S',
            .right = "SaSb",
            .point = 3,
            .first = 'a'};
    Situation<char> situationNext = situation;
    situationNext.point = 4;

    ASSERT_EQ(situation.getNext(), situationNext);
    ASSERT_THROW(situationNext.getNext(), std::range_error);
}

TEST(TestSituation, testGetSymbol) {
    Situation<char> situation = {.left = 'S',
                                 .right = "SaSb",
                                 .point = 3,
                                 .first = 'a'};

    ASSERT_EQ(situation.getSymbol(), 'b');
    ASSERT_EQ(situation.getNext().getSymbol(), 'a');
}

TEST_F(TestNode, testAddNextSituations) {
    testAddNextSituation();
}

TEST_F(TestNode, testOperators) {
    testOperators();
}

TEST_F(TestNode, testGoTo) {
    testGoTo();
}

TEST_F(TestNode, testGetFinish) {
    testGetFinish();
}

TEST_F(TestAlgo, testFindEpsGeneratingAndFirst) {
    testFindEpsGeneratingAndFirst();
}

TEST_F(TestAlgo, testGetFirstFunction) {
    testGetFirstFunction();
}

TEST_F(TestAlgo, testFit) {
    testFit();
}

TEST_F(TestAlgo, testBacktrace) {
    testBacktrace();
}

TEST_F(TestAlgo, testPredict) {
    testPredict();
}
