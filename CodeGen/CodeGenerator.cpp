#include "CodeGenerator.h"

void CodeGenerator::indent() {
    outputCode += std::string(indentLevel * 4, ' ');
}

void CodeGenerator::append(const std::string& text) {
    outputCode += text;
}

void CodeGenerator::appendLine(const std::string& text) {
    indent();
    outputCode += text + "\n";
}

std::string CodeGenerator::generateType(TypeNode* type) {
    if (!type) return "void";
    
    if (auto basic = dynamic_cast<BasicTypeNode*>(type)) {
        // C++ 표준 타입으로 변환
        if (basic->typeName == "u8" || basic->typeName == "Ai_8") return "uint8_t";
        if (basic->typeName == "u16" || basic->typeName == "Ai_16") return "uint16_t";
        if (basic->typeName == "u32" || basic->typeName == "Ai_32") return "uint32_t";
        if (basic->typeName == "u64" || basic->typeName == "Ai_64") return "uint64_t";
        if (basic->typeName == "i8" || basic->typeName == "ri_8") return "int8_t";
        if (basic->typeName == "i16" || basic->typeName == "ri_16") return "int16_t";
        if (basic->typeName == "i32" || basic->typeName == "ri_32") return "int32_t";
        if (basic->typeName == "i64" || basic->typeName == "ri_64") return "int64_t";
        if (basic->typeName == "bool") return "bool";
        return basic->typeName; // 구조체 등
    } else if (auto ptr = dynamic_cast<PointerTypeNode*>(type)) {
        return generateType(ptr->baseType.get()) + "*";
    } else if (auto arr = dynamic_cast<ArrayTypeNode*>(type)) {
        // C-style 배열은 타입 선언이 복잡하므로 간단히 포인터 처리 고려
        // 본 구현에서는 포인터로 취급하거나 이름 뒤에 붙이는 방식을 써야 함.
        return generateType(arr->elementType.get()) + "*"; 
    }
    return "void";
}

std::string CodeGenerator::generateExpression(Node* expr) {
    if (!expr) return "";

    if (auto num = dynamic_cast<NumberNode*>(expr)) {
        return std::to_string(num->value);
    } 
    else if (auto ident = dynamic_cast<IdentifierNode*>(expr)) {
        return ident->name == "Wolf" ? "true" : (ident->name == "woof" ? "false" : ident->name);
    } 
    else if (auto binOp = dynamic_cast<BinaryOpNode*>(expr)) {
        std::string op = binOp->op;
        // Airi Lang 커스텀 연산자 복구
        if (op == "A") op = "+";
        else if (op == "i") op = "-";
        else if (op == "r") op = "*";
        else if (op == "I") op = "/";
        else if (op == "PON?") op = "==";
        else if (op == "!PON?") op = "!=";
        else if (op == "...") op = "=";

        return "(" + generateExpression(binOp->left.get()) + " " + op + " " + generateExpression(binOp->right.get()) + ")";
    }
    else if (auto castNode = dynamic_cast<TypeCastNode*>(expr)) {
        std::string target = generateType(castNode->targetType.get());
        std::string inner = generateExpression(castNode->expression.get());
        return "((" + target + ")" + inner + ")";
    }
    else if (auto callNode = dynamic_cast<FunctionCallNode*>(expr)) {
        std::string call = callNode->functionName + "(";
        for (size_t i = 0; i < callNode->arguments.size(); ++i) {
            call += generateExpression(callNode->arguments[i].get());
            if (i < callNode->arguments.size() - 1) call += ", ";
        }
        call += ")";
        return call;
    }
    else if (auto strNode = dynamic_cast<StringNode*>(expr)) {
        return "\"" + strNode->value + "\"";
    }
    return "";
}

void CodeGenerator::generateStatement(Node* stmt) {
    if (!stmt) return;

    if (auto letStmt = dynamic_cast<LetStatementNode*>(stmt)) {
        // C++11 auto 타입 추론 활용
        std::string expr = generateExpression(letStmt->expression.get());
        appendLine("auto " + letStmt->identifier + " = " + expr + ";");
    } 
    else if (auto exprStmt = dynamic_cast<ExpressionStatementNode*>(stmt)) {
        std::string expr = generateExpression(exprStmt->expression.get());
        appendLine(expr + ";");
    }
    else if (auto retStmt = dynamic_cast<ReturnStatementNode*>(stmt)) {
        std::string expr = generateExpression(retStmt->expression.get());
        appendLine("return " + expr + ";");
    }
    else if (auto printStmt = dynamic_cast<PrintStatementNode*>(stmt)) {
        std::string expr = generateExpression(printStmt->expression.get());
        appendLine("std::cout << " + expr + " << std::endl;");
    }
    else if (auto blockNode = dynamic_cast<BlockNode*>(stmt)) {
        generateBlock(blockNode);
    }
    else if (auto ifStmt = dynamic_cast<IfStatementNode*>(stmt)) {
        std::string cond = generateExpression(ifStmt->condition.get());
        appendLine("if (" + cond + ")");
        generateStatement(ifStmt->thenBlock.get());
        if (ifStmt->elseBlock) {
            appendLine("else");
            generateStatement(ifStmt->elseBlock.get());
        }
    }
}

void CodeGenerator::generateBlock(BlockNode* block) {
    appendLine("{");
    indentLevel++;
    for (auto& stmt : block->statements) {
        generateStatement(stmt.get());
    }
    indentLevel--;
    appendLine("}");
}

std::string CodeGenerator::generate(ProgramNode* root) {
    outputCode = "#include <cstdint>\n#include <stdbool.h>\n#include <iostream>\n#include <string>\n\n";

    for (auto& stmt : root->statements) {
        if (auto structNode = dynamic_cast<StructDeclarationNode*>(stmt.get())) {
            appendLine("struct " + structNode->name + " {");
            indentLevel++;
            for (const auto& member : structNode->members) {
                appendLine(generateType(member.type.get()) + " " + member.name + ";");
            }
            indentLevel--;
            appendLine("};\n");
        }
        else if (auto funcNode = dynamic_cast<FunctionNode*>(stmt.get())) {
            // C++는 포인터 타입을 파싱할 때 AST에 TypeNode로 저장해야 하나,
            // 현 AST 구현상 returnType이 string입니다.
            // 일단 단순화하여 string 기반 매핑 추가
            std::string ret = funcNode->returnType;
            if (funcNode->name == "main") {
                ret = "int";
            } else {
                if (ret == "u8" || ret == "Ai_8") ret = "uint8_t";
                else if (ret == "u16" || ret == "Ai_16") ret = "uint16_t";
                else if (ret == "u32" || ret == "Ai_32") ret = "uint32_t";
                else if (ret == "u64" || ret == "Ai_64") ret = "uint64_t";
                else if (ret == "i32" || ret == "ri_32") ret = "int32_t";
            }
            
            std::string sig = ret + " " + funcNode->name + "(";
            for (size_t i = 0; i < funcNode->parameters.size(); ++i) {
                sig += generateType(funcNode->parameters[i].type.get()) + " " + funcNode->parameters[i].name;
                if (i < funcNode->parameters.size() - 1) sig += ", ";
            }
            sig += ")";
            appendLine(sig);
            
            if (auto block = dynamic_cast<BlockNode*>(funcNode->body.get())) {
                generateBlock(block);
            } else {
                appendLine("{");
                indentLevel++;
                generateStatement(funcNode->body.get());
                indentLevel--;
                appendLine("}");
            }
            appendLine(""); // 함수 간 개행
        }
    }
    return outputCode;
}
