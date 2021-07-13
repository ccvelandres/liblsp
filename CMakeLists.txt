cmake_minimum_required(VERSION 3.13)

SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
SET(CMAKE_INSTALL_RPATH "/usr/local/lib")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
SET(CMAKE_INSTALL_RPATH "/usr/local/lib")

# Project's name (get directory name)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_NAME})
project(${PROJECT_NAME})
set(PROJECT_EXE ${PROJECT_NAME})

# Project Folder 
set(CMAKE_PROJECT_DIR ${CMAKE_SOURCE_DIR})

add_compile_options(-Wno-psabi -Wno-unused-variable -g -gdwarf-2)
add_link_options(-rdynamic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

set (LSP_SOURCES
${CMAKE_SOURCE_DIR}/src/lsp_iflist.c
${CMAKE_SOURCE_DIR}/src/lsp_buffer.c
${CMAKE_SOURCE_DIR}/src/lsp_conf.c
${CMAKE_SOURCE_DIR}/src/lsp_conn.c
${CMAKE_SOURCE_DIR}/src/lsp_port.c
${CMAKE_SOURCE_DIR}/src/lsp_socket.c
${CMAKE_SOURCE_DIR}/src/lsp_routing.c
${CMAKE_SOURCE_DIR}/src/port/generic/lsp_log.c
${CMAKE_SOURCE_DIR}/src/arch/posix/lsp_queue.c
${CMAKE_SOURCE_DIR}/src/arch/posix/lsp_memory.c
${CMAKE_SOURCE_DIR}/src/arch/posix/lsp_mutex.c
${CMAKE_SOURCE_DIR}/src/arch/posix/lsp_thread.c
${CMAKE_SOURCE_DIR}/src/arch/posix/lsp_time.c
)

set (LSP_INCLUDE_DIRS
${CMAKE_SOURCE_DIR}/src/include
${CMAKE_SOURCE_DIR}/src/include/arch
)

add_library(lsp STATIC ${LSP_SOURCES})
target_compile_definitions(lsp PUBLIC LSP_POSIX)
target_include_directories(lsp PUBLIC ${LSP_INCLUDE_DIRS})
target_include_directories(lsp PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(lsp PUBLIC pthread)

add_executable(${PROJECT_EXE} ${CMAKE_SOURCE_DIR}/tests/main.c)
target_link_libraries(${PROJECT_EXE} PUBLIC lsp)