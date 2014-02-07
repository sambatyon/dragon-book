CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(dragonch03 CXX)

SET(BIN_OUTPUT_DIR ${dragonch03_BINARY_DIR}/bin)
SET(LIB_OUTPUT_DIR ${dragonch03_BINARY_DIR}/lib)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_OUTPUT_DIR})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_OUTPUT_DIR})

IF(MSVC)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS) # does not prompt warnings for printf, memcpy, etc
    ADD_DEFINITIONS(-D_WIN32_WINDOWS=_WIN32_WINNT_WIN7)
ENDIF()

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

ADD_EXECUTABLE(finiteautomata
               finite-automata.hh
               deterministic-finite-automata.hh
               non-deterministic-finite-automata.hh
               regex-lexer.hh
               regex-parser.hh
               finite-automata.cc
               deterministic-finite-automata.cc
               non-deterministic-finite-automata.cc
               regex-lexer.cc
               regex-parser.cc
               automata-main.cc)
ADD_EXECUTABLE(knuth-morris-pratt knuth-morris-pratt.cc)
