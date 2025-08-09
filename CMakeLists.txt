cmake_minimum_required(VERSION 3.10)
project(latex_to_svg C)

set(CMAKE_C_STANDARD 99)

include_directories(include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

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

# WASM build
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s WASM=1 -s EXPORTED_FUNCTIONS='[\"_main\", \"_malloc\", \"_free\", \"_latex_to_svg\"]' -s EXPORTED_RUNTIME_METHODS='[\"ccall\", \"cwrap\", \"setValue\", \"getValue\", \"writeAsciiToMemory\", \"stringToUTF8\", \"UTF8ToString\"]'")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s ALLOW_MEMORY_GROWTH=1")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s MODULARIZE=1 -s EXPORT_NAME='LatexToSvg'")
    
    set(SKIP_FLEX_BISON ON)
    set(WEB_SOURCES src/web_api.c)
endif()

# find flex/bison if not building for Emscripten
if(NOT SKIP_FLEX_BISON)
    find_package(BISON REQUIRED)
    find_package(FLEX REQUIRED)
    
    BISON_TARGET(Parser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)
    FLEX_TARGET(Lexer src/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
    ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)
    
    set(GENERATED_SOURCES
        ${BISON_Parser_OUTPUTS}
        ${FLEX_Lexer_OUTPUTS}
    )
else()
    # wasm: pre-generate the parser files
    set(GENERATED_SOURCES
        src/parser.c
        src/lexer.c
    )
endif()

set(SOURCES
    src/main.c
    src/ast.c
    src/svg_letters.c
    ${GENERATED_SOURCES}
    ${WEB_SOURCES}
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