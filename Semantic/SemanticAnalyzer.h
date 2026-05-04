#pragma once

#include "../Parser/AST.h"
#include "SymbolTable.h"
#include <string>

class SemanticAnalyzer {
private:
    SymbolTable symTable;
    SemanticType currentFunctionReturnType;

    // AST 순회 기본 함수
    SemanticType checkExpression(Node* node);
    void checkStatement(Node* node);

    // 구체적인 타입 유추/검사 헬퍼
    SemanticType getTypeFromTypeNode(TypeNode* node);

public:
    SemanticAnalyzer() {}

    void analyze(ProgramNode* root);
    
    // 특정 함수나 블록을 개별적으로 검사할 수 있는 엔트리 포인트
    void analyzeFunction(FunctionNode* func);
    void analyzeBlock(BlockNode* block);
};
