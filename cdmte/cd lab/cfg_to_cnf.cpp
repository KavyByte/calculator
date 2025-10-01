#include <bits/stdc++.h>
using namespace std;

using p1 = vector<string>;
using p = vector<p1>;

map<string, p> grammar;

bool isNonTerminal(const string& s) {
    return isupper(s[0]);
}

void printGrammar(const string& title, const map<string, p>& g) {
    cout << "\n=== " << title << " ===" << endl;
    for (auto& [lhs, rhs] : g) {
        cout << lhs << " -> ";
        for (size_t i = 0; i < rhs.size(); ++i) {
            for (auto& sym : rhs[i]) cout << sym << " ";
            if (i != rhs.size() - 1) cout << "| ";
        }
        cout << endl;
    }
}
void readGrammar(const string& filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        istringstream iss(line);
        string lhs, arrow;
        iss >> lhs >> arrow;
        string prodPart;
        getline(iss, prodPart);
        istringstream pss(prodPart);
        string token;
        while (getline(pss, token, '|')) {
            istringstream symbols(token);
            string sym;
            p1 prod;
            while (symbols >> sym)
                prod.push_back(sym);
            grammar[lhs].push_back(prod);
        }
    }
}

set<string> findNullable(const map<string, p>& grammar) {
    set<string> nullable;
    bool changed = true;

    while (changed) {
        changed = false;
        for (auto& rule : grammar) {
            string lhs = rule.first;
            p rhs = rule.second;

            for (auto& p1 : rhs) {
                if (p1.size() == 1 && p1[0] == "ε") {
                    if (nullable.find(lhs) == nullable.end()) {
                        nullable.insert(lhs);
                        changed = true;
                    }
                } else {
                    bool allNullable = true;
                    for (const string& symbol : p1) {
                        if (nullable.find(symbol) == nullable.end()) {
                            allNullable = false;
                            break;
                        }
                    }
                    if (allNullable && nullable.find(lhs) == nullable.end()) {
                        nullable.insert(lhs);
                        changed = true;
                    }
                }
            }
        }
    }

    return nullable;
}
map<string, p> eliminateEpsilon(const map<string, p>& g) {
    set<string> nullable = findNullable(g);
    map<string, set<p1>> tempG;

    for (auto& [lhs, rhs] : g) {
        for (auto& prod : rhs) {
            int n = prod.size();
            for (int i = 0; i < (1 << n); ++i) {
                p1 newProd;
                bool valid = true;
                for (int j = 0; j < n; ++j) {
                    if (i & (1 << j)) newProd.push_back(prod[j]);
                    else if (!nullable.count(prod[j])) {
                        valid = false;
                        break;
                    }
                }
                if (valid && !newProd.empty())
                    tempG[lhs].insert(newProd);
            }
            if (find(prod.begin(), prod.end(), "ε") == prod.end())
                tempG[lhs].insert(prod);
        }
    }

    map<string, p> newG;
    for (auto& [lhs, prodSet] : tempG) {
        newG[lhs] = p(prodSet.begin(), prodSet.end());
    }
    return newG;
}


map<string, p> eliminateUnitProductions(const map<string, p>& g) {
    map<string, p> newG;
    for (auto& [lhs, rhs] : g) {
        set<string> units = { lhs };
        queue<string> q;
        q.push(lhs);
        while (!q.empty()) {
            string nt = q.front(); q.pop();
            for (auto& prod : g.at(nt)) {
                if (prod.size() == 1 && isNonTerminal(prod[0])) {
                    if (!units.count(prod[0])) {
                        units.insert(prod[0]);
                        q.push(prod[0]);
                    }
                } else {
                    newG[lhs].push_back(prod);
                }
            }
        }
    }
    return newG;
}

map<string, p> replaceTerminals(map<string, p> g, map<string, string>& termMap) {
    int count = 1;
    for (auto& [lhs, rhs] : g) {
        for (auto& prod : rhs) {
            if (prod.size() >= 2) {
                for (auto& sym : prod) {
                    if (!isNonTerminal(sym)) {
                        if (!termMap.count(sym)) {
                            string newVar = "T" + to_string(count++);
                            termMap[sym] = newVar;
                            g[newVar].push_back({ sym });
                        }
                        sym = termMap[sym];
                    }
                }
            }
        }
    }
    return g;
}

map<string, p> binarize(const map<string, p>& g) {
    map<string, p> newG;
    int count = 1;
    for (const auto& [original_lhs, rhs] : g) {
        for (const auto& prod : rhs) {
            if (prod.size() <= 2) {
                newG[original_lhs].push_back(prod);
            } else {
                string prev = prod[0];
                string current_lhs = original_lhs;
                for (size_t i = 1; i < prod.size() - 1; ++i) {
                    string newVar = "X" + to_string(count++);
                    newG[current_lhs].push_back({ prev, newVar });
                    current_lhs = newVar;
                    prev = prod[i];
                }
                newG[current_lhs].push_back({ prev, prod.back() });
            }
        }
    }
    return newG;
}

int main() {
    readGrammar("grammar.txt");

    printGrammar("Original CFG", grammar);

    grammar = eliminateEpsilon(grammar);
    printGrammar("After Eliminating ε-productions", grammar);

    grammar = eliminateUnitProductions(grammar);
    printGrammar("After Eliminating Unit Productions", grammar);

    map<string, string> termMap;
    grammar = replaceTerminals(grammar, termMap);
    printGrammar("After Replacing Terminals", grammar);

    grammar = binarize(grammar);
    printGrammar("Final CNF", grammar);

    return 0;
}