//
// Created by aleks311001 on 18.12.2020.
//

#ifndef LR_1__TEST_H_
#define LR_1__TEST_H_

#include <gtest/gtest.h>
#include "Algo.h"
#include "TestUtils.h"

class TestGrammar: public ::testing::Test {
protected:
    Grammar<char> grammar;

    void testAddNonTerminalSymbol() {
        grammar.addNonTerminalSymbol({'T', 'S'});
        EXPECT_TRUE(equalContainers(grammar.nonTerminal_, {'S', 'T'}));

        grammar.addNonTerminalSymbol('S')
               .addNonTerminalSymbol({'A', 'B', 'C'})
               .addNonTerminalSymbol('D');
        EXPECT_TRUE(equalContainers(grammar.nonTerminal_, {'S', 'T', 'A', 'B', 'C', 'D'}));

        grammar.addTerminalSymbol({'a', 'b'});
        ASSERT_THROW(grammar.addNonTerminalSymbol('a'), std::range_error);
        ASSERT_THROW(grammar.addNonTerminalSymbol({'b', 'c'}), std::range_error);
    }

    void testAddTerminalSymbol() {
        grammar.addTerminalSymbol({'a', 'f'});
        EXPECT_TRUE(equalContainers(grammar.terminal_, {'a', 'f'}));

        grammar.addTerminalSymbol('g')
               .addTerminalSymbol('f');
        EXPECT_TRUE(equalContainers(grammar.terminal_, {'a', 'f', 'g'}));

        grammar.addNonTerminalSymbol({'S', 'T'});
        ASSERT_THROW(grammar.addTerminalSymbol('S'), std::range_error);
        ASSERT_THROW(grammar.addTerminalSymbol({'T', 'P'}), std::range_error);
    }

    void testSetStartSymbol() {
        ASSERT_THROW(grammar.setStartSymbol('S'), std::range_error);

        grammar.addNonTerminalSymbol('S')
               .setStartSymbol('S');

        ASSERT_EQ(grammar.start_, 'S');
    }

    void testAddRule() {
        grammar.addNonTerminalSymbol('T');

        ASSERT_THROW(grammar.addRule('S', "T"), std::range_error);
        ASSERT_THROW(grammar.addRule('T', "TST"), std::range_error);
        ASSERT_THROW(grammar.addRule('T', "Ta"), std::range_error);

        grammar.addTerminalSymbol({'a', 'b'})
               .addNonTerminalSymbol('S')
               .addRule('S', "aTbT")
               .addRule('S', "ST");

        EXPECT_TRUE(equalContainers(grammar.rules_, {{'S', std::string("aTbT")}, {'S', std::string("ST")}}));
    }
};

class TestSituation: public ::testing::Test {
protected:

};

#endif //LR_1__TEST_H_
