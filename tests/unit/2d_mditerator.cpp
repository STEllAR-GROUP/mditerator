// Copyright (c) 2022 Hartmut Kaiser
//
// SPDX-License-Identifier: BSL-1.0
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <iostream>
#include <numeric>
#include <vector>

#include <experimental/mditerator.hpp>

#include "gtest/gtest.h"
#include "iterator_tests.hpp"

namespace {

TEST(mditerator_2d, trivial_iterator_test)
{
    using value_type = int;

    std::vector<value_type> data(99);
    std::iota(data.begin(), data.end(), 0);

    using mdspan_t = std::experimental::mdspan<value_type,
        std::experimental::extents<std::size_t,
            std::experimental::dynamic_extent,
            std::experimental::dynamic_extent>,
        std::experimental::layout_left>;

    mdspan_t view(data.data(), 11, 9);

    auto begin = mditerator_begin(view);
    auto end = mditerator_end(view);

    auto val =
        std::experimental::submdspan(view, 0, std::experimental::full_extent);

    trivial_iterator_test(begin, end, val);
}

}    // namespace
