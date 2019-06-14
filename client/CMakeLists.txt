cmake_minimum_required(VERSION 3.13)
set(PROJECT_NAME "WebClient")
set(CMAKE_CXX_STANDARD 17)
project(${PROJECT_NAME})

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)
if (APPLE)
    link_directories("/usr/local/opt/llvm/lib")
endif ()

add_executable(${PROJECT_NAME} client.cpp)

target_link_libraries(${PROJECT_NAME} $<$<PLATFORM_ID:Darwin>:c++fs>)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
