#ifndef KEYWORD_DIC_H
#define KEYWORD_DIC_H
#include <unordered_map>
#include "TokenType.h"



static const std::unordered_map<std::string, TokenType> keyword_dic =
{
    {"Ai", TokenType::KW_FN},
    {"ri", TokenType::KW_LET},
    {"Airis", TokenType::KW_STRUCT},
    {"Airi!!!", TokenType::KW_INTERRUPT}, // handled in lexer specially or here if we allow ! in ident
    {"Phone_queen", TokenType::KW_ASM},
    {"PON", TokenType::KW_RETURN},
    {"phase_connects_famous_azooma", TokenType::KW_PRINT},    
    {"Colony", TokenType::KW_MMIO},
    {"Airhead", TokenType::KW_VOLATILE},

    {"Ai_8", TokenType::TYPE_U8},
    {"Ai_16", TokenType::TYPE_U16},
    {"Ai_32", TokenType::TYPE_U32},
    {"Ai_64", TokenType::TYPE_U64},

    {"ri_8", TokenType::TYPE_I8},
    {"ri_16", TokenType::TYPE_I16},
    {"ri_32", TokenType::TYPE_I32},
    {"ri_64", TokenType::TYPE_I64},

    {"Wolf", TokenType::TYPE_BOOL},
    {"woof", TokenType::TYPE_BOOL}, // false
    {"AiriType", TokenType::TYPE_PTR},
    
    // Alphanumeric Operators
    {"A", TokenType::PLUS},
    {"i", TokenType::MINUS},
    {"r", TokenType::STAR},
    {"I", TokenType::SLASH},
    {"Gaslight", TokenType::AMPERSAND}
};
#endif // KEYWORD_DIC_H