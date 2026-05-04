#include "SemanticAnalyzer.h"
#include <stdexcept>
#include <iostream>

SemanticType SemanticAnalyzer::getTypeFromTypeNode(TypeNode* node) {
    if (!node) return SemanticType("void");
    
    if (auto basic = dynamic_cast<BasicTypeNode*>(node)) {
        std::string name = basic->typeName;
        if (name == "Ai_8") name = "u8";
        else if (name == "Ai_16") name = "u16";
        else if (name == "Ai_32") name = "u32";
        else if (name == "Ai_64") name = "u64";
        else if (name == "ri_8") name = "i8";
        else if (name == "ri_16") name = "i16";
        else if (name == "ri_32") name = "i32";
        else if (name == "ri_64") name = "i64";
        return SemanticType(name, false);
    } else if (auto ptr = dynamic_cast<PointerTypeNode*>(node)) {
        SemanticType base = getTypeFromTypeNode(ptr->baseType.get());
        base.isPtr = true;
        return base;
    } else if (auto arr = dynamic_cast<ArrayTypeNode*>(node)) {
        SemanticType base = getTypeFromTypeNode(arr->elementType.get());
        base.isPtr = true; // 배열은 시맨틱하게 포인터로 취급 가능
        return base;
    }
    return SemanticType("unknown");
}

SemanticType SemanticAnalyzer::checkExpression(Node* node) {
    if (!node) return SemanticType("void");

    if (auto num = dynamic_cast<NumberNode*>(node)) {
        return SemanticType("u32"); // 기본적으로 숫자는 u32로 추론
    } 
    else if (auto ident = dynamic_cast<IdentifierNode*>(node)) {
        SemanticType type;
        if (!symTable.lookup(ident->name, type)) {
            std::cerr << "PON! undefined variable '" << ident->name << "' intercepted!" << std::endl;
            throw std::runtime_error("Undefined variable");
        }
        return type;
    } 
    else if (auto binOp = dynamic_cast<BinaryOpNode*>(node)) {
        SemanticType left = checkExpression(binOp->left.get());
        SemanticType right = checkExpression(binOp->right.get());
        
        // 간단한 타입 체크 (연산자 양 옆의 타입이 같은지)
        if (left != right) {
            std::cerr << "PON! Type mismatch... You need Gaslight! (" << left.name << " vs " << right.name << ")" << std::endl;
            throw std::runtime_error("Type mismatch");
        }
        return left; // 결과 타입은 같다면 그대로 반환
    }
    else if (auto castNode = dynamic_cast<TypeCastNode*>(node)) {
        // Gaslight: 강제 형변환 기능. 표현식 검사만 하고, 타입을 강제로 변경함.
        checkExpression(castNode->expression.get()); 
        return getTypeFromTypeNode(castNode->targetType.get());
    }
    else if (auto callNode = dynamic_cast<FunctionCallNode*>(node)) {
        for (auto& arg : callNode->arguments) {
            checkExpression(arg.get());
        }
        return SemanticType("u32");
    }
    else if (auto strNode = dynamic_cast<StringNode*>(node)) {
        return SemanticType("string");
    }

    return SemanticType("unknown");
}

void SemanticAnalyzer::checkStatement(Node* node) {
    if (!node) return;

    if (auto letStmt = dynamic_cast<LetStatementNode*>(node)) {
        SemanticType exprType = checkExpression(letStmt->expression.get());
        symTable.define(letStmt->identifier, exprType);
    } 
    else if (auto exprStmt = dynamic_cast<ExpressionStatementNode*>(node)) {
        checkExpression(exprStmt->expression.get());
    }
    else if (auto retStmt = dynamic_cast<ReturnStatementNode*>(node)) {
        SemanticType exprType = checkExpression(retStmt->expression.get());
        if (exprType != currentFunctionReturnType) {
            std::cerr << "PON! Return type mismatch! Expected " << currentFunctionReturnType.name 
                      << " but got " << exprType.name << std::endl;
            throw std::runtime_error("Return type mismatch");
        }
    }
    else if (auto printStmt = dynamic_cast<PrintStatementNode*>(node)) {
        checkExpression(printStmt->expression.get());
    }
    else if (auto blockNode = dynamic_cast<BlockNode*>(node)) {
        analyzeBlock(blockNode);
    }
    else if (auto ifStmt = dynamic_cast<IfStatementNode*>(node)) {
        SemanticType condType = checkExpression(ifStmt->condition.get());
        if (condType.name != "bool") {
            std::cerr << "PON! If condition must be 'bool'." << std::endl;
            throw std::runtime_error("Condition type mismatch");
        }
        checkStatement(ifStmt->thenBlock.get());
        if (ifStmt->elseBlock) {
            checkStatement(ifStmt->elseBlock.get());
        }
    }
}

void SemanticAnalyzer::analyzeBlock(BlockNode* block) {
    symTable.enterScope();
    for (auto& stmt : block->statements) {
        checkStatement(stmt.get());
    }
    symTable.exitScope();
}

void SemanticAnalyzer::analyzeFunction(FunctionNode* func) {
    std::string ret = func->returnType;
    if (ret == "Ai_8") ret = "u8";
    else if (ret == "Ai_16") ret = "u16";
    else if (ret == "Ai_32") ret = "u32";
    else if (ret == "Ai_64") ret = "u64";
    else if (ret == "ri_8") ret = "i8";
    else if (ret == "ri_16") ret = "i16";
    else if (ret == "ri_32") ret = "i32";
    else if (ret == "ri_64") ret = "i64";

    currentFunctionReturnType = SemanticType(ret, false);

    symTable.enterScope();
    
    // 파라미터 등록
    for (const auto& param : func->parameters) {
        SemanticType pType = getTypeFromTypeNode(param.type.get());
        symTable.define(param.name, pType);
    }

    if (auto block = dynamic_cast<BlockNode*>(func->body.get())) {
        analyzeBlock(block);
    } else {
        checkStatement(func->body.get());
    }

    symTable.exitScope();
}

void SemanticAnalyzer::analyze(ProgramNode* root) {
    std::cout << "Starting Semantic Analysis (Airi Lang)..." << std::endl;

    for (auto& stmt : root->statements) {
        if (auto funcNode = dynamic_cast<FunctionNode*>(stmt.get())) {
            // 이후 1 Pass 방식: 함수 정보 전역에 저장하는 과정은 생략 
            // (구조체 선언이나 전역 변수 대응 시 pass 분리 필요)
            analyzeFunction(funcNode);
        }
        else if (auto structNode = dynamic_cast<StructDeclarationNode*>(stmt.get())) {
            // 구조체 정의
            symTable.define(structNode->name, SemanticType("struct"));
        }
    }
    
    std::cout << "Semantic Analysis completed! No PON intercepted." << std::endl;
}
