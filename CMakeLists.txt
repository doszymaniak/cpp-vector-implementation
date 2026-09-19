cmake_minimum_required(VERSION 3.20)

project(cpp_vector_implementation LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
      DOWNLOAD_EXTRACT_TIMESTAMP TRUE)

set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(myvector_tests tests.cpp)

target_compile_features(myvector_tests PRIVATE cxx_std_23)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(myvector_tests PRIVATE -Wall -Wextra -Wpedantic)
elseif(MSVC)
  target_compile_options(myvector_tests PRIVATE /W4)
endif()

target_link_libraries(myvector_tests PRIVATE GTest::gtest_main)

include(CTest)
include(GoogleTest)

if(BUILD_TESTING)
  gtest_discover_tests(myvector_tests)
endif()
