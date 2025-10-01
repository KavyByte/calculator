#include <bits/stdc++.h>
using namespace std;

bool isTerminal(const string &s) {
    unordered_set<string> terminal= {"+","-","*","/","(",")","id","$"};
    return terminal.count(s) > 0;
}

bool isOperatorSym(const string &s) {
    return s=="+"||s=="-"||s=="*"||s=="/";
}

vector<string> tokenize(const string &s) {
    vector<string> tokens;
    int n = s.size();
    for (int i = 0; i < n;) {
        if (isspace(s[i])) { i++; continue; }
        if (isalpha(s[i]) || isdigit(s[i])) {
            while (i < n && (isalnum(s[i]))) i++;
            tokens.push_back("id"); 
        } else if (strchr("+-*/=<>&|!", s[i])) {
            tokens.push_back(string(1, s[i]));
            i++;
        } else {
            tokens.push_back(string(1, s[i]));
            i++;
        }
    }
    tokens.push_back("$");
    return tokens;
}

char prec(char top, char input) {
    string t = "+-*/()i$";  
    int r = t.find(top), c = t.find(input);
    static char table[8][8] = {
        // +    -    *    /    (    )    i    $
        { '>', '>', '<', '<', '<', '>', '<', '>'}, // +
        { '>', '>', '<', '<', '<', '>', '<', '>'}, // -
        { '>', '>', '>', '>', '<', '>', '<', '>'}, // *
        { '>', '>', '>', '>', '<', '>', '<', '>'}, // /
        { '<', '<', '<', '<', '<', '=', '<', ' '}, // (
        { '>', '>', '>', '>', ' ', '>', ' ', '>'}, // )
        { '>', '>', '>', '>', ' ', '>', ' ', '>'}, // i
        { '<', '<', '<', '<', '<', ' ', '<', '='}  // $
    };
    if (r == -1 || c == -1) return ' ';
    return table[r][c];
}

int topTerminalIdx(vector<string> &stk) {
    for (int i = stk.size() - 1; i >= 0; i--)
        if (isTerminal(stk[i])) return i;
    return -1;
}

char toChar(const string &s) {
    if (s=="id") return 'i';
    if (s=="+") return '+'; if (s=="-") return '-';
    if (s=="*") return '*'; if (s=="/") return '/';
    if (s=="(") return '('; if (s==")") return ')';
    if (s=="$") return '$';
    return ' ';
}

string stackToString(vector<string> &stk) {
    string s;
    for (auto &x : stk) s += x + " ";
    return s;
}

string remainingInput(vector<string> &tokens, int pos) {
    string s;
    for (int i = pos; i < tokens.size(); i++) s += tokens[i] + " ";
    return s;
}

void shift(vector<string> &stk, const string &token, int &pos, vector<string> &tokens, int step) {
    stk.push_back(token);
    pos++;
    cout << step << " | " << stackToString(stk) << " | " << remainingInput(tokens, pos) << " | Shift\n";
}

bool reduce(vector<string> &stk, vector<string> &tokens, int pos, int step) {
    int n = stk.size();

    if (stk[n-1] == "id") {
        stk[n-1] = "E";
        cout << step << " | " << stackToString(stk) 
             << " | " << remainingInput(tokens, pos) 
             << " | Reduce id -> E\n";
        return true;
    }

    if (n >= 3 && stk[n-3] == "E" && isOperatorSym(stk[n-2]) && stk[n-1] == "E") {
        stk[n-3] = "E";  
        stk.pop_back();   
        stk.pop_back();   
        cout << step << " | " << stackToString(stk) 
             << " | " << remainingInput(tokens, pos) 
             << " | Reduce E op E -> E\n";
        return true;
    }

    if (n >= 3 && stk[n-3] == "(" && stk[n-2] == "E" && stk[n-1] == ")") {
        stk[n-3] = "E"; 
        stk.pop_back(); 
        stk.pop_back();   
        cout << step << " | " << stackToString(stk) 
             << " | " << remainingInput(tokens, pos) 
             << " | Reduce (E) -> E\n";
        return true;
    }

    return false;
}


int main() {
    cout << "Operator Precedence Parser (simple)\n";
    cout << "Enter an expression (identifiers are letters/digits). Example: a+b*(c-d)\n> ";

    string line;
    getline(cin, line);

    vector<string> tokens = tokenize(line);
    vector<string> stack = {"$"};
    int pos = 0;
    int step = 1;
    bool error = false;

    cout << "\nStep | Stack | Input | Action\n";
    cout << "-------------------------------------------\n";

    while (true) {
        string topTerminal = stack[topTerminalIdx(stack)]; 
        string currentToken = tokens[pos];

        char relation = prec(toChar(topTerminal), toChar(currentToken));

        if (topTerminal == "$" && currentToken == "$") {
            cout << step << " | " << stackToString(stack)
                 << " | " << remainingInput(tokens, pos)
                 << " | Accept\n";
            break; 
        }

        if (relation == '<' || relation == '=') {
            stack.push_back(currentToken);
            pos++;
            cout << step << " | " << stackToString(stack)
                 << " | " << remainingInput(tokens, pos)
                 << " | Shift\n";
        }
        else if (relation == '>') {
            if (!reduce(stack, tokens, pos, step)) {
                cout << step << " | " << stackToString(stack)
                     << " | " << remainingInput(tokens, pos)
                     << " | Error\n";
                error = true;
                break;
            }
        }
        else {
            cout << step << " | " << stackToString(stack)
                 << " | " << remainingInput(tokens, pos)
                 << " | Error\n";
            error = true;
            break;
        }

        step++;
    }

    if (error)
        cout << "Invalid Expression\n";
    else
        cout << "Valid Expression\n";

    return 0;
}
