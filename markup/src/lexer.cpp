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

    if(m_next_token_ptr == m_end_ptr) {
        return false; 
    }

    const char* current_ptr = m_next_token_ptr;

    if(lex_whitespace(current_ptr, token)) {
        return true;
    }
    else if(*current_ptr == ':') {
        token.emplace<colon_token>(eat(current_ptr, 1));
        m_next_token_ptr = current_ptr;
        return true;
    }
    else if(*current_ptr == '{') {
        token.emplace<left_brace_token>(eat(current_ptr, 1));
        m_next_token_ptr = current_ptr;
        return true;
    }
    else if(*current_ptr == '}') {
        token.emplace<right_brace_token>(eat(current_ptr, 1));
        m_next_token_ptr = current_ptr;
        return true;
    }
    else if(lex_keywords(current_ptr, token)) {
        m_next_token_ptr = current_ptr;
        return true;
    }
    else {
        token.emplace<invalid_token>(std::string_view { current_ptr, 1 });
        ++current_ptr;
        m_next_token_ptr = current_ptr;
        return true;
    }
}


bool lexer::is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}


bool lexer::lex_whitespace(const char*& current_ptr, token_variant& token) {    
    if(!is_whitespace(*current_ptr)) return false;

    token.emplace<whitespace_token>(eat(current_ptr, &lexer::is_whitespace));
    m_next_token_ptr = current_ptr;

    return true;
}


bool lexer::try_eat_keyword(const char*& current_ptr, std::string_view keyword, std::string_view& source) {
    if(static_cast<std::size_t>(m_end_ptr - current_ptr) < keyword.size()) {
        return false;
    }

    std::string_view test_source { current_ptr, keyword.size() };
    if(test_source != keyword) {
        return false;
    }

    source = test_source;
    current_ptr += keyword.size();

    return true;
}


bool lexer::lex_keywords(const char*& current_ptr, token_variant& token) {
    std::string_view keyword_source;
    
    if(try_eat_keyword(current_ptr, "def", keyword_source)) {
        token.emplace<keyword_def_token>(keyword_source);
        return true;
    }
    else if(try_eat_keyword(current_ptr, "using", keyword_source)) {
        token.emplace<keyword_using_token>(keyword_source);
        return true;
    }

    return false;
}


bool lexer::lex_identifier(const char*& current_ptr, token_variant& token) {
    return false;
}


}
