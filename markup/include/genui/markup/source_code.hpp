#ifndef GENUI_MARKUP_SOURCE_CODE_HPP
#define GENUI_MARKUP_SOURCE_CODE_HPP

#pragma once

#include <filesystem>
#include <string_view>


namespace genui::markup {


class source_code {
public:
    source_code(std::string code);
    source_code(std::string_view code);

    const std::string_view buffer() const { return std::string_view(m_code); }

private:
    std::string m_code;
};


class source_file : public source_code {
public:
    source_file(std::experimental::filesystem::path source_file);

private:
    std::experimental::filesystem::path m_source_file;
};


class source_code_location {
public:
    constexpr source_code_location(int row, int col, std::size_t buffer_offset);

    constexpr unsigned int row() const noexcept { return m_row; }
    constexpr unsigned int col() const noexcept { return m_col; }
    constexpr std::size_t buffer_offset() const noexcept { return m_buffer_offset; }

private:
    const unsigned int m_row;
    const unsigned int m_col;
    const std::size_t m_buffer_offset;
};


}

#endif