#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <variant>


namespace genui::markup {


class object;


class object_property {
public:
    std::string_view name() const noexcept {
        return m_name;
    }
    
    template<typename T>
    bool is_of_type() const noexcept {
        return std::get_if<T>(&m_value) != nullptr;
    }

    template<>
    bool is_of_type<object>() const noexcept {
        return std::get_if<object&>(&m_value) != nullptr;
    }

    template<typename T>
    const T& value() const {
        return std::get<T>(m_value);
    }

    template<>
    const object& value() const {
        return std::get<object&>(m_value);
    }

private:
    std::string m_name;

    std::variant<
        object&,
        long long,
        std::string
    > m_value;
};


class event {
public:

};


class object_type {
public:
    object_type(std::string canonical_name)
        : m_canonical_name(std::move(canonical_name)) {}

    std::string_view canonical_name() const noexcept { return m_canonical_name; }

private:
    std::string m_canonical_name;
};


class object {
public:
    object(object_type object_type)
        : m_object_type(std::move(object_type)) {}

    const object_type& type() const noexcept { return m_object_type; }

    const std::vector<object_property>& properties() const noexcept { return m_properties; }
    const std::vector<event>& events() const noexcept { return m_events; }

private:
    object_type m_object_type;
    std::vector<object_property> m_properties;
    std::vector<event> m_events;
};


}
