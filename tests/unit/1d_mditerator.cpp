// Copyright (c) 2022 Hartmut Kaiser
//
// SPDX-License-Identifier: BSL-1.0
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <vector>

#include <experimental/mditerator.hpp>

#include "gtest/gtest.h"
#include "iterator_tests.hpp"

namespace {

TEST(mditerator_1d, trivial_iterator_test)
{
    using value_type = int;

    std::vector<value_type> data(100, 42);

    using mdspan_t = std::experimental::mdspan<value_type,
        std::experimental::extents<std::size_t,
            std::experimental::dynamic_extent>>;

    mdspan_t view(data.data(), data.size());

    auto begin = mditerator_begin(view);
    auto end = mditerator_end(view);

    trivial_iterator_test(begin, end, 42);
}

TEST(mditerator_1d, input_iterator_test)
{
    using value_type = int;

    std::vector<value_type> data(100);
    std::iota(data.begin(), data.end(), 42);

    using mdspan_t = std::experimental::mdspan<value_type,
        std::experimental::extents<std::size_t,
            std::experimental::dynamic_extent>>;

    mdspan_t view(data.data(), data.size());

    auto begin = mditerator_begin(view);

    input_iterator_test(begin, 42, 43);
    readable_iterator_test(begin, 42);
}

TEST(mditerator_1d, forward_iterator_test)
{
    using value_type = int;

    std::vector<value_type> data(100);
    std::iota(data.begin(), data.end(), 42);

    using mdspan_t = std::experimental::mdspan<value_type,
        std::experimental::extents<std::size_t,
            std::experimental::dynamic_extent>>;

    mdspan_t view(data.data(), data.size());

    auto begin = mditerator_begin(view);
    auto end = mditerator_end(view);

    forward_iterator_test(begin, 42, 43);
    forward_readable_iterator_test(begin, end, 42, 43);
}

TEST(mditerator_1d, bidirectional_iterator_test)
{
    using value_type = int;

    std::vector<value_type> data(100);
    std::iota(data.begin(), data.end(), 42);

    using mdspan_t = std::experimental::mdspan<value_type,
        std::experimental::extents<std::size_t,
            std::experimental::dynamic_extent>>;

    mdspan_t view(data.data(), data.size());

    auto begin = mditerator_begin(view);
    auto end = mditerator_end(view);

    bidirectional_iterator_test(begin, 42, 43);
    bidirectional_readable_iterator_test(begin, 42, 43);
}

TEST(mditerator_1d, random_access_iterator_test)
{
    using value_type = int;

    std::vector<value_type> data(100);
    std::iota(data.begin(), data.end(), 42);

    using mdspan_t = std::experimental::mdspan<value_type,
        std::experimental::extents<std::size_t,
            std::experimental::dynamic_extent>>;

    mdspan_t view(data.data(), data.size());

    auto begin = mditerator_begin(view);
    auto end = mditerator_end(view);

    random_access_iterator_test(begin, data.size(), data);
    random_access_readable_iterator_test(begin, data.size(), data);
}

TEST(mditerator_1d, writable_iterator_test)
{
    using value_type = int;

    std::vector<value_type> data(100);
    std::iota(data.begin(), data.end(), 42);

    using mdspan_t = std::experimental::mdspan<value_type,
        std::experimental::extents<std::size_t,
            std::experimental::dynamic_extent>>;

    mdspan_t view(data.data(), data.size());

    auto begin = mditerator_begin(view);
    auto end = mditerator_end(view);

    writable_iterator_test(begin, 42, 43);
}
}    // namespace
