#include <bits/stdc++.h>
using namespace std;

struct Rule {
  string left;
  vector<string> right;
};

class CFG {
  vector<Rule> rules;
  set<string> variables, terminals;

public:
  void addRule(const string &left, const vector<string> &right) {
    rules.push_back({left, right});
    variables.insert(left);

    for (const string &symbol : right) {
      if (islower(symbol[0])) {
        terminals.insert(symbol);
      } else {
        variables.insert(symbol);
      }
    }
  }

  void replaceTerminalsInRules() {
    map<string, string> terminalMap;
    int terminalCount = 1;
    vector<Rule> updatedRules;

    for (auto &rule : rules) {
      vector<string> newRightSide;

      for (auto &symbol : rule.right) {
        if (islower(symbol[0]) && rule.right.size() > 1) {

          if (terminalMap.find(symbol) == terminalMap.end()) {
            string newVar = "T" + to_string(terminalCount++);
            terminalMap[symbol] = newVar;
            updatedRules.push_back({newVar, {symbol}});
          }
          newRightSide.push_back(terminalMap[symbol]);
        } else {
          newRightSide.push_back(symbol);
        }
      }
      updatedRules.push_back({rule.left, newRightSide});
    }

    rules = updatedRules;
  }

  void binarizeRules() {
    vector<Rule> updatedRules;
    int variableCount = 1;

    for (auto &rule : rules) {
      if (rule.right.size() <= 2) {
        updatedRules.push_back(rule);
      } else {
        string prev = rule.right[0];
        for (size_t i = 1; i < rule.right.size() - 1; i++) {
          string newVar = "X" + to_string(variableCount++);
          updatedRules.push_back({newVar, {prev, rule.right[i]}});
          prev = newVar;
        }
        updatedRules.push_back({rule.left, {prev, rule.right.back()}});
      }
    }

    rules = updatedRules;
  }

  void convertToCNF() {
    replaceTerminalsInRules();
    binarizeRules();
  }

  void printGrammar() const {
    for (const auto &rule : rules) {
      cout << rule.left << " -> ";
      for (size_t i = 0; i < rule.right.size(); i++) {
        cout << rule.right[i];
        if (i + 1 < rule.right.size())
          cout << " ";
      }
      cout << endl;
    }
  }
};

int main() {
  CFG grammar;

  grammar.addRule("S", {"A", "B", "C"});
  grammar.addRule("A", {"a"});
  grammar.addRule("B", {"b"});
  grammar.addRule("C", {"c", "D"});
  grammar.addRule("D", {"d"});

  cout << "Input CFG:\n";
  grammar.printGrammar();

  grammar.convertToCNF();

  cout << "\nCNF Grammar:\n";
  grammar.printGrammar();

  return 0;
}
