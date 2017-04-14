#ifndef GENUI_MARKUP_LEXER_HPP
#define GENUI_MARKUP_LEXER_HPP

#pragma once

#include "source_code.hpp"
#include "token.hpp"

#include <gsl/gsl>


namespace genui::markup {


class lexer {
public:
    lexer(gsl::not_null<source_code*> source_code);
    lexer(const lexer&) = delete;

private:
    gsl::not_null<source_code*> m_source_code;
};


}


#endif