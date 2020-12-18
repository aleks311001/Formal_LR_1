#include <iostream>
#include "Algo.h"

int main() {
    Grammar<char> grammar;

    grammar.addTerminalSymbol({'a', 'b'})
           .addNonTerminalSymbol({'S'})
           .setStartSymbol('S')
           .addRule('S', "")
           .addRule('S', "SaSb");

    Algo<char> algo;
    algo.fit(grammar);
    std::cout << algo.predict("ababaabbaabababb");

    return 0;
}
