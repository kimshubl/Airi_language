#pragma once

#include "../keyword/Keyword_dic.h"
#include "../keyword/TokenType.h"
#include <string>
#include <cctype>
#include <iostream>
#include <stdexcept>

class Lexer {
    std::string src;
    size_t pos = 0;
    int line = 1;
    int column = 1;

    void advance() {
        if (pos < src.length()) {
            if (src[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
        }
    }

    void advance(int count) {
        for (int i = 0; i < count; ++i) advance();
    }

public:

    Lexer(std::string source) : src(std::move(source)) {}

    Token next_token() {
        // whitespace skip
        while (pos < src.length() && std::isspace(src[pos])) {
            advance();
        }

        // EOF
        if (pos >= src.length()) {
            return Token{TokenType::END_OF_FILE, ""};
        }

        char c = src[pos];

        // -------------------------
        // Airi Lang Punctuation & Operators
        // -------------------------
        auto match = [&](const std::string& pattern) {
            if (pos + pattern.length() <= src.length() && 
                src.substr(pos, pattern.length()) == pattern) {
                advance(pattern.length());
                return true;
            }
            return false;
        };

        // Comments (skip)
        if (pos + 1 < src.length() && src.substr(pos, 2) == "//") {
            advance(2);
            while (pos < src.length() && src[pos] != '\n') { advance(); }
            return next_token();
        }
        if (pos + 1 < src.length() && src.substr(pos, 2) == "/*") {
            advance(2);
            while (pos + 1 < src.length()) {
                if (src[pos] == '*' && src[pos+1] == '/') {
                    advance(2);
                    break;
                }
                advance();
            }
            return next_token();
        }

        // Punctuation (Order matters for overlapping prefixes!)
        if (match("...iriA")) return Token{TokenType::RBRACE, "...iriA"};
        if (match("...ri")) return Token{TokenType::RPAREN, "...ri"};
        if (match("...")) return Token{TokenType::ASSIGN, "..."};
        if (match("..")) return Token{TokenType::COLON, ".."};
        if (match(".")) return Token{TokenType::COMMA, "."};
        
        if (match("Airi...")) return Token{TokenType::LBRACE, "Airi..."};
        if (match("Airi>")) return Token{TokenType::ARROW, "Airi>"};
        if (match("Ai...")) return Token{TokenType::LPAREN, "Ai..."};

        if (match("!PON?")) return Token{TokenType::NOT_EQ, "!PON?"};
        if (match("PON?")) return Token{TokenType::EQ_EQ, "PON?"};

        if (match(",")) return Token{TokenType::SEMICOLON, ","};


        // -------------------------
        // NUMBER (Airi_ for Binary, Gacha_ for Hex)
        // -------------------------
        if (pos + 5 <= src.length() && src.substr(pos, 5) == "Airi_") {
            advance(5);
            std::string num;
            while (pos < src.length()) {
                if (std::isspace(src[pos])) {
                    advance(); // ignore spaces in binary string
                    continue;
                }
                if (pos + 1 < src.length() && src.substr(pos, 2) == "Ai") {
                    num += "1";
                    advance(2);
                } else if (pos + 1 < src.length() && src.substr(pos, 2) == "ri") {
                    num += "0";
                    advance(2);
                } else {
                    break;
                }
            }
            if (!num.empty()) {
                long long val = std::stoll(num, nullptr, 2);
                return Token{TokenType::NUMBER, std::to_string(val)};
            }
        }

        if (pos + 6 <= src.length() && src.substr(pos, 6) == "Gacha_") {
            advance(6);
            std::string hexNum;
            while (pos < src.length() && std::isxdigit(src[pos])) {
                hexNum += src[pos];
                advance();
            }
            if (!hexNum.empty()) {
                long long val = std::stoll(hexNum, nullptr, 16);
                return Token{TokenType::NUMBER, std::to_string(val)};
            }
        }

        // -------------------------
        // STRING LITERAL
        // -------------------------
        if (c == '"') {
            advance();
            std::string str;
            while (pos < src.length() && src[pos] != '"') {
                str += src[pos];
                advance();
            }
            if (pos < src.length() && src[pos] == '"') {
                advance(); // consume closing quote
            } else {
                std::cerr << "PON! Airi missed the closing quote at line " << line << ", col " << column << std::endl;
                throw std::runtime_error("Unterminated string literal (PON!)");
            }
            return Token{TokenType::STRING, str};
        }

        // Normal Numbers (Fallback, if user types normal numbers)
        if (std::isdigit(c)) {
            std::string num;
            while (pos < src.length() && std::isdigit(src[pos])) {
                num += src[pos];
                advance();
            }
            return Token{TokenType::NUMBER, num};
        }

        // -------------------------
        // IDENTIFIER / KEYWORD
        // -------------------------
        if (std::isalpha(c) || c == '_' || c == '!') {
            std::string ident;

            while (pos < src.length() &&
                  (std::isalnum(src[pos]) || src[pos] == '_' || src[pos] == '!')) {
                ident += src[pos];
                advance();
            }

            auto it = keyword_dic.find(ident);
            if (it != keyword_dic.end()) {
                return Token{it->second, ident};
            }

            return Token{TokenType::IDENTIFIER, ident};
        }

        // UNKNOWN - PON ERROR HANDLING
        std::cerr << "PON! Airi messed up at line " << line << ", col " << column 
                  << ": Unrecognized token '" << c << "'" << std::endl;
        throw std::runtime_error("Lexer encountered unknown token (PON!)");
    }
};
