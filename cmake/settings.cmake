# 基本设置
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 基本路径设置
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(LOG_DIR ${CMAKE_SOURCE_DIR}/log)
set(CPP_CHECK_DIR ${LOG_DIR}/cppcheck)

include(${CMAKE_SOURCE_DIR}/cmake/library.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/tools.cmake)

add_subdirectory(src)
add_subdirectory(test)

# CXX相关设置
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

option(WERROR "if -Werror" ON)
if (WERROR)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif ()

# 依赖
find_program(CPP_CHECK "cppcheck")
