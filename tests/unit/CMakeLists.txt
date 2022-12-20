# Copyright (c) 2022 Hartmut Kaiser
#
# SPDX-License-Identifier: BSL-1.0 Distributed under the Boost Software License,
# Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

macro(mditerator_add_test name)
  add_executable(${name} ${name}.cpp)
  add_test(${name} ${name})
  target_link_libraries(${name} mditerator GTest::GTest GTest::Main)
  set_target_properties(${name} PROPERTIES FOLDER "Tests")
endmacro()

mditerator_add_test(1d_mditerator)
mditerator_add_test(2d_mditerator)
