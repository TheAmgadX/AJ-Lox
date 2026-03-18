#pragma  once

#include "token.h"
#include <unordered_map>
#include <vector>
#include <concepts>
#include <string>
#include <type_traits>

template<typename T>
concept LiteralType =
    std::same_as<std::decay_t<T>, double> ||
    std::same_as<std::decay_t<T>, std::string> ||
    std::same_as<std::decay_t<T>, bool>;

class Scanner {
public:
    Scanner(std::string& source) : m_Source(source) {
        m_Line = 1;
        m_Current = 0;
        m_Start = 0;

        initKeywords();
    }

    void scanTokens(std::vector<Token>& tokens);
private:
    std::string& m_Source;
    int m_Line;
    int m_Current;
    int m_Start;

    std::unordered_map<std::string, TokenType> m_Keywords;

    /// @brief Initializes the keyword map with the language's keywords.
    void initKeywords();

    bool isAtEnd() const;

    void scanToken(std::vector<Token>& tokens);

    char advance();

    bool match(char expected);

    char peek() const;

    char peekNext() const;

    void addString(std::vector<Token>& tokens);

    void addNumber(std::vector<Token>& tokens);

    void addIdentifier(std::vector<Token>& tokens);

    bool isDigit(char c) const;

    /// @brief Returns true if the character is a letter or underscore.
    bool isAlpha(char c) const;

    /// @brief Returns true if the character is a number, letter, or underscore.
    bool isAlphaNumeric(char c) const;

    /// @brief Adds a token to the list with the given type and literal value.
    template<LiteralType T>
    void addToken(std::vector<Token>& tokens, TokenType type, T&& literal);

    /// @brief Adds a token to the list with the given type and nil literal value.
    void addToken(std::vector<Token>& tokens, TokenType type);
};
