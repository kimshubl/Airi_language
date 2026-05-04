#pragma once
#include <string>


enum class TokenType
{
    // literals
    IDENTIFIER,
    NUMBER,
    STRING,

    // keywords
    KW_FN,
    KW_LET,
    KW_STRUCT,
    KW_INTERRUPT,
    KW_ASM,
    KW_RETURN,
    KW_PRINT,
    // types
    TYPE_U8,
    TYPE_U16,
    TYPE_U32,
    TYPE_U64,
    TYPE_I8,
    TYPE_I16,
    TYPE_I32,
    TYPE_I64,
    TYPE_BOOL,
    TYPE_PTR,

    // memory / hardware
    KW_MMIO,
    KW_VOLATILE,

    // operators
    PLUS,
    MINUS,
    STAR,
    SLASH,
    ASSIGN,
    AMPERSAND,

    // punctuation
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    LBRACKET,
    RBRACKET,
    COLON,
    SEMICOLON,
    COMMA,
    ARROW,

    // special
    END_OF_FILE,
    UNKNOWN,

    //bit_operators
    EQ_EQ,
    NOT_EQ,
    LESS,
    LESS_EQ,
    GREATER,
    GREATER_EQ,
    SHIFT_LEFT,
    SHIFT_RIGHT,
    EQUAL

};

struct Token {
    TokenType type;
    std::string value;
};
