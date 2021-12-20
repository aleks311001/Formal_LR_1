//
// Created by aleks311001 on 18.12.2020.
//

#ifndef LR_1__GRAMMAR_H_
#define LR_1__GRAMMAR_H_

#include <iostream>
#include <set>
#include <vector>
#include <unordered_map>
#include <compare>
#include <queue>
#include <map>
#include <functional>
#include <ostream>
#include <istream>
#include <fstream>

template <typename SymbolType = char, typename Hash = std::hash<SymbolType>>
class Grammar {
private:
    SymbolType start_;
    std::set<SymbolType> terminal_;
    std::set<SymbolType> nonTerminal_;
    std::unordered_multimap<SymbolType, std::basic_string<SymbolType>, Hash> rules_;

public:
    template <typename ST, typename H>
    friend class Algo;
    template <typename ST>
    friend class Node;
    friend class TestGrammar;

    Grammar(): start_(),
               terminal_(),
               nonTerminal_(),
               rules_() {}

    Grammar& addTerminalSymbol(std::set<SymbolType>&& symbols) {
        for (auto ch: symbols) {
            if (nonTerminal_.contains(ch)) {
                throw std::range_error("Non-terminal set already contains \"" + std::to_string(ch) + "\" symbol\n");
            }
        }

        terminal_.merge(std::move(symbols));
        return *this;
    }
    Grammar& addTerminalSymbol(const SymbolType& symbol) {
        if (nonTerminal_.template contains(symbol)) {
            throw std::range_error("Non-terminal set already contain \"" + std::to_string(symbol) + "\" symbol\n");
        }

        terminal_.insert(symbol);
        return *this;
    }

    Grammar& addNonTerminalSymbol(std::set<SymbolType>&& symbols) {
        for (auto ch: symbols) {
            if (terminal_.contains(ch)) {
                throw std::range_error("Terminal set already contains \"" + std::to_string(ch) + "\" symbol\n");
            }
        }

        nonTerminal_.merge(std::move(symbols));
        return *this;
    }
    Grammar& addNonTerminalSymbol(const SymbolType& symbol) {
        if (terminal_.contains(symbol)) {
            throw std::range_error("Terminal set already contains \"" + std::to_string(symbol) + "\" symbol\n");
        }

        nonTerminal_.insert(symbol);
        return *this;
    }

    Grammar& setStartSymbol(const SymbolType& newStart) {
        if (!nonTerminal_.contains(newStart)) {
            throw std::range_error("Non-terminal set doesn't contain start symbol\n");
        }
        start_ = newStart;

        return *this;
    }

    Grammar& addRule(const SymbolType& left, const std::basic_string<SymbolType>& right) {
        if (!nonTerminal_.contains(left)) {
            throw std::range_error("Non-terminal set doesn't contain \"" + std::to_string(left) + "\" symbol\n");
        }
        for (auto ch: right) {
            if (!nonTerminal_.contains(ch) && !terminal_.contains(ch)) {
                throw std::range_error("Non-terminal and terminal sets don't contain \"" + std::to_string(ch) + "\" symbol\n");
            }
        }

        rules_.emplace(left, right);

        return *this;
    }
};

#endif //LR_1__GRAMMAR_H_
