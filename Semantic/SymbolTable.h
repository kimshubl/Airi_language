#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream>

// 간단한 타입 표현을 위한 구조체
struct SemanticType {
    std::string name;
    bool isPtr;
    SemanticType(std::string n = "", bool ptr = false) : name(n), isPtr(ptr) {}

    bool operator==(const SemanticType& other) const {
        return name == other.name && isPtr == other.isPtr;
    }
    bool operator!=(const SemanticType& other) const {
        return !(*this == other);
    }
};

class SymbolTable {
private:
    std::vector<std::unordered_map<std::string, SemanticType>> scopes;

public:
    SymbolTable() {
        // 전역 스코프 초기화
        enterScope();
    }

    void enterScope() {
        scopes.push_back({});
    }

    void exitScope() {
        if (!scopes.empty()) {
            scopes.pop_back();
        }
    }

    void define(const std::string& name, const SemanticType& type) {
        if (scopes.empty()) return;
        scopes.back()[name] = type;
    }

    bool lookup(const std::string& name, SemanticType& outType) {
        // 현재 스코프부터 상위 스코프로 탐색
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) {
                outType = found->second;
                return true;
            }
        }
        return false;
    }
};
