#ifndef BRACKETCHECKER_CODE_PAIRS_CHECK_H
#define BRACKETCHECKER_CODE_PAIRS_CHECK_H

#include <vector>
#include <stack>
#include <unordered_map>
#include <string>

struct PositionError {
    int line;
    int col;
    std::string description;
};


static bool startsWith(const std::string &code, int index, const std::string &word) {
    return code.compare(index, word.size(), word) == 0;
}

std::vector<PositionError> checkCode(std::string &code) {
    std::vector<PositionError> errors;

    static const std::unordered_map<std::string, std::string> substrCloseToOpen = {
            {"end", "begin"},
            {"*)",  "(*"}
    };
    static const std::unordered_map<std::string, bool> substrOpen = {
            {"begin", true},
            {"(*",    true}
    };
    static const std::unordered_map<char, char> bracketCloseToOpen = {
            {')', '('},
            {']', '['},
            {'}', '{'}
    };
    std::stack<std::tuple<std::string, int, int>> symbols;
    int line = 1, col = 1;
    for (int i = 0; i < (int) code.size(); i++, col++) {
        if (code[i] == '\n') {
            line++;
            col = 0;
            continue;
        }
        if (code[i] == '\'') {
            i++;
            col++;
            while (i < (int) code.size()) {
                if (code[i] == '\n') {
                    line++;
                    col = 0;
                }
                if (code[i] == '\'' && !(i + 1 < (int) code.size() && code[i + 1] == '\'')) break;
                if (code[i] == '\'' && code[i + 1] == '\'') {
                    i++;
                    col++;
                }
                i++;
                col++;
            }
            continue;
        }

        bool processed = false;
        for (const auto &p: substrOpen) {
            const std::string &sub = p.first;
            if (startsWith(code, i, sub)) {
                symbols.emplace(sub, line, col);
                i += sub.size() - 1;
                col += sub.size() - 1;
                processed = true;
                break;
            }
        }
        if (processed) continue;
        for (const auto &p: substrCloseToOpen) {
            const std::string &close = p.first;
            const std::string &open = p.second;
            if (startsWith(code, i, close)) {
                if (symbols.empty()) {
                    errors.push_back({
                        line,
                        col,
                        "Closing token '" + close + "' without matching opening token '" +
                        open + "'"});
                } else {
                    auto [topToken, topLine, topCol] = symbols.top();
                    symbols.pop();
                    if (topToken != open) {
                        errors.push_back({
                        topLine,
                        topCol,
                        "Token '" + topToken + "' does not match closing token '" +
                        close + "'"});
                    }
                }
                i += close.size() - 1;
                col += close.size() - 1;
                processed = true;
                break;
            }
        }
        if (processed) continue;
        if (code[i] == '(' || code[i] == '[' || code[i] == '{') {
            symbols.emplace(std::string(1, code[i]), line, col);
            continue;
        }
        if (code[i] == ')' || code[i] == ']' || code[i] == '}') {
            if (symbols.empty()) {
                errors.push_back({
                    line,
                    col,
                    std::string("Closing bracket '") + code[i] +
                    "' without matching opening bracket"});
                continue;
            }
            auto [openTok, openLine, openCol] = symbols.top();
            symbols.pop();
            if (openTok != std::string(1, bracketCloseToOpen.at(code[i]))) {
                errors.push_back({
                openLine,
                openCol,
                "Opening token '" + openTok + "' does not match closing bracket '" +
                code[i] + "'"});
            }
            continue;
        }
    }
    while (!symbols.empty()) {
        auto [tok, l, c] = symbols.top();
        symbols.pop();
        errors.push_back({l, c, "Opening token '" + tok + "' is not closed"});
    }
    return errors;
}

#endif // BRACKETCHECKER_CODE_PAIRS_CHECK_H
