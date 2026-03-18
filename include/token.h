#pragma once
#include <string>
#include <variant>
#include <iostream>

enum TokenType {
    /// SINGLE CHARACTERS TOKENS.
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE, COMMA, DOT,
    MINUS, PLUS, STAR, SLASH, SEMICOLON,

    // ONE OR TWO CHARACTERS TOKENS.
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,

    // LITERALS.
    IDENTIFIER, STRING, NUMBER,

    // KEYWORDS.
    CLASS, SUPER, THIS, FUNC, IF, ELSE, AND, OR, FALSE, TRUE, NIL, WHILE, FOR, RETURN, BREAK, VAR, PRINT,

    END_OF, // EOF is a macro in C++.
};

class Token {
public:
    std::string m_Lexeme;
    TokenType m_Type;
    int m_Line;

    // Design Note: all the types supported by the language are covered here.
    std::variant<std::monostate, // for nil value.
        std::string, double, bool> m_Literal;
public:
    Token(std::string lexeme, TokenType type, int line) : m_Lexeme(lexeme), m_Type(type), m_Line(line) {
        if (type == STRING || type == IDENTIFIER) {
            m_Literal = lexeme;
        } else {
            m_Literal = std::monostate{};
        }
    }

    // used for debugging purposes.
    std::string getTypeStr() const {
        switch (m_Type) {
            // Single-character tokens
            case TokenType::LEFT_PAREN: return "LEFT_PAREN";
            case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
            case TokenType::LEFT_BRACE: return "LEFT_BRACE";
            case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
            case TokenType::COMMA: return "COMMA";
            case TokenType::DOT: return "DOT";
            case TokenType::MINUS: return "MINUS";
            case TokenType::PLUS: return "PLUS";
            case TokenType::STAR: return "STAR";
            case TokenType::SLASH: return "SLASH";
            case TokenType::SEMICOLON: return "SEMICOLON";

            // One or two character tokens
            case TokenType::BANG: return "BANG";
            case TokenType::BANG_EQUAL: return "BANG_EQUAL";
            case TokenType::EQUAL: return "EQUAL";
            case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
            case TokenType::GREATER: return "GREATER";
            case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
            case TokenType::LESS: return "LESS";
            case TokenType::LESS_EQUAL: return "LESS_EQUAL";

            // Literals
            case TokenType::IDENTIFIER: return "IDENTIFIER";
            case TokenType::STRING: return "STRING";
            case TokenType::NUMBER: return "NUMBER";

            // Keywords
            case TokenType::CLASS: return "CLASS";
            case TokenType::SUPER: return "SUPER";
            case TokenType::THIS: return "THIS";
            case TokenType::FUNC: return "FUNC";
            case TokenType::IF: return "IF";
            case TokenType::ELSE: return "ELSE";
            case TokenType::AND: return "AND";
            case TokenType::OR: return "OR";
            case TokenType::FALSE: return "FALSE";
            case TokenType::TRUE: return "TRUE";
            case TokenType::NIL: return "NIL";
            case TokenType::WHILE: return "WHILE";
            case TokenType::FOR: return "FOR";
            case TokenType::RETURN: return "RETURN";
            case TokenType::BREAK: return "BREAK";
            case TokenType::VAR: return "VAR";
            case TokenType::PRINT: return "PRINT";

            case TokenType::END_OF: return "END_OF";
        }

        return "UNKNOWN";
    }

    // used for debugging purposes.
    void printToken() const {
        std::cout << "\033[32m-------------------------\033[0m\n";
        std::cout << "\033[34mLexeme: " << m_Lexeme << std::endl;
        std::cout << "\033[34mType: " << getTypeStr() << std::endl;
        std::cout << "\033[34mLine: " << m_Line << std::endl;

        // if the value is not a monostate, print the literal
        if (std::holds_alternative<std::string>(m_Literal)) {
            std::cout << "\033[34mLiteral: " << std::get<std::string>(m_Literal) << std::endl;
        }
        else if (std::holds_alternative<double>(m_Literal)) {
            std::cout << "\033[34mLiteral: " << std::get<double>(m_Literal) << std::endl;
        }
        else if (std::holds_alternative<bool>(m_Literal)) {
            std::cout << "\033[34mLiteral: " << std::get<bool>(m_Literal) << std::endl;
        }
    }
};
