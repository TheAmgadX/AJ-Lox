#include "scanner.h"
#include "aj_lox.h"
#include "token.h"
#include <cstdio>
#include <charconv>

void Scanner::initKeywords() {
    m_Keywords["and"] = AND;
    m_Keywords["class"] = CLASS;
    m_Keywords["else"] = ELSE;
    m_Keywords["false"] = FALSE;
    m_Keywords["for"] = FOR;
    m_Keywords["fun"] = FUNC;
    m_Keywords["if"] = IF;
    m_Keywords["nil"] = NIL;
    m_Keywords["or"] = OR;
    m_Keywords["print"] = PRINT;
    m_Keywords["return"] = RETURN;
    m_Keywords["super"] = SUPER;
    m_Keywords["this"] = THIS;
    m_Keywords["true"] = TRUE;
    m_Keywords["var"] = VAR;
    m_Keywords["while"] = WHILE;
}

void Scanner::scanTokens(std::vector<Token>& tokens) {
    // it may reserve more than needed, but it's a reasonable approximation
    // most likely the vector will be destroyed soon, so no problem if it reserves more space than needed.
    tokens.reserve(m_Source.size() / 3);

    while(!isAtEnd()) {
        m_Start = m_Current;
        scanToken(tokens);
    }

    tokens.emplace_back("", END_OF, m_Line);
}

void Scanner::scanToken(std::vector<Token>& tokens) {
    char c = advance();

    switch (c) {
        case '(': addToken(tokens, LEFT_PAREN); break;
        case ')': addToken(tokens, RIGHT_PAREN); break;
        case '{': addToken(tokens, LEFT_BRACE); break;
        case '}': addToken(tokens, RIGHT_BRACE); break;
        case ',': addToken(tokens, COMMA); break;
        case '.': addToken(tokens, DOT); break;
        case '-': addToken(tokens, MINUS); break;
        case '+': addToken(tokens, PLUS); break;
        case '*': addToken(tokens, STAR); break;
        case ';':addToken(tokens, SEMICOLON); break;
        case '!': addToken(tokens, match('=') ? BANG_EQUAL : BANG); break;
        case '=': addToken(tokens, match('=') ? EQUAL_EQUAL : EQUAL); break;
        case '>': addToken(tokens, match('=') ? GREATER_EQUAL : GREATER); break;
        case '<': addToken(tokens, match('=') ? LESS_EQUAL : LESS); break;
        case '/':
            if(match('/')) {
                // if comment, neglect the rest of the line
                while(peek() != '\n' && !isAtEnd()) {
                    advance();
                }
            } else {
                addToken(tokens, SLASH);
            }
        case ' ':
        case '\t':
        case '\r':
            break;

        case '\n': m_Line++; break;

        case '"':
            addString(tokens); break;

        default:
            if(isDigit(c)) {
                addNumber(tokens);
            } else if(isAlpha(c)) {
                addIdentifier(tokens);
            } else {
                AJ_Lox::error(m_Line, "Unexpected character: " + std::string(1, c)); break;
            }
    }
}

bool Scanner::isAtEnd() const {
    return m_Current >= m_Source.size();
}

char Scanner::advance() {
    return m_Source[m_Current++];
}

bool Scanner::match(char expected) {
    if (isAtEnd() || m_Source[m_Current] != expected) {
        return false;
    }

    m_Current++;
    return true;
}

char Scanner::peek() const {
    if (isAtEnd()) {
        return '\0';
    }

    return m_Source[m_Current];
}

char Scanner::peekNext() const {
    if (m_Current + 1 >= m_Source.size()) {
        return '\0';
    }

    return m_Source[m_Current + 1];
}

void Scanner::addString(std::vector<Token>& tokens) {
    while(peek() != '"' && !isAtEnd()) {
        if(peek() == '\n') m_Line++;
        advance();
    }

    if(isAtEnd()) {
        AJ_Lox::error(m_Line, "Unterminated string");
    }

    // for the closing quote ".
    advance();

    // trim the surrounding quotes from the text.
    std::string text = m_Source.substr(m_Start + 1, m_Current - m_Start - 2);
    addToken(tokens, STRING, std::move(text));
}

void Scanner::addNumber(std::vector<Token>& tokens) {
    while(isDigit(peek())) advance();

    // get the fractional part if there is one. it must be followed by a digit.
    if(peek() == '.' && isDigit(peekNext())) {
        // consume the '.'
        advance();

        while(isDigit(peek())) advance();
    }

    std::string text = m_Source.substr(m_Start, m_Current - m_Start);

    // check if the number is out of bounds. Safe conversion is checked by std::from_chars.
    double value;
    auto [ptr, ec] = std::from_chars(text.data(), text.data() + text.size(), value);

    if(ec == std::errc::result_out_of_range) {
        AJ_Lox::error(m_Line, "Number out of range at: " + text);
        addToken(tokens, NUMBER, 0.0);
        return;
    }

    addToken(tokens, NUMBER, value);
}

void Scanner::addIdentifier(std::vector<Token>& tokens) {
    while(isAlphaNumeric(peek())) advance();

    std::string text = m_Source.substr(m_Start, m_Current - m_Start);

    TokenType type;

    if(m_Keywords.count(text)) {
        type = m_Keywords[text];
    } else {
        type = IDENTIFIER;
    }

    addToken(tokens, type);
}

bool Scanner::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool Scanner::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::isAlphaNumeric(char c) const {
    return isDigit(c) || isAlpha(c);
}

template<LiteralType T>
void Scanner::addToken(std::vector<Token>& tokens, TokenType type, T&& literal) {
    // create the token with the lexeme and literal value
    std::string text = m_Source.substr(m_Start, m_Current - m_Start);
    Token token(text, type, m_Line);
    token.m_Literal = literal;

    tokens.emplace_back(std::move(token));
}

void Scanner::addToken(std::vector<Token>& tokens, TokenType type) {
    // create the token with the lexeme only
    std::string text = m_Source.substr(m_Start, m_Current - m_Start);
    Token token(text, type, m_Line);

    tokens.emplace_back(std::move(token));
}
