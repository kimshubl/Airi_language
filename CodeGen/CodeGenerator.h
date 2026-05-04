#pragma once

#include "../Parser/AST.h"
#include <string>
#include <vector>

class CodeGenerator {
private:
    std::string outputCode;
    int indentLevel = 0;

    void indent();
    void append(const std::string& text);
    void appendLine(const std::string& text);

    std::string generateType(TypeNode* type);
    std::string generateExpression(Node* expr);
    void generateStatement(Node* stmt);
    void generateBlock(BlockNode* block);

public:
    CodeGenerator() {}

    // 트랜스파일 시작: 루트 노드를 받아 C++ 코드를 생성
    std::string generate(ProgramNode* root);
    
    // 생성을 마친 완성된 코드 문자열 반환
    const std::string& getCode() const { return outputCode; }
};
