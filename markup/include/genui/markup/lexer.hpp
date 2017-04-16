#ifndef GENUI_MARKUP_LEXER_HPP
#define GENUI_MARKUP_LEXER_HPP

#pragma once

#include "source_code.hpp"
#include "token.hpp"

#include <functional>
#include <type_traits>


namespace genui::markup {


class lexer {
public:
    lexer(source_code* source_code) noexcept;
    lexer(lexer&& rhs) noexcept;
    lexer(const lexer&) = delete;
    lexer& operator=(lexer&& rhs) noexcept;

    bool lex(token_variant& token);

private:
    source_code* m_source_code;
    const char* m_next_token_ptr = nullptr;
    const char* m_end_ptr = nullptr;

    bool is_whitespace(char c);
    bool lex_whitespace(const char*& current_ptr, token_variant& token);
    bool try_eat_keyword(const char*& current_ptr, std::string_view keyword, std::string_view& source);
    bool lex_keywords(const char*& current_ptr, token_variant& token);
    bool lex_identifier(const char*& current_ptr, token_variant& token);

    template<typename func_predicate_type>
    std::enable_if_t<
        std::is_function_v<func_predicate_type>,
        std::string_view
    >
    eat(const char*& current_ptr, func_predicate_type&& func_predicate) {
        const char* begin = current_ptr;
        while(std::forward<func_predicate_type>(func_predicate)(*current_ptr)) ++current_ptr;

        return std::string_view { begin, static_cast<std::size_t>(current_ptr - begin) };
    }

    std::string_view eat(const char*& current_ptr, bool (lexer::*func)(char)) {
        const char* begin = current_ptr;
        while((this->*func)(*current_ptr)) ++current_ptr;

        return std::string_view { begin, static_cast<std::size_t>(current_ptr - begin) };
    }

    std::string_view eat(const char*& current_ptr, std::size_t size) {
        const char* begin = current_ptr;
        current_ptr += size;

        return std::string_view { begin, size };
    }
};


}


#endif