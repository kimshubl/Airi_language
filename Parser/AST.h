#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>

// 모든 노드의 기본 클래스
struct Node {
    virtual ~Node() = default;
};


struct IfStatementNode : Node {
    std::unique_ptr<Node> condition;
    std::unique_ptr<Node> thenBlock; // BlockNode를 담는 바구니
    std::unique_ptr<Node> elseBlock; // (선택 사항)
    
    IfStatementNode(std::unique_ptr<Node> c, std::unique_ptr<Node> t, 
        std::unique_ptr<Node> e): 
        condition(std::move(c)),
        thenBlock(std::move(t)),
        elseBlock(std::move(e))
        {}

};

struct BlockNode : Node {
    std::vector<std::unique_ptr<Node>> statements;
    BlockNode(std::vector<std::unique_ptr<Node>> s) : statements(std::move(s)) {}
};

// 모든 타입 노드의 부모
struct TypeNode : Node {
    virtual ~TypeNode() = default;
};

// 기본 타입 (u8, u32, bool 등)
struct BasicTypeNode : TypeNode {
    std::string typeName; // "u32"
    BasicTypeNode(std::string name) : typeName(std::move(name)) {}
};

// 포인터 타입 (ptr<T>)
struct PointerTypeNode : TypeNode {
    std::unique_ptr<TypeNode> baseType; // 가리키는 대상의 타입
    PointerTypeNode(std::unique_ptr<TypeNode> base) : baseType(std::move(base)) {}
};

// 배열 타입 (u8[512])
struct ArrayTypeNode : TypeNode {
    std::unique_ptr<TypeNode> elementType; // 배열 요소의 타입
    int size;                             // 배열의 크기 
    ArrayTypeNode(std::unique_ptr<TypeNode> elem, int s) 
        : elementType(std::move(elem)), size(s) {}
};

struct Parameter {
    std::string name;
    std::unique_ptr<TypeNode> type; // string -> TypeNode로 업그레이드!
};

struct FunctionNode : Node {
    std::string name;                          // 함수 이름 (예: main, add) 
    std::vector<Parameter> parameters;         // 매개변수들의 목록 
    std::string returnType;                    // 반환 타입 (예: u32, ptr<u16>) [cite: 5, 6]
    std::unique_ptr<Node> body;                // 함수 내부 블록 { ... } 

    FunctionNode(std::string n, std::vector<Parameter> p, std::string rt, std::unique_ptr<Node> b)
        : name(std::move(n)), parameters(std::move(p)), 
          returnType(std::move(rt)), body(std::move(b)) {}
};


// 숫자 리터럴 (예: 10, 20)
struct NumberNode : Node {
    int value;
    NumberNode(int v) : value(v) {}
};

struct IdentifierNode : Node {
    std::string name;
    IdentifierNode(std::string n) : name(std::move(n)) {}
};

// 이항 연산 (예: 10 + 20)
struct BinaryOpNode : Node {
    std::string op;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    BinaryOpNode(std::string o, std::unique_ptr<Node> l, std::unique_ptr<Node> r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
};

//삼항연산
struct TenaryOpNode : Node{   
    std::string condition;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    TenaryOpNode(std::string c, std::unique_ptr<Node> l, std::unique_ptr<Node> r)
        : condition(std::move(c)), left(std::move(l)), right(std::move(r)) {}

    
};

//N항 연산자
struct NaryOpNode : Node {
    std::string name; // 함수 이름 등
    std::vector<std::unique_ptr<Node>> children; // 자식 노드들의 목록

    NaryOpNode(std::string n, std::vector<std::unique_ptr<Node>> c)
        : name(n), children(std::move(c)) {}
};


// 변수 선언문 (예: let x = ...)
struct LetStatementNode : Node {
    std::string identifier;
    std::unique_ptr<Node> expression;

    LetStatementNode(std::string id, std::unique_ptr<Node> expr)
        : identifier(id), expression(std::move(expr)) {}
};


// 표현식 구문 (예: a + b;)
struct ExpressionStatementNode : Node {
    std::unique_ptr<Node> expression;

    ExpressionStatementNode(std::unique_ptr<Node> expr)
        : expression(std::move(expr)) {}
};

// 반환문 (현 컨셉 상 PON 등, 예: PON x;)
struct ReturnStatementNode : Node {
    std::unique_ptr<Node> expression;

    ReturnStatementNode(std::unique_ptr<Node> expr)
        : expression(std::move(expr)) {}
};

// 문자열 리터럴 (예: "Hello World")
struct StringNode : Node {
    std::string value;
    StringNode(std::string v) : value(std::move(v)) {}
};

// 출력문 (phase_connects_famous_azooma "...", 콘솔 출력)
struct PrintStatementNode : Node {
    std::unique_ptr<Node> expression;
    PrintStatementNode(std::unique_ptr<Node> expr) : expression(std::move(expr)) {}
};

// 구조체 멤버
struct StructMember {
    std::string name;
    std::unique_ptr<TypeNode> type;
};

// 구조체 선언문 (Airis 구조체)
struct StructDeclarationNode : Node {
    std::string name;
    std::vector<StructMember> members;

    StructDeclarationNode(std::string n, std::vector<StructMember> m)
        : name(std::move(n)), members(std::move(m)) {}
};

// 타입 캐스트 명시 (Gaslight)
struct TypeCastNode : Node {
    std::unique_ptr<Node> expression;
    std::unique_ptr<TypeNode> targetType;

    TypeCastNode(std::unique_ptr<Node> expr, std::unique_ptr<TypeNode> type)
        : expression(std::move(expr)), targetType(std::move(type)) {}
};

// 함수 호출 (예: add(1, 2))
struct FunctionCallNode : Node {
    std::string functionName;
    std::vector<std::unique_ptr<Node>> arguments;

    FunctionCallNode(std::string name, std::vector<std::unique_ptr<Node>> args)
        : functionName(std::move(name)), arguments(std::move(args)) {}
};

// 프로그램 전체 (루트)
struct ProgramNode : Node {
    std::vector<std::unique_ptr<Node>> statements; // 함수나 구조체 선언문들

    ProgramNode(std::vector<std::unique_ptr<Node>> s) : statements(std::move(s)) {}
};