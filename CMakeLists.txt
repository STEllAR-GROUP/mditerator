# Copyright (c) 2022 Hartmut Kaiser
#
# SPDX-License-Identifier: BSL-1.0 Distributed under the Boost Software License,
# Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.18)
project(
  mditerator
  VERSION 0.0.1
  LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ##############################################################################

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# ##############################################################################

option(MDITERATOR_ENABLE_TESTS "Enable tests." Off)
option(MDITERATOR_ENABLE_EXAMPLES "Build examples." Off)

# Option to override which C++ standard to use
set(MDITERATOR_CXX_STANDARD
    DETECT
    CACHE STRING "Override the default CXX_STANDARD to compile with.")
set_property(CACHE MDITERATOR_CXX_STANDARD PROPERTY STRINGS DETECT 14 17 20 23)

# ##############################################################################

# Decide on the standard to use
if(MDITERATOR_CXX_STANDARD STREQUAL "17")
  if("cxx_std_17" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    message(STATUS "Using C++17 standard")
    set(CMAKE_CXX_STANDARD 17)
  else()
    message(
      FATAL_ERROR
        "Requested MDITERATOR_CXX_STANDARD \"17\" not supported by provided C++ compiler"
    )
  endif()
elseif(MDITERATOR_CXX_STANDARD STREQUAL "14")
  if("cxx_std_14" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    message(STATUS "Using C++14 standard")
    set(CMAKE_CXX_STANDARD 14)
  else()
    message(
      FATAL_ERROR
        "Requested MDITERATOR_CXX_STANDARD \"14\" not supported by provided C++ compiler"
    )
  endif()
elseif(MDITERATOR_CXX_STANDARD STREQUAL "20")
  if("cxx_std_20" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    message(STATUS "Using C++20 standard")
    set(CMAKE_CXX_STANDARD 20)
  else()
    message(
      FATAL_ERROR
        "Requested MDITERATOR_CXX_STANDARD \"20\" not supported by provided C++ compiler"
    )
  endif()
elseif(MDITERATOR_CXX_STANDARD STREQUAL "23")
  if("cxx_std_23" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    message(STATUS "Using C++23 standard")
    set(CMAKE_CXX_STANDARD 23)
  else()
    message(
      FATAL_ERROR
        "Requested MDITERATOR_CXX_STANDARD \"23\" not supported by provided C++ compiler"
    )
  endif()
else()
  if("cxx_std_23" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    set(CMAKE_CXX_STANDARD 23)
    message(STATUS "Detected support for C++23 standard")
  elseif("cxx_std_20" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    set(CMAKE_CXX_STANDARD 20)
    message(STATUS "Detected support for C++20 standard")
  elseif("cxx_std_17" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    set(CMAKE_CXX_STANDARD 17)
    message(STATUS "Detected support for C++17 standard")
  elseif("cxx_std_14" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
    set(CMAKE_CXX_STANDARD 14)
    message(STATUS "Detected support for C++14 standard")
  else()
    message(FATAL_ERROR "Cannot detect CXX_STANDARD of C++14 or newer.")
  endif()
endif()

# ##############################################################################

find_package(mdspan QUIET)
if(NOT mdspan_FOUND)
  message(STATUS "No installed mdspan found, fetching from Github")
  include(FetchContent)
  FetchContent_Declare(
    mdspan
    GIT_REPOSITORY https://github.com/kokkos/mdspan.git
    GIT_TAG stable)
  FetchContent_GetProperties(mdspan)
  if(NOT mdspan_POPULATED)
    FetchContent_Populate(mdspan)
    add_subdirectory(${mdspan_SOURCE_DIR} ${mdspan_BINARY_DIR} EXCLUDE_FROM_ALL)
  endif()
endif()

# ##############################################################################

configure_file(
  cmake/templates/mditerator_config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/include/experimental/mditerator_config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include/experimental)
message(
  STATUS
    "Build include directory: ${CMAKE_CURRENT_BINARY_DIR}/include/experimental")

# ##############################################################################

add_library(mditerator INTERFACE)

target_link_libraries(mditerator INTERFACE std::mdspan)
target_include_directories(
  mditerator INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                       $<INSTALL_INTERFACE:include>)

# ##############################################################################

find_package(HPX 1.8 REQUIRED)
target_link_libraries(mditerator INTERFACE HPX::hpx)
if(MSVC)
  target_compile_definitions(mditerator INTERFACE NOMINMAX _CRT_SECURE_NO_WARNINGS)
endif()
message(STATUS "HPX version: " ${HPX_VERSION_STRING})

# ##############################################################################

install(
  TARGETS mditerator
  EXPORT mditeratorTargets
  INCLUDES
  DESTINATION include)

install(
  EXPORT mditeratorTargets
  FILE mditeratorTargets.cmake
  NAMESPACE std::
  DESTINATION cmake)

export(
  TARGETS mditerator
  NAMESPACE std::
  FILE lib/cmake/mditerator/mditeratorTargets.cmake)

install(DIRECTORY include/experimental DESTINATION include)
install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/include/experimental/mditerator_config.h
  DESTINATION include/experimental)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  cmake/templates/mditeratorConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/lib/cmake/mditerator/mditeratorConfig.cmake
  INSTALL_DESTINATION cmake)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/lib/cmake/mditerator/mditeratorConfigVersion.cmake
  COMPATIBILITY SameMajorVersion)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/lib/cmake/mditerator/mditeratorConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/lib/cmake/mditerator/mditeratorConfigVersion.cmake
  DESTINATION cmake/mditerator)

# ##############################################################################

if(MDITERATOR_ENABLE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

if(MDITERATOR_ENABLE_EXAMPLES)
  add_subdirectory(examples)
endif()
