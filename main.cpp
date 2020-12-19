#include <iostream>
#include "Algo.h"

int main() {
    Grammar<char> grammar;

    /*grammar.addTerminalSymbol({'a', 'b'})
           .addNonTerminalSymbol({'S'})
           .setStartSymbol('S')
           .addRule('S', "")
           .addRule('S', "SaSb");*/

    grammar.addTerminalSymbol({'a', 'b'})
           .addNonTerminalSymbol({'S'})
           .setStartSymbol('S')
           .addRule('S', "aSa")
           .addRule('S', "bSb")
           .addRule('S', "a")
           .addRule('S', "b")
           .addRule('S', "");

    Algo<char> algo;
    algo.fit(grammar);
    std::cout << algo.predict("aa");

    return 0;
}
