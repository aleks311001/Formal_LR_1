//
// Created by aleks311001 on 01.12.2020.
//

#ifndef LR_1__ALGO_H_
#define LR_1__ALGO_H_

#include <iostream>
#include <set>
#include <vector>
#include <unordered_map>

template <typename SymbolType = char, typename Hash = std::hash<SymbolType>>
class Grammar {
private:
    SymbolType start_;
    std::set<SymbolType> terminal_;
    std::set<SymbolType> nonTerminal_;
    std::unordered_multimap<SymbolType, std::basic_string<SymbolType>, Hash> rules_;

public:
    Grammar(const SymbolType& start, const std::set<SymbolType>& terminal, const std::set<SymbolType>& nonTerminal)
            : start_(start),
              terminal_(terminal),
              nonTerminal_(nonTerminal),
              rules_() {
        if (!nonTerminal_.contains(start_)) {
            throw std::range_error("Non-terminal set doesn't contain start symbol\n");
        }
    }

    bool addTerminalSymbol(const SymbolType& symbol) {
        return terminal_.insert(symbol).second;
    }
    void addNonTerminalSymbol() {
        
    }
    void setStartSymbol() {

    }

    void addRule() {

    }
};

class Algo {
private:

public:
    Algo& fit(const Grammar& grammar) {
        return *this;
    }

    bool predict(const std::string& word) {

    }
};

#endif //LR_1__ALGO_H_
