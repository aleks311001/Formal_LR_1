//
// Created by aleks311001 on 01.12.2020.
//

#ifndef LR_1__ALGO_H_
#define LR_1__ALGO_H_

#include <cmath>
#include "Grammar.h"

template<typename SymbolType = char, typename Hash = std::hash<SymbolType>>
class Algo;

template<typename SymbolType = char>
struct Situation {
  SymbolType left = 0;
  std::basic_string<SymbolType> right;
  size_t point = 0;
  SymbolType first = 0;

  bool operator==(const Situation &other) const {
    return left == other.left &&
        right == other.right &&
        point == other.point &&
        first == other.first;
  }
  auto operator<=>(const Situation &other) const = default;

  bool isEnd() const {
    return point == right.size();
  }
  Situation getNext() const {
    if (isEnd()) {
      throw std::range_error("Point already in the end of situation\n");
    }
    return Situation{left, right, point + 1, first};
  }
  SymbolType getSymbol() const {
    if (isEnd()) {
      return first;
    }
    return right[point];
  }

  std::string dump() const {
    std::string result;
    if (left != Algo<SymbolType>::BEFORE_START_SYMBOL) {
      result += left;
    } else {
      result += "S'";
    }
    result += " -> ";
    result += right.substr(0, point) + "." + right.substr(point) + ", ";
    result = result + first + "\n";
    return result;
  }
};

template<typename SymbolType = char>
struct Node {
  const Grammar<SymbolType> &grammar_;
  std::set<Situation<SymbolType>> situations_;
  bool safeMode = true;

  Node(const Grammar<SymbolType> &grammar) : grammar_(grammar), situations_(), safeMode(true) {}
  Node(const Node &other) : grammar_(other.grammar_), situations_(other.situations_), safeMode(true) {}

  bool addNextSituations(const Situation<SymbolType> &nowSituation,
                         std::queue<typename std::set<Situation<SymbolType>>::const_iterator> &queueSituations,
                         auto &first) {
    bool isLR = true;

    auto rangeRules = grammar_.rules_.equal_range(nowSituation.getSymbol());
    for (auto it = rangeRules.first; it != rangeRules.second; ++it) {
      Situation<SymbolType> newSituation = {.left = nowSituation.getSymbol(),
          .right = it->second,
          .point = 0};

      for (auto &firstSymbol: first(nowSituation)) {
        newSituation.first = firstSymbol;
        auto addResult = addSituation(newSituation);

        if (addResult.resultInsert.second) {
          queueSituations.push(addResult.resultInsert.first);
        }

        if (!addResult.isLR) {
          isLR = false;
        }
      }
    }

    return isLR;
  }

  bool addNextSituations(const Situation<SymbolType> &nowSituation,
                         auto &first) {
    bool isLR = true;

    std::queue<typename std::set<Situation<SymbolType>>::const_iterator> q;
    q.push(situations_.find(nowSituation));

    while (!q.empty()) {
      auto now = q.front();
      q.pop();

      if (!addNextSituations(*now, q, first)) {
        isLR = false;
      }
    }

    return isLR;
  }

  auto operator<=>(const Node &other) const {
    if (situations_.size() != other.situations_.size()) {
      return situations_.size() <=> other.situations_.size();
    }

    for (auto it1 = situations_.begin(), it2 = other.situations_.begin(); it1 != situations_.end(); ++it1, ++it2) {
      if (auto cmp = *it1 <=> *it2; cmp != 0) {
        return cmp;
      }
    }

    return 0 <=> 0;
  }

  struct AddResult {
    std::pair<typename std::set<Situation<SymbolType>>::const_iterator, bool> resultInsert;
    bool isLR = true;
  };

  AddResult addSituation(const Situation<SymbolType> &situation) {
    Situation<SymbolType> endSituation = {.left = Algo<SymbolType>::BEFORE_START_SYMBOL,
        .right = std::string(&grammar_.start_, 1),
        .point = 1,
        .first = Algo<SymbolType>::END_SYMBOL};

    bool isLR = true;
    for (auto &situations: situations_) {
      if (safeMode &&
          situation.first == situations.first &&
          situation.isEnd() &&
          situations.isEnd() &&
          situations != situation &&
          situations != endSituation) {
        isLR = false;
        //throw std::runtime_error("This grammar is not LR(1) language.\n");
      }
    }

    return {situations_.insert(situation), isLR};
  }

  struct ResultGoTo {
    Node node;
    bool isLR = true;
  };

  ResultGoTo goTo(const SymbolType &symbol, auto &first) const {
    Node<SymbolType> newNode(grammar_);
    newNode.safeMode = safeMode;
    std::queue<typename std::set<Situation<SymbolType>>::const_iterator> queueSituations;
    bool isLR = true;

    for (auto &situation: situations_) {
      if (!situation.isEnd() && situation.getSymbol() == symbol) {
        auto iterator = newNode.addSituation(situation.getNext()).resultInsert.first;
        queueSituations.push(iterator);
      }
    }

    while (!queueSituations.empty()) {
      auto &nowSituation = *queueSituations.front();
      queueSituations.pop();

      if (nowSituation.isEnd() || !grammar_.nonTerminal_.contains(nowSituation.getSymbol())) {
        continue;
      }

      if (!newNode.addNextSituations(nowSituation, queueSituations, first)) {
        isLR = false;
      }
    }

    return {newNode, isLR};
  }

  struct EndSituation {
    Situation<SymbolType> situation;
    bool isValid = false;
  };

  EndSituation getEndSituation(const SymbolType &next) const {
    size_t count = 0;
    EndSituation result;

    Situation<SymbolType> mainSituation = {.left = Algo<SymbolType>::BEFORE_START_SYMBOL,
        .right = std::string(&grammar_.start_, 1),
        .point = 1,
        .first = Algo<SymbolType>::END_SYMBOL};

    for (auto &situation: situations_) {
      if (situation.isEnd() && situation.first == next) {
        ++count;
        if ((count > 1 && situation != mainSituation && result.situation != mainSituation) || count > 2) {
          throw std::runtime_error("This grammar is not LR(1) language!\n");
        }
        result.situation = situation;
        result.isValid = true;
      }
    }

    return result;
  }
};

template<typename SymbolType, typename Hash>
class Algo {
 public:
  inline static const SymbolType END_SYMBOL = '$';
  inline static const SymbolType BEFORE_START_SYMBOL = '\0';

 private:
  std::vector<Node<SymbolType>> nodes_;
  std::unordered_map<size_t, std::unordered_map<SymbolType, size_t>> edges_;

  std::set<SymbolType> epsGenerating_;
  std::unordered_map<SymbolType, std::set<SymbolType>, Hash> first_;

  friend class TestAlgo;

  void findEpsGeneratingOneStep_(const Grammar<SymbolType> &grammar) {
    for (auto &rule: grammar.rules_) {
      bool wasNonEpsGen = false;

      for (const SymbolType &ch: rule.second) {
        if (!epsGenerating_.contains(ch)) {
          wasNonEpsGen = true;
          break;
        }
      }

      if (!wasNonEpsGen) {
        epsGenerating_.insert(rule.first);
      }
    }
  }
  void findEpsGenerating_(const Grammar<SymbolType> &grammar) {
    size_t lastSize = 0;

    do {
      lastSize = epsGenerating_.size();
      findEpsGeneratingOneStep_(grammar);
    } while (lastSize != epsGenerating_.size());
  }

  bool findFirstOneStep_(const Grammar<SymbolType> &grammar) {
    bool wasChanged = false;

    for (auto &rule: grammar.rules_) {
      SymbolType leftSymbol = rule.first;
      for (const SymbolType &ch: rule.second) {
        if (grammar.terminal_.contains(ch)) {
          wasChanged = first_[leftSymbol].insert(ch).second;
          break;
        }

        size_t beforeSize = first_[leftSymbol].size();
        first_[leftSymbol].merge(first_[ch]);
        if (beforeSize != first_[leftSymbol].size()) {
          wasChanged = true;
        }

        if (!epsGenerating_.contains(ch)) {
          break;
        }
      }
    }

    return wasChanged;
  }
  void findAllFirst_(const Grammar<SymbolType> &grammar) {
    findEpsGenerating_(grammar);

    while (findFirstOneStep_(grammar));

    for (const SymbolType &ch: grammar.nonTerminal_) {
      if (!first_.contains(ch) || first_[ch].empty()) {
        first_[ch].insert(END_SYMBOL);
      }
    }
  }

  bool goto_(size_t node, SymbolType symbol,
             std::map<Node<SymbolType>, size_t> &nodesToIndex,
             auto &first) {
    auto addedNode = nodes_[node].goTo(symbol, first);
    if (addedNode.node.situations_.empty()) {
      return addedNode.isLR;
    }

    auto pair = nodesToIndex.emplace(std::move(addedNode.node), nodes_.size());

    if (pair.second) {
      nodes_.push_back(pair.first->first);
    }

    edges_[node][symbol] = pair.first->second;
    return addedNode.isLR;
  }

  auto getFirstFunction_(const Grammar<SymbolType> &grammar) {
    return [&first_ = this->first_, &epsGenerating_ = this->epsGenerating_, &grammar]
        (const Situation<SymbolType> &situation) {
      std::set<SymbolType> result;

      size_t i = situation.point + 1;
      for (; i < situation.right.size(); ++i) {
        if (grammar.terminal_.contains(situation.right[i])) {
          result.insert(situation.right[i]);
          break;
        }

        result.merge(first_[situation.right[i]]);

        if (!epsGenerating_.contains(situation.right[i])) {
          break;
        }
      }
      if (i == situation.right.size() && (situation.first != '$' || result.empty())) {
        result.insert(situation.first);
      }

      return result;
    };
  }

  struct PathItem {
    SymbolType symbol;
    size_t finishNode;
  };

  bool backtrace_(std::vector<PathItem> &path, SymbolType &symbol, size_t &nowNode) {
    auto endSituation = nodes_[nowNode].getEndSituation(symbol);
    if (!endSituation.isValid) {
      return false;
    }

    auto &backtraceWord = endSituation.situation.right;
    for (auto it = backtraceWord.rbegin(); it != backtraceWord.rend(); ++it) {
      if (path.empty() || *it != path.back().symbol) {
        return false;
      }

      path.pop_back();
    }

    symbol = endSituation.situation.left;
    if (path.empty()) {
      nowNode = 0;
    } else {
      nowNode = path.back().finishNode;
    }

    return true;
  }

 public:
  Algo &fit(const Grammar<SymbolType> &grammar, bool safeMode = true) {
    findAllFirst_(grammar);

    std::map<Node<SymbolType>, size_t> nodesToIndex;

    Situation<SymbolType> startSituation = {.left = BEFORE_START_SYMBOL,
        .right = std::string(&grammar.start_, 1),
        .point = 0,
        .first = END_SYMBOL};

    auto firstFunction = getFirstFunction_(grammar);
    nodes_.emplace_back(grammar);
    nodes_.front().safeMode = safeMode;
    nodes_.front().addSituation(startSituation);
    nodes_.front().addNextSituations(startSituation, firstFunction);
    nodesToIndex.emplace(nodes_.front(), 0);

    bool isLR = true;

    for (size_t nowNode = 0; nowNode < nodes_.size(); ++nowNode) {
      for (auto &symbol: grammar.terminal_) {
        if (!goto_(nowNode, symbol, nodesToIndex, firstFunction)) {
          isLR = false;
        }
      }
      for (auto &symbol: grammar.nonTerminal_) {
        if (!goto_(nowNode, symbol, nodesToIndex, firstFunction)) {
          isLR = false;
        }
      }
    }

    if (!isLR) {
      std::cerr << dump();
      throw std::runtime_error("This grammar is not LR(1) language.\n");
    }

    return *this;
  }

  bool predict(std::basic_string<SymbolType> word) {
    std::vector<PathItem> path;
    size_t nowNode = 0;
    word.push_back(END_SYMBOL);
    SymbolType symbol = word[0];

    for (size_t index = 0; true;) {
      if (edges_[nowNode].contains(symbol)) {
        nowNode = edges_[nowNode][symbol];
        path.emplace_back(symbol, nowNode);

        if (word[index] == symbol) {
          ++index;
        }

        symbol = word[index];
      } else {
        if (symbol == END_SYMBOL) {
          auto endSituation = nodes_[nowNode].getEndSituation(END_SYMBOL);
          if (endSituation.isValid &&
              endSituation.situation.left == BEFORE_START_SYMBOL &&
              endSituation.situation.point == 1) {
            return true;
          }
        }

        if (!backtrace_(path, symbol, nowNode)) {
          return false;
        }
      }
    }
  }

  std::string dump() const {
    std::string result = "nodes:\n";

    for (size_t i = 0; i < nodes_.size(); ++i) {
      result += std::to_string(i) + "\n";

      for (auto &situation: nodes_[i].situations_) {
        result += situation.dump();
      }

      result += '\n';
    }

    result += '\n';
    result += "edges:\n";
    for (auto &edge: edges_) {
      for (auto transition: edge.second) {
        result += std::to_string(edge.first) + " " + transition.first + " " + std::to_string(transition.second) + "\n";
      }
    }

    return result;
  }

  void writePrefix_(std::ofstream &file) {
    file << "\\documentclass[12pt,a4paper]{article}\n"
            "\\usepackage{cmap}\n"
            "\\usepackage[T2A]{fontenc}\n"
            "\\usepackage[utf8]{inputenc}\n"
            "\\usepackage[russian]{babel}\n"
            "\\usepackage{amsmath}\n"
            "\\usepackage{amsfonts}\n"
            "\\usepackage{amssymb}\n"
            "\\usepackage[left=1.5cm,right=1.5cm,top=1.75cm,bottom=1.75cm]{geometry}\n"
            "\n"
            "\\newcommand{\\timelimit}{}\n"
            "\n"
            "\\usepackage{tikz}\n"
            "\\usetikzlibrary{automata, positioning, arrows}\n"
            "\n"
            "\n"
            "\\begin{document}\n"
            "\\pagestyle{empty}\n"
            "\\noindent\n";
  }

  void writeConfigurations_(std::ofstream &file,
                            double r) {
    for (size_t conf = 0; conf < nodes_.size(); ++conf) {
      file << "    \\node [state";
      if (conf == 0) {
        file << ", initial";
      }
//      if (acceptingConfigurations_.contains(conf)) {
//        file << ", accepting";
//      }

      double angle = 2 * M_PI / nodes_.size() * conf + M_PI;
      file << "] (" << conf << ") at ("
           << r * std::cos(angle) << ", "
           << r * std::sin(angle) << ") {"
           << conf << "};\n";
    }
  }
  void writeEdge_(std::ofstream &file, SymbolType word,
                  long long int startConf, long long int finishConf, /*int angle,*/
                  const std::unordered_map<long long, std::set<long long>> &daddies) {
    file << "\n            edge ";

    if (finishConf == startConf) {
      file << "[loop ";

      long long int sz = nodes_.size();
      long long int num = finishConf;
      if (num > sz / 8 && num <= sz * 3 / 8) {
        file << "below] ";
      } else if ((num > sz * 3 / 8 && num <= sz * 5 / 8) || num == 0) {
        file << "right] ";
      } else if (num > sz * 5 / 8 && num <= sz * 7 / 8) {
        file << "above] ";
      } else {
        file << "left] ";
      }
    } else {
      if (daddies.at(startConf).contains(finishConf)) {
        file << "[bend left = 20] ";
      }
    }

    file << "node {$" << word;
//    if (word == EPS) {
//      file << "\\varepsilon";
//    }
    file << "$} (" << finishConf << ")";
  }

  std::unordered_map<long long, std::set<long long>> findDaddies() {
    std::unordered_map<long long, std::set<long long>> result;

    for (size_t conf = 0; conf < nodes_.size(); ++conf) {
      result.emplace(conf, std::set<long long>());
    }

    for (auto &node: edges_) {
      for (auto &pair: node.second) {
//        for (auto &finish: pair.second) {
        result[pair.second].insert(node.first);
//        }
      }
    }

    return result;
  }

  void writeTransitions_(std::ofstream &file) {
    auto daddies = findDaddies();

    file << "\n    \\path";

    for (auto &edge: edges_) {
      file << "\n        ("
           << edge.first << ") ";

      for (auto &pair: edge.second) {
//        for (auto &conf: pair.second) {
          //++numEdges[transition.first][conf];
          writeEdge_(file, pair.first, edge.first, pair.second,/* numEdges[transition.first][conf] * 10,*/
                     daddies);
//        }
      }
    }
    file << ";\n";
  }

  void writeMainPart_(std::ofstream &file, double r) {
//    writeAlphabet_(file);
//    file << "    $q_0 = " << numsConfigurations.at(q0_) << "$\\\\\n";
//    writeAcceptingConfigurations_(file, numsConfigurations);

//    makeOneEdgeForAllPairs_(',', "\\varepsilon");

    file << "\\begin{tikzpicture}"
            "[->, >=stealth', on grid, auto, node distance = 2.5cm, every state/.style={thick, fill=gray!20}]\n";
    writeConfigurations_(file, r);
    writeTransitions_(file);
  }

  void writeSuffix_(std::ofstream &file) {
    file << "\\end{tikzpicture}\n\n";
    file << "\\end{document}\n";
  }

  void createTexFileThisNKAWithoutCopy(const std::string &filename, double r = 1) {
    std::ofstream newFile(filename);

    writePrefix_(newFile);
    writeMainPart_(newFile, r);
    writeSuffix_(newFile);

    newFile.close();
  }
};

#endif //LR_1__ALGO_H_
