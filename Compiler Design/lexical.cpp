#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>

struct Token {
    std::string type;
    std::string value;
    int line;
};

std::unordered_map<std::string, std::string> keywords = {
    {"if", "IF"},
    {"else", "ELSE"},
    {"while", "WHILE"},
    {"return", "RETURN"},
    {"def", "DEF"}
};

struct Rule {
    std::string type;
    std::regex pattern;
};

std::vector<Rule> rules = {
    {"FLOAT",  std::regex(R"(\d+\.\d+)")},
    {"INT",    std::regex(R"(\d+)")},
    {"STRING", std::regex(R"("(\\.|[^"\\])*"|'(\\.|[^'\\])*')")},
    {"ID",     std::regex(R"([A-Za-z_][A-Za-z0-9_]*)")},
    {"EQ",     std::regex(R"(==)")},
    {"NEQ",    std::regex(R"(!=)")},
    {"LE",     std::regex(R"(<=)")},
    {"GE",     std::regex(R"(>=)")},
    {"ASSIGN", std::regex(R"(=)")},
    {"PLUS",   std::regex(R"(\+)")},
    {"MINUS",  std::regex(R"(\-)")},
    {"TIMES",  std::regex(R"(\*)")},
    {"DIVIDE", std::regex(R"(/)")},
    {"LT",     std::regex(R"(<)")},
    {"GT",     std::regex(R"(>)")},
    {"LPAREN", std::regex(R"(\()")},
    {"RPAREN", std::regex(R"(\))")},
    {"LBRACE", std::regex(R"(\{)")},
    {"RBRACE", std::regex(R"(\})")},
    {"COMMA",  std::regex(R"(,)")},
    {"SEMI",   std::regex(R"(;)")},
    {"COLON",  std::regex(R"(:)")}
};

std::vector<Token> tokenize(const std::string &input) {
    std::vector<Token> tokens;
    size_t pos = 0;
    int line = 1;

    while (pos < input.size()) {
        // Skip whitespace
        if (isspace(input[pos])) {
            if (input[pos] == '\n') line++;
            pos++;
            continue;
        }

        if (input.compare(pos, 2, "//") == 0) {
            while (pos < input.size() && input[pos] != '\n') pos++;
            continue;
        }
        if (input.compare(pos, 2, "/*") == 0) {
            pos += 2;
            while (pos < input.size() && !(input[pos-1] == '*' && input[pos] == '/')) {
                if (input[pos] == '\n') line++;
                pos++;
            }
            pos++;
            continue;
        }

        bool matched = false;
        for (auto &rule : rules) {
            std::smatch match;
            std::string remaining = input.substr(pos);
            if (std::regex_search(remaining, match, rule.pattern, std::regex_constants::match_continuous)) {
                std::string val = match.str();
                std::string type = rule.type;

                // check if ID is keyword
                if (type == "ID" && keywords.count(val)) {
                    type = keywords[val];
                }

                tokens.push_back({type, val, line});
                pos += val.size();
                matched = true;
                break;
            }
        }

        if (!matched) {
            std::cerr << "Illegal character '" << input[pos] 
                      << "' at line " << line << "\n";
            pos++;
        }
    }

    return tokens;
}

int main() {
    std::string code = R"(
        // demo program
        def add(a, b) {
            return a + b;
        }
        x = 123;
        y = 3.14;
        s = "hello\nworld";
        if x >= 10 {
            y = y * 2;
        }
    )";

    auto tokens = tokenize(code);

    for (auto &tok : tokens) {
        std::cout << tok.type << "   " 
                  << tok.value << "   " 
                  << tok.line << "\n";
    }

    return 0;
}
