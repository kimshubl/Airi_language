#include "Parser.h"
#include <iostream>

Token Parser::peek(int offset) {
    if (m_pos + offset >= m_tokens.size()) {
        return Token{TokenType::END_OF_FILE, ""};
    }
    return m_tokens[m_pos + offset];
}

Token Parser::consume(TokenType expected) {
    if (peek().type == expected) {
        return m_tokens[m_pos++];
    }
    throw std::runtime_error("Parser PON! Expected token " + std::to_string((int)expected) + " but got " + peek().value);
}

Token Parser::consume() {
    if (m_pos >= m_tokens.size()) return Token{TokenType::END_OF_FILE, ""};
    return m_tokens[m_pos++];
}

bool Parser::match(TokenType t) {
    if (peek().type == t) {
        m_pos++;
        return true;
    }
    return false;
}

int Parser::getPrecedence(const Token& token) {
    switch (token.type) {
        case TokenType::EQ_EQ:
        case TokenType::NOT_EQ:
            return 1;
        case TokenType::PLUS:
        case TokenType::MINUS:
            return 2;
        case TokenType::STAR:
        case TokenType::SLASH:
            return 3;
        default:
            return 0;
    }
}

std::unique_ptr<Node> Parser::parseProgram() {
    std::vector<std::unique_ptr<Node>> stmts;
    while (peek().type != TokenType::END_OF_FILE) {
        if (peek().type == TokenType::KW_FN) { // "Ai"
            stmts.push_back(parseFunction());
        } else if (peek().type == TokenType::KW_STRUCT) {
            consume();
        } else {
            throw std::runtime_error("Parser PON! Unknown global declaration.");
        }
    }
    return std::make_unique<ProgramNode>(std::move(stmts));
}

std::unique_ptr<TypeNode> Parser::parseType() {
    Token t = consume();
    // BasicTypeNode는 typeName 하나만 저장 (필요 시 더 디테일하게 분기 가능)
    return std::make_unique<BasicTypeNode>(t.value);
}

std::vector<Parameter> Parser::parseParameterList() {
    std::vector<Parameter> params;
    if (peek().type != TokenType::RPAREN) {
        do {
            std::string name = consume(TokenType::IDENTIFIER).value;
            consume(TokenType::COLON); // ".."
            auto type = parseType();
            params.push_back({name, std::move(type)});
        } while (match(TokenType::COMMA)); // "."
    }
    return params;
}

std::unique_ptr<Node> Parser::parseBlock() {
    std::vector<std::unique_ptr<Node>> stmts;
    consume(TokenType::LBRACE); // "Airi..."
    while (peek().type != TokenType::RBRACE && peek().type != TokenType::END_OF_FILE) {
        stmts.push_back(parseStatement());
    }
    consume(TokenType::RBRACE); // "...iriA"
    return std::make_unique<BlockNode>(std::move(stmts));
}

std::unique_ptr<Node> Parser::parseFunction() {
    consume(TokenType::KW_FN); // "Ai"
    std::string name = consume(TokenType::IDENTIFIER).value;
    consume(TokenType::LPAREN); // "Ai..."
    
    auto params = parseParameterList();
    
    consume(TokenType::RPAREN); // "...ri"
    
    std::string retType = "void";
    if (match(TokenType::ARROW)) { // "Airi>"
        // AST 구현상 function returnType을 문자열로 보관
        retType = consume().value;
    }
    
    auto body = parseBlock();
    
    return std::make_unique<FunctionNode>(name, std::move(params), retType, std::move(body));
}

std::unique_ptr<Node> Parser::parseStatement() {
    if (match(TokenType::KW_RETURN)) { // "PON"
        auto expr = parseExpression();
        consume(TokenType::SEMICOLON); // ","
        return std::make_unique<ReturnStatementNode>(std::move(expr));
    }
    if (match(TokenType::KW_PRINT)) { // "phase_connects_famous_azooma"
        auto expr = parseExpression();
        consume(TokenType::SEMICOLON); // ","
        return std::make_unique<PrintStatementNode>(std::move(expr));
    }
    if (match(TokenType::KW_LET)) { // "ri"
        std::string id = consume(TokenType::IDENTIFIER).value;
        consume(TokenType::ASSIGN); // "..."
        auto expr = parseExpression();
        consume(TokenType::SEMICOLON);
        return std::make_unique<LetStatementNode>(id, std::move(expr));
    }
    // 표현식
    auto expr = parseExpression();
    consume(TokenType::SEMICOLON);
    return std::make_unique<ExpressionStatementNode>(std::move(expr));
}

std::unique_ptr<Node> Parser::parsePrimary() {
    Token t = consume();
    if (t.type == TokenType::NUMBER) {
        return std::make_unique<NumberNode>(std::stoi(t.value));
    }
    if (t.type == TokenType::STRING) {
        return std::make_unique<StringNode>(t.value);
    }
    if (t.type == TokenType::IDENTIFIER) {
        if (peek().type == TokenType::LPAREN) { // "Ai..."
            consume(TokenType::LPAREN);
            std::vector<std::unique_ptr<Node>> args;
            if (peek().type != TokenType::RPAREN) {
                do {
                    args.push_back(parseExpression());
                } while (match(TokenType::COMMA));
            }
            consume(TokenType::RPAREN);
            return std::make_unique<FunctionCallNode>(t.value, std::move(args));
        }
        return std::make_unique<IdentifierNode>(t.value);
    }
    if (t.type == TokenType::LPAREN) { // "Ai..."
        auto expr = parseExpression();
        consume(TokenType::RPAREN); // "...ri"
        return expr;
    }
    throw std::runtime_error("Parser PON! Expected primary expression but got '" + t.value + "'");
}

std::unique_ptr<Node> Parser::parseExpression(int min_precedence) {
    auto left = parsePrimary();

    while (true) {
        Token op = peek();
        int precedence = getPrecedence(op);

        if (precedence <= min_precedence) break;

        consume(); // 연산자 소비
        
        auto right = parseExpression(precedence); 
        left = std::make_unique<BinaryOpNode>(op.value, std::move(left), std::move(right));
    }
    return left;
}