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

class TestNode: public ::testing::Test {
protected:
    Grammar<char> grammar;
    Node<char> node;
    Situation<char> startSituation;

    TestNode(): grammar(), node(grammar), startSituation() {}

    void SetUp() {
        grammar.addTerminalSymbol({'a', 'b'})
               .addNonTerminalSymbol({'S'})
               .setStartSymbol('S')
               .addRule('S', "")
               .addRule('S', "SaSSb");

        startSituation = {.left = 'S', .right = "SaSSb", .point = 2, .first = '$'};
        node.addSituation(startSituation);
    }

    void testAddNextSituation() {
        auto first = [](const Situation<char>&) {
            return std::set<char>{'a', 'b'};
        };
        node.addNextSituations(startSituation, first);

        std::set<Situation<char>> answer = {{'S', "", 0, 'a'},
                                            {'S', "", 0, 'b'},
                                            {'S', "SaSSb", 0, 'a'},
                                            {'S', "SaSSb", 0, 'b'},
                                            {'S', "SaSSb", 2, '$'}};
        EXPECT_TRUE(equalContainers(node.situations_, answer));
    }

    void testOperators() {
        Node<char> node2(grammar);
        ASSERT_GT(node, node2);

        node2.addSituation({'S', "SaSSb", 2, 'a'});
        ASSERT_LT(node, node2);
        ASSERT_LE(node, node);
    }

    void testGoTo() {
        auto first = [](const Situation<char>&) {
            return std::set<char>{'a', 'b'};
        };

        std::set<Situation<char>> answer = {{'S', "", 0, 'a'},
                                            {'S', "", 0, 'b'},
                                            {'S', "SaSSb", 0, 'a'},
                                            {'S', "SaSSb", 0, 'b'},
                                            {'S', "SaSSb", 3, '$'}};
        EXPECT_TRUE(equalContainers(node.goTo('S', first).situations_, answer));
    }

    void testGetFinish() {
        node.addSituation({'S', "SaSSb", 5, 'a'});

        ASSERT_FALSE(node.getEndSituation('b').isValid);
        ASSERT_TRUE(node.getEndSituation('a').isValid);
        ASSERT_EQ(node.getEndSituation('a').situation, Situation<char>({'S', "SaSSb", 5, 'a'}));

        node.addSituation({'S', "", 0, 'a'});
        ASSERT_THROW(node.getEndSituation('a'), std::runtime_error);
    }
};

class TestAlgo: public ::testing::Test {
protected:
    Grammar<char> grammar;
    Algo<char> algo;

    void makeGrammarForTestFirst() {
        grammar.addTerminalSymbol({'a', 'b'})
               .addNonTerminalSymbol({'S', 'T', 'P'})
               .setStartSymbol('S')
               .addRule('S', "")
               .addRule('T', "SSS")
               .addRule('P', "PS")
               .addRule('P', "Pa")
               .addRule('P', "bS");
    }

    void testFindEpsGeneratingAndFirst() {
        makeGrammarForTestFirst();

        algo.findEpsGenerating_(grammar);
        EXPECT_TRUE(equalContainers(algo.epsGenerating_, {'S', 'T'}));

        algo.findAllFirst_(grammar);
        EXPECT_TRUE(equalContainers(algo.first_, {{'S', {'$'}}, {'P', {'b'}}, {'T', {'$'}}}));
    }

    void testGetFirstFunction() {
        makeGrammarForTestFirst();

        algo.findAllFirst_(grammar);
        auto first = algo.getFirstFunction_(grammar);

        Situation<char> situation = {.left = 'T', .right = "SSS", .point = 1, .first = '$'};
        EXPECT_TRUE(equalContainers(first(situation), {'$'}));

        situation.right = "SSPa";
        EXPECT_TRUE(equalContainers(first(situation), {'b'}));

        situation.right = "SS";
        situation.first = 'a';
        EXPECT_TRUE(equalContainers(first(situation), {'a'}));
    }

    void makeGrammarForMainTests() {
        grammar.addTerminalSymbol({'a', 'b'})
                .addNonTerminalSymbol({'S'})
                .setStartSymbol('S')
                .addRule('S', "")
                .addRule('S', "SaSb");
    }

    void testFit() {
        makeGrammarForMainTests();
        algo.fit(grammar);

        std::set<Situation<char>> startNodeSituations = {{Algo<char>::BEFORE_START_SYMBOL, "S", 0, '$'},
                                                         {'S', "", 0, '$'},
                                                         {'S', "", 0, 'a'},
                                                         {'S', "SaSb", 0, '$'},
                                                         {'S', "SaSb", 0, 'a'}};
        EXPECT_TRUE(equalContainers(algo.nodes_[0].situations_, startNodeSituations));

        std::set<Situation<char>> fourNodeSituations = {{'S', "", 0, 'a'},
                                                        {'S', "", 0, 'b'},
                                                        {'S', "SaSb", 0, 'a'},
                                                        {'S', "SaSb", 0, 'b'},
                                                        {'S', "SaSb", 2, 'a'},
                                                        {'S', "SaSb", 2, 'b'}};
        EXPECT_TRUE(equalContainers(algo.nodes_[4].situations_, fourNodeSituations));

        std::set<Situation<char>> sevenNodeSituations = {{'S', "SaSb", 4, 'a'},
                                                         {'S', "SaSb", 4, 'b'}};
        EXPECT_TRUE(equalContainers(algo.nodes_[7].situations_, sevenNodeSituations));

        EXPECT_TRUE(equalContainers(algo.edges_[3], {{'a', {4}}, {'b', {5}}}));
        EXPECT_TRUE(equalContainers(algo.edges_[4], {{'S', {6}}}));
        EXPECT_TRUE(equalContainers(algo.edges_[7], {}));
    }

    void testBacktrace() {
        makeGrammarForMainTests();
        algo.fit(grammar);

        std::vector<Algo<char>::PathItem> path = {{'S', 3},
                                                  {'b', 5}};
        char symbol = 'b';
        size_t nowNode = 5;
        bool result = algo.backtrace_(path, symbol, nowNode);
        ASSERT_FALSE(result);

        symbol = 'a';
        result = algo.backtrace_(path, symbol, nowNode);
        ASSERT_FALSE(result);

        path = {{'S', 1},
                {'a', 2},
                {'S', 3},
                {'b', 5}};
        result = algo.backtrace_(path, symbol, nowNode);
        ASSERT_TRUE(result);
        ASSERT_EQ(symbol, 'S');
        ASSERT_EQ(nowNode, 0);
    }

    void testPredict() {
        makeGrammarForMainTests();
        algo.fit(grammar);

        ASSERT_TRUE(algo.predict(""));
        ASSERT_TRUE(algo.predict("ab"));
        ASSERT_TRUE(algo.predict("aabbab"));
        ASSERT_TRUE(algo.predict("aababb"));
        ASSERT_TRUE(algo.predict("aaabbabbab"));

        ASSERT_FALSE(algo.predict("a"));
        ASSERT_FALSE(algo.predict("abba"));
        ASSERT_FALSE(algo.predict("aabbabba"));
        ASSERT_FALSE(algo.predict("aba"));
        ASSERT_FALSE(algo.predict("ba"));
    }
};

#endif //LR_1__TEST_H_
