#pragma once
#include <vector>
#include <memory>
#include <stdexcept>
#include <string>
#include "../keyword/TokenType.h"
#include "AST.h"

class Parser {
public:
    Parser(const std::vector<Token>& tokens) : m_tokens(tokens), m_pos(0) {}
    std::unique_ptr<Node> parseProgram();

private:
    std::vector<Token> m_tokens;
    size_t m_pos = 0;

    Token peek(int offset = 0);
    Token consume(TokenType expected);
    Token consume();
    bool match(TokenType t);
    int getPrecedence(const Token& token);

    std::unique_ptr<Node> parseFunction();
    std::vector<Parameter> parseParameterList();
    std::unique_ptr<TypeNode> parseType();
    std::unique_ptr<Node> parseBlock();
    std::unique_ptr<Node> parseStatement();
    std::unique_ptr<Node> parseExpression(int min_precedence = 0);
    std::unique_ptr<Node> parsePrimary();
};