cmake_minimum_required(VERSION 3.6)
project(genui)

set_property(DIRECTORY PROPERTY CXX_STANDARD 17)
set_property(DIRECTORY PROPERTY CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /std:c++latest /permissive- /await)

    if(DEFINED VCPKG_TOOLCHAIN)
        # Note: set CMAKE_TOOLCHAIN_FILE to <whatever>/vcpkg/scripts/buildsystems/vcpkg.cmake
        include_directories(${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include)
    endif()
elseif(GCC OR CLANG)
    add_compile_options(-Wall -Wpedantic)
endif()

add_subdirectory(markup)
add_subdirectory(platforms)