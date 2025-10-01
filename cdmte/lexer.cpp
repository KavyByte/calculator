#include<bits/stdc++.h>
using namespace std;

unordered_set<string> keywords = {"int", "float", "if", "else", "for", "while"};

struct Token {
    string type;
    string value;
};

vector<Token> analyze(string code) {
    vector<Token> tokens;
    int n = code.size();
    string word;

    for (int i = 0; i < n; i++) {
        char c = code[i];

        if (isspace(c)) continue;

        if (isalpha(c) || c == '_') {
            word.clear();
            while (i < n && (isalnum(code[i]) || code[i] == '_')) {
                word.push_back(code[i]);
                i++;
            }
            i--;
            if (keywords.count(word))
                tokens.push_back({"Keyword", word});
            else
                tokens.push_back({"Identifier", word});
        }

        else if (isdigit(c)) {
            word.clear();
            bool isFloat = false;
            while (i < n && (isdigit(code[i]) || code[i] == '.')) {
                if (code[i] == '.') isFloat = true;
                word.push_back(code[i]);
                i++;
            }
            i--;
            if (isFloat)
                tokens.push_back({"Float", word});
            else
                tokens.push_back({"Integer", word});
        }

        else if (strchr("+-*/=<>&|!", c)) {
            tokens.push_back({"Operator", string(1, c)});
        }
        
        else if (strchr(";,(){}", c)) {
            tokens.push_back({"Punctuation", string(1, c)});
        }

        else {
            tokens.push_back({"Unknown", string(1, c)});
        }
    }
    return tokens;
}

int main() {
    // string code = "int a = 10; float b = 3.14; if (a < b) { a = a + 1; }";
    string code = "a*x+by";

    cout << " Lexical Analysis \n";
    vector<Token> tokens = analyze(code);

    for (auto &t : tokens) {
        cout << t.type << " - " << t.value << endl;
    }

    return 0;
}
