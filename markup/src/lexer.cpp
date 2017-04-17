#include "genui/markup/lexer.hpp"


namespace genui::markup {


lexer::lexer(source_code* source_code) noexcept
    : m_source_code(source_code) {
    m_next_token_ptr = m_source_code->buffer().data();
    m_end_ptr = m_source_code->buffer().data() + m_source_code->buffer().size();
}


lexer::lexer(lexer&& rhs) noexcept
    : m_source_code(rhs.m_source_code) {
    rhs.m_source_code = nullptr;
    rhs.m_next_token_ptr = nullptr;

    m_next_token_ptr = m_source_code->buffer().data();
    m_end_ptr = m_source_code->buffer().data() + m_source_code->buffer().size();
}


lexer& lexer::operator=(lexer&& rhs) noexcept {
    m_source_code = rhs.m_source_code;
    rhs.m_source_code = nullptr;
    rhs.m_next_token_ptr = nullptr;

    m_next_token_ptr = m_source_code->buffer().data();
    m_end_ptr = m_source_code->buffer().data() + m_source_code->buffer().size();

    return *this;
}


bool lexer::lex(token_variant& token) {
    assert(m_next_token_ptr <= m_end_ptr);

    std::string_view source(m_next_token_ptr, static_cast<std::size_t>(m_end_ptr - m_next_token_ptr));
    if(source.empty()) {
        return false; 
    }

    if(lex_whitespace(source, token)) {
        m_next_token_ptr = source.data();
        return true;
    }
    else if(source.front() == ':') {
        token.emplace<colon_token>(eat(source, 1));
        m_next_token_ptr = source.data();
        return true;
    }
    else if(source.front() == '{') {
        token.emplace<left_brace_token>(eat(source, 1));
        m_next_token_ptr = source.data();
        return true;
    }
    else if(source.front() == '}') {
        token.emplace<right_brace_token>(eat(source, 1));
        m_next_token_ptr = source.data();
        return true;
    }
    else if(lex_keywords(source, token)) {
        m_next_token_ptr = source.data();
        return true;
    }
    else if(lex_identifier(source, token)) {
        m_next_token_ptr = source.data();
        return true;
    }
    else {
        token.emplace<invalid_token>(std::string_view { source.data(), 1 });
        source.remove_prefix(1);
        m_next_token_ptr = source.data();
        return true;
    }
}


bool lexer::is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}


bool lexer::lex_whitespace(std::string_view& source, token_variant& token) {
    if(!is_whitespace(source.front())) return false;

    token.emplace<whitespace_token>(eat(source, &lexer::is_whitespace));

    return true;
}


bool lexer::try_eat_keyword(std::string_view& source, std::string_view keyword, std::string_view& keyword_source) {
    if(source.size() < keyword.size()) {
        return false;
    }

    if(source.substr(0, keyword.size()) != keyword) {
        return false;
    }

    keyword_source = source.substr(0, keyword.size());
    source.remove_prefix(keyword.size());

    return true;
}


bool lexer::lex_keywords(std::string_view& source, token_variant& token) {
    std::string_view keyword_source;
    
    if(try_eat_keyword(source, "def", keyword_source)) {
        token.emplace<keyword_def_token>(keyword_source);
        return true;
    }
    else if(try_eat_keyword(source, "using", keyword_source)) {
        token.emplace<keyword_using_token>(keyword_source);
        return true;
    }

    return false;
}


bool lexer::lex_identifier(std::string_view& source, token_variant& token) {
    return false;
}


}
