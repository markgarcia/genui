#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "genui/markup/lexer.hpp"

#include <vector>
#include <utility>

using namespace genui::markup;


std::vector<token_variant> tokens_from_source(source_code& source_code) {
    lexer lexer(&source_code);

    std::vector<token_variant> tokens;
    token_variant token;

    while(lexer.lex(token)) {
        tokens.push_back(token);
    }

    return tokens;
}


TEST_CASE("Lexer produces proper token types.", "[lexer]") {
    SECTION("Source strings that produce individual tokens.") {
        std::vector<std::pair<token_type, const char*>> source_types = {
            { token_type::whitespace, " " },
            { token_type::whitespace, "\t" },
            { token_type::whitespace, "\r" },
            { token_type::whitespace, "\n" },
            { token_type::colon, ":" },
            { token_type::left_brace, "{" },
            { token_type::right_brace, "}" }
        };

        for(const auto& source_type : source_types) {
            source_code source(source_type.second);

            auto tokens = tokens_from_source(source);
            REQUIRE(tokens.size() == 1);

            auto token_base = get_token_base(tokens[0]);
            REQUIRE(token_base.type() == source_type.first);
        }
    }

    SECTION("Contiguous whitespace should be treated as a single token.") {
        const std::string_view whitespace_chars = " \t\r\n";

        for(int n = 1; n <= 2; ++n) {
            std::string test_source_string;
            for(int i = 0; i < n; ++i) test_source_string += whitespace_chars;

            std::sort(test_source_string.begin(), test_source_string.end());

            do {
                source_code source(test_source_string);

                auto tokens = tokens_from_source(source);
                REQUIRE(tokens.size() == 1);

                auto token_base = get_token_base(tokens[0]);
                REQUIRE(token_base.type() == token_type::whitespace);
            } while(std::next_permutation(test_source_string.begin(), test_source_string.end()));
        }
    }
    
    SECTION("Keywords.") {
        SECTION("def") {
            source_code source("def");
            auto tokens = tokens_from_source(source);

            REQUIRE(tokens.size() == 1);
            REQUIRE(std::get_if<keyword_def_token>(&tokens[0]) != nullptr);
        }

        SECTION("using") {
            source_code source("using");
            auto tokens = tokens_from_source(source);

            REQUIRE(tokens.size() == 1);
            REQUIRE(std::get_if<keyword_using_token>(&tokens[0]) != nullptr);
        }
    }
}