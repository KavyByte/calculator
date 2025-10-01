#include <cctype>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

enum TokenType { ID, PLUS, TIMES, LPAREN, RPAREN, END };

struct Token {
  TokenType type;
  string value;
};

vector<Token> lex(const string &input) {
  vector<Token> tokens;
  for (size_t i = 0; i < input.size();) {
    if (isspace(input[i])) {
      i++;
      continue;
    }
    if (isalpha(input[i])) {
      string id;
      while (i < input.size() && isalnum(input[i])) {
        id.push_back(input[i++]);
      }
      tokens.push_back({ID, id});
    } else if (input[i] == '+') {
      tokens.push_back({PLUS, "+"});
      i++;
    } else if (input[i] == '*') {
      tokens.push_back({TIMES, "*"});
      i++;
    } else if (input[i] == '(') {
      tokens.push_back({LPAREN, "("});
      i++;
    } else if (input[i] == ')') {
      tokens.push_back({RPAREN, ")"});
      i++;
    } else {
      cerr << "Illegal char: " << input[i] << "\n";
      i++;
    }
  }
  tokens.push_back({END, "$"});
  return tokens;
}

struct Rule {
  string lhs;
  int rhsCount;
};

vector<Rule> rules = {{"E", 3}, {"E", 1}, {"T", 3},
                      {"T", 1}, {"F", 3}, {"F", 1}};

string tok2sym(TokenType t) {
  switch (t) {
  case ID:
    return "id";
  case PLUS:
    return "+";
  case TIMES:
    return "*";
  case LPAREN:
    return "(";
  case RPAREN:
    return ")";
  case END:
    return "$";
  }
  return "?";
}

void parse(const vector<Token> &tokens) {
  vector<string> stack;
  size_t ip = 0;

  cout << "Stack\t\tInput\t\tAction\n";

  while (true) {
    for (auto &s : stack)
      cout << s << " ";
    cout << "\t\t";
    for (size_t j = ip; j < tokens.size(); j++)
      cout << tokens[j].value << " ";
    cout << "\t\t";

    bool reduced = false;
    for (auto &r : rules) {
      if (stack.size() >= (size_t)r.rhsCount) {
        vector<string> rhs(stack.end() - r.rhsCount, stack.end());

        if (r.lhs == "E" && r.rhsCount == 3 && rhs[0] == "E" && rhs[1] == "+" &&
            rhs[2] == "T") {
          stack.erase(stack.end() - 3, stack.end());
          stack.push_back("E");
          cout << "Reduce by E->E+T\n";
          reduced = true;
          break;
        } else if (r.lhs == "E" && r.rhsCount == 1 && rhs[0] == "T") {
          stack.erase(stack.end() - 1, stack.end());
          stack.push_back("E");
          cout << "Reduce by E->T\n";
          reduced = true;
          break;
        } else if (r.lhs == "T" && r.rhsCount == 3 && rhs[0] == "T" &&
                   rhs[1] == "*" && rhs[2] == "F") {
          stack.erase(stack.end() - 3, stack.end());
          stack.push_back("T");
          cout << "Reduce by T->T*F\n";
          reduced = true;
          break;
        } else if (r.lhs == "T" && r.rhsCount == 1 && rhs[0] == "F") {
          stack.erase(stack.end() - 1, stack.end());
          stack.push_back("T");
          cout << "Reduce by T->F\n";
          reduced = true;
          break;
        } else if (r.lhs == "F" && r.rhsCount == 3 && rhs[0] == "(" &&
                   rhs[1] == "E" && rhs[2] == ")") {
          stack.erase(stack.end() - 3, stack.end());
          stack.push_back("F");
          cout << "Reduce by F->(E)\n";
          reduced = true;
          break;
        } else if (r.lhs == "F" && r.rhsCount == 1 && rhs[0] == "id") {
          stack.erase(stack.end() - 1, stack.end());
          stack.push_back("F");
          cout << "Reduce by F->id\n";
          reduced = true;
          break;
        }
      }
    }

    if (reduced)
      continue;

    if (ip < tokens.size()) {
      string sym = tok2sym(tokens[ip].type);
      stack.push_back(sym);
      cout << "Shift " << sym << "\n";
      ip++;
    } else {
      cout << "Reject\n";
      break;
    }

    if (stack.size() == 1 && stack[0] == "E" && tokens[ip - 1].type == END) {
      cout << "Accept!\n";
      break;
    }
  }
}

int main() {
  string input = "id + id * id";
  auto tokens = lex(input);
  parse(tokens);
  return 0;
}
