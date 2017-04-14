#ifndef GENUI_MARKUP_TOKEN_HPP
#define GENUI_MARKUP_TOKEN_HPP

#pragma once

#include <variant>
#include <optional>
#include <string_view>
#include <string>
#include <cstdint>
#include <cassert>


namespace genui::markup {


enum class token_type {
    whitespace,
    invalid,
    eof,

    colon,
    left_brace,
    right_brace,

    identifier,
    built_in_typename,
    string_literal,
    integer_constant,

    keyword_def,
    keyword_using,
};


class token {
protected:
    constexpr token(token_type type, std::string_view source) noexcept
        : m_type(type), m_source(source) {}

public:
    constexpr token_type type() const noexcept {
        return m_type;
    }

    constexpr std::string_view source() const noexcept {
        return m_source;
    }

private:
    const token_type m_type;
    const std::string_view m_source;
};


class whitespace_token : public token {
public:
    constexpr whitespace_token(std::string_view whitespace) noexcept
        : token(token_type::whitespace, whitespace) {}
    
    constexpr std::string_view whitespace() const noexcept { return source(); };
};


class invalid_token : public token {
public:
    constexpr invalid_token(std::string_view invalid_string) noexcept
        : token(token_type::invalid, invalid_string) {}

    constexpr std::string_view invalid_string() const noexcept { return source(); }
};


class eof_token : public token {
public:
    constexpr eof_token(std::string_view eof) noexcept
        : token(token_type::eof, eof) {}
};


class identifier_token : public token {
public:
    constexpr identifier_token(std::string_view identifier) noexcept
        : token(token_type::identifier, identifier) {}

    constexpr std::string_view identifier() const noexcept { return source(); }
};


class string_literal_token : public token {
public:
    constexpr string_literal_token(std::string_view string) noexcept
        : token(token_type::string_literal, string) {}

    constexpr std::string_view value() const noexcept { return source(); }
};


class integer_constant_token : public token {
public:
    constexpr integer_constant_token(std::uint64_t value, std::string_view source) noexcept
        : token(token_type::integer_constant, source), m_value(value) {}

    constexpr std::uint64_t value() const noexcept { return m_value; }

private:
    const std::uint64_t m_value;
};


class colon_token : public token {
    constexpr colon_token(std::string_view colon) noexcept
        : token(token_type::colon, colon) {}
};


class brace_token : public token {
public:
    using token::token;
};


class left_brace_token : public brace_token {
public:
    constexpr left_brace_token(std::string_view left_brace) noexcept
        : brace_token(token_type::left_brace, left_brace) {}
};


class right_brace_token : public brace_token {
public:
    constexpr right_brace_token(std::string_view right_brace) noexcept
        : brace_token(token_type::right_brace, right_brace) {}
};


class keyword_token : public token {
public:
    using token::token;
};


template<typename derived_t>
class keyword_token_base : public keyword_token {
public:
    using keyword_token::keyword_token;

    constexpr std::string_view canonical_keyword() const { return derived_t::canonical_keyword; }
};


class keyword_def_token : public keyword_token_base<keyword_def_token> {
public:
    static constexpr const char* canonical_keyword = "def";
    
    constexpr keyword_def_token(std::string_view def_keyword) noexcept
        : keyword_token_base(token_type::keyword_def, def_keyword) {}
};


class keyword_using_token : public keyword_token_base<keyword_using_token> {
public:
    static constexpr const char* canonical_keyword = "using";

    constexpr keyword_using_token(std::string_view using_keyword) noexcept
        : keyword_token_base(token_type::keyword_using, using_keyword) {}
};


}

#endif