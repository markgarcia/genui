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
    bool lex_whitespace(std::string_view& source, token_variant& token);
    bool try_eat_keyword(std::string_view& source, std::string_view keyword, std::string_view& keyword_source);
    bool lex_keywords(std::string_view& source, token_variant& token);
    bool lex_identifier(std::string_view& source, token_variant& token);

    const char* get_end_data(const std::string_view& str) {
        return str.data() + str.size();
    }

    template<typename func_predicate_type>
    std::enable_if_t<
        std::is_function_v<func_predicate_type>,
        std::string_view
    >
    eat(std::string_view& source, func_predicate_type&& func_predicate) {
        const char* begin = source.data();
        const char* current_ptr = source.data();
        while(current_ptr != get_end_data(source)
              && std::forward<func_predicate_type>(func_predicate)(*current_ptr)) {
            ++current_ptr;
        }

        auto size = static_cast<std::size_t>(current_ptr - begin);
        source.remove_prefix(size)
        return std::string_view { begin, size };
    }

    std::string_view eat(std::string_view& source, bool (lexer::*func)(char)) {
        const char* begin = source.data();
        const char* current_ptr = source.data();
        while(current_ptr != get_end_data(source)
              && (this->*func)(*current_ptr)) {
            ++current_ptr;
        }

        auto size = static_cast<std::size_t>(current_ptr - begin);
        source.remove_prefix(size);
        return std::string_view { begin, size };
    }

    std::string_view eat(std::string_view& source, std::size_t size) {
        if(size > source.size()) size = source.size();

        const char* begin = source.data();
        source.remove_prefix(static_cast<std::size_t>(size));
        return std::string_view { begin, size };
    }
};


}


#endif