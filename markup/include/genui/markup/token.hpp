#ifndef GENUI_MARKUP_TOKEN_HPP
#define GENUI_MARKUP_TOKEN_HPP

#pragma once

#include <variant>
#include <string_view>
#include <string>
#include <cstdint>
#include <cassert>


namespace genui::markup {


enum class token_type {
    empty_token,

    whitespace,
    invalid,

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


class token_base {
public:
    constexpr token_base() noexcept
        : m_type(token_type::empty_token) {}

    constexpr token_base(token_type type, std::string_view source) noexcept
        : m_type(type), m_source(source) {}

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


class whitespace_token : public token_base {
public:
    constexpr whitespace_token(std::string_view whitespace) noexcept
        : token_base(token_type::whitespace, whitespace) {}
};


class invalid_token : public token_base {
public:
    constexpr invalid_token(std::string_view source) noexcept
        : token_base(token_type::invalid, source) {}
};


class identifier_token : public token_base {
public:
    constexpr identifier_token(std::string_view identifier, std::string_view canonical_identifier) noexcept
        : token_base(token_type::identifier, identifier), m_canonical_identifier(canonical_identifier) {}

    constexpr std::string_view canonical_identifier() const noexcept { return m_canonical_identifier; }

private:
    const std::string_view m_canonical_identifier;
};


class string_literal_token : public token_base {
public:
    constexpr string_literal_token(std::string_view source, std::string_view value) noexcept
        : token_base(token_type::string_literal, source), m_value(value) {}

    constexpr std::string_view value() const noexcept { return m_value; }

private:
    const std::string_view m_value;
};


class integer_constant_token : public token_base {
public:
    constexpr integer_constant_token(std::string_view source, std::uint64_t value) noexcept
        : token_base(token_type::integer_constant, source), m_value(value) {}

    constexpr std::uint64_t value() const noexcept { return m_value; }

private:
    const std::uint64_t m_value;
};


class colon_token : public token_base {
public:
    constexpr colon_token(std::string_view colon) noexcept
        : token_base(token_type::colon, colon) {}
};


class brace_token : public token_base {
public:
    using token_base::token_base;
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


class keyword_token : public token_base {
public:
    using token_base::token_base;
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


template<template<typename...> typename T>
using apply_token_type_list_for = T<
    token_base,
    whitespace_token,
    invalid_token,
    colon_token,
    left_brace_token,
    right_brace_token,
    identifier_token,
    string_literal_token,
    integer_constant_token,
    keyword_def_token,
    keyword_using_token
>;


using token_variant = apply_token_type_list_for<std::variant>;


inline const token_base& get_token_base(const token_variant& token) {
    return std::visit([](const auto& token) -> const token_base& {
        return static_cast<const token_base&>(token);
    }, token);
}


}

#endif