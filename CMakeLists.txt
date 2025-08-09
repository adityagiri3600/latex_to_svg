cmake_minimum_required(VERSION 3.10)
project(latex_to_svg C)

set(CMAKE_C_STANDARD 99)

if(WIN32)
    set(ADDITIONAL_PATHS
        "C:/msys64/usr/bin"
        "C:/msys64/mingw64/bin"
        "C:/MinGW/bin"
        "C:/Program Files/Git/usr/bin"
        "$ENV{MSYS2_ROOT}/usr/bin"
        "$ENV{MINGW_ROOT}/bin"
    )
    
    find_program(FLEX_EXECUTABLE
        NAMES flex win_flex
        PATHS ${ADDITIONAL_PATHS}
        DOC "Path to flex executable"
    )
    
    find_program(BISON_EXECUTABLE
        NAMES bison win_bison
        PATHS ${ADDITIONAL_PATHS}
        DOC "Path to bison executable"
    )
    
    if(FLEX_EXECUTABLE)
        set(FLEX_EXECUTABLE ${FLEX_EXECUTABLE} CACHE FILEPATH "Path to flex")
    endif()
    
    if(BISON_EXECUTABLE)
        set(BISON_EXECUTABLE ${BISON_EXECUTABLE} CACHE FILEPATH "Path to bison")
    endif()
endif()

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

include_directories(include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

message(STATUS "Generator: ${CMAKE_GENERATOR}")
message(STATUS "C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Flex: ${FLEX_EXECUTABLE}")
message(STATUS "Bison: ${BISON_EXECUTABLE}")


BISON_TARGET(Parser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)

FLEX_TARGET(Lexer src/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)

ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

set(SOURCES
    src/main.c
    src/ast.c
    src/svg_letters.c
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Lexer_OUTPUTS}
)

add_executable(l2s ${SOURCES})

if(MSVC)
    target_compile_definitions(l2s PRIVATE
        YY_NO_UNISTD_H
        isatty=_isatty
        fileno=_fileno
    )
endif()

if(MSVC)
    target_compile_options(l2s PRIVATE /W3)
else()
    target_compile_options(l2s PRIVATE -Wall -Wextra)
endif()