#include <iostream>
#include "Algo.h"

Grammar<char> read_grammar(std::ifstream& data) {
  Grammar<char> grammar;

  std::string line;
  data >> line;

  std::set<char> term;
  while(data >> line) {
    if (line == "nonterm") {
      break;
    }
    grammar.addTerminalSymbol(line[0]);
  }

  while(data >> line) {
    if (line == "start") {
      break;
    }
    grammar.addNonTerminalSymbol(line[0]);
  }

  while(data >> line) {
    if (line == "rules") {
      break;
    }
    grammar.setStartSymbol(line[0]);
  }

  while(data >> line) {
    if (line == "check") {
      break;
    }
    grammar.addRule(line[0], line.substr(3));
  }

  return grammar;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "Please enter input and output files" << std::endl;
    return 1;
  }

  std::ifstream data(argv[1]);
  auto grammar = read_grammar(data);

  std::string check_word;
  data >> check_word;

//  Grammar<char> grammar;
//
//  grammar.addTerminalSymbol({'a', 'b'})
//      .addNonTerminalSymbol({'S'})
//      .setStartSymbol('S')
//      .addRule('S', "")
//      .addRule('S', "SaSb");

//    grammar.addTerminalSymbol({'a', 'b'})
//           .addNonTerminalSymbol({'S'})
//           .setStartSymbol('S')
//           .addRule('S', "aSa")
//           .addRule('S', "bSb")
//           .addRule('S', "a")
//           .addRule('S', "b")
//           .addRule('S', "");

  Algo<char> algo;
  algo.fit(grammar);


//  std::cout << algo.dump();
  algo.createTexFileThisNKAWithoutCopy(argv[2], 4.0);

  std::cout << algo.predict(check_word);
//    std::cout << algo.predict("ab");

  return 0;
}
