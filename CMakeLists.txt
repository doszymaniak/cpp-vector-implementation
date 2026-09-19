cmake_minimum_required(VERSION 3.20)

project(cpp_vector_implementation LANGUAGES CXX)

add_executable(myvector_tests tests.cpp)

target_compile_features(myvector_tests PRIVATE cxx_std_23)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(myvector_tests PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
elseif(MSVC)
    target_compile_options(myvector_tests PRIVATE /W4)
endif()

include(CTest)

if(BUILD_TESTING)
    add_test(NAME myvector_tests COMMAND myvector_tests)
endif()