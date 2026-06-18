cmake_minimum_required(VERSION 3.16)
project(pypilot_syslib LANGUAGES CXX)

option(PYPILOT_SYSLIB_BUILD_TESTS "Build tests" ON)
option(PYPILOT_SYSLIB_BUILD_EXAMPLES "Build examples" ON)

add_library(pypilot_syslib INTERFACE)
target_include_directories(pypilot_syslib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_features(pypilot_syslib INTERFACE cxx_std_11)

if(PYPILOT_SYSLIB_BUILD_EXAMPLES)
  add_executable(logging_example examples/linux/logging_example.cpp)
  target_link_libraries(logging_example PRIVATE pypilot_syslib)
endif()

if(PYPILOT_SYSLIB_BUILD_TESTS)
  enable_testing()
  foreach(test_name test_logging test_logging_sinks)
    add_executable(${test_name} tests/${test_name}.cpp)
    target_link_libraries(${test_name} PRIVATE pypilot_syslib)
    add_test(NAME ${test_name} COMMAND ${test_name})
  endforeach()
endif()
