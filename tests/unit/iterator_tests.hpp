//  Copyright David Abrahams 2001-2004.
//  Copyright (c) Jeremy Siek 2001-2003.
//  Copyright (c) Thomas Witt 2002.
//
// Copyright (c) 2022 Hartmut Kaiser
//
// SPDX-License-Identifier: BSL-1.0
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

///////////////////////////////////////////////////////////////////////////
// Tests whether type Iterator satisfies the requirements for a
// TrivialIterator.
// Preconditions: i != j, *i == val
template <typename Iterator, typename T>
void trivial_iterator_test(Iterator const i, Iterator const j, T val)
{
    EXPECT_TRUE(i == i);
    EXPECT_TRUE(j == j);
    EXPECT_TRUE(i != j);
    typename std::iterator_traits<Iterator>::value_type v = *i;

    EXPECT_TRUE(v == val);

    Iterator k = i;
    EXPECT_TRUE(k == k);
    EXPECT_TRUE(k == i);
    EXPECT_TRUE(k != j);
    EXPECT_TRUE(*k == val);
}

// Preconditions: *i == v1, *++i == v2
template <typename Iterator, typename T>
void input_iterator_test(Iterator i, T v1, T v2)
{
    Iterator i1(i);

    EXPECT_TRUE(i == i1);
    EXPECT_TRUE(!(i != i1));

    EXPECT_TRUE(*i1 == v1);
    EXPECT_TRUE(*i == v1);

    // we cannot test for equivalence of (void)++i & (void)i++
    // as i is only guaranteed to be single pass.
    EXPECT_TRUE(*i++ == v1);

    i1 = i;

    EXPECT_TRUE(i == i1);
    EXPECT_TRUE(!(i != i1));

    EXPECT_TRUE(*i1 == v2);
    EXPECT_TRUE(*i == v2);

    // i is dereferenceable, so it must be incrementable.
    ++i;

    // how to test for operator-> ?
}

namespace detail {
template <typename T, typename Enable = void>
struct is_incrementable : std::false_type
{};

template <typename T>
struct is_incrementable<T, typename std::void_t<decltype(++std::declval<T&>())>>
  : std::true_type
{};

template <typename T, typename Enable = void>
struct is_postfix_incrementable : std::false_type
{};

template <typename T>
struct is_postfix_incrementable<T,
    typename std::void_t<decltype(std::declval<T&>()++)>> : std::true_type
{};
}    // namespace detail

// Preconditions: *i == v
// Do separate tests for *i++ so we can treat, e.g., smart pointers,
// as readable and/or writable iterators.
template <typename Iterator, typename T>
void readable_iterator_traversal_test(Iterator i1, T v, std::true_type)
{
    T v2(*i1++);
    EXPECT_TRUE(v == v2);
}

template <typename Iterator, typename T>
void readable_iterator_traversal_test(const Iterator, T, std::false_type)
{}

template <typename Iterator, typename T>
void readable_iterator_test(Iterator const i1, T v)
{
    typedef typename std::iterator_traits<Iterator>::reference ref_t;

    Iterator i2(i1);    // Copy Constructible
    ref_t r1 = *i1;
    ref_t r2 = *i2;
    T v1 = r1;
    T v2 = r2;
    EXPECT_TRUE(v1 == v);
    EXPECT_TRUE(v2 == v);

    readable_iterator_traversal_test(i1, v,
        typename std::integral_constant<bool,
            detail::is_postfix_incrementable<Iterator>::value>::type());
}

template <typename Iterator, typename T>
void forward_iterator_test(Iterator i, T v1, T v2)
{
    input_iterator_test(i, v1, v2);

    Iterator i1 = i, i2 = i;

    EXPECT_TRUE(i == i1++);
    EXPECT_TRUE(i != ++i2);

    trivial_iterator_test(i, i1, v1);
    trivial_iterator_test(i, i2, v1);

    ++i;
    EXPECT_TRUE(i == i1);
    EXPECT_TRUE(i == i2);
    ++i1;
    ++i2;

    trivial_iterator_test(i, i1, v2);
    trivial_iterator_test(i, i2, v2);

    //lvalue_test<(std::is_pointer<Iterator>::value)>::check_(i);
}

template <typename Iterator, typename T>
void forward_readable_iterator_test(Iterator i, Iterator j, T val1, T val2)
{
    Iterator i2;
    Iterator i3(i);
    i2 = i;
    EXPECT_TRUE(i2 == i3);
    EXPECT_TRUE(i != j);
    EXPECT_TRUE(i2 != j);
    readable_iterator_test(i, val1);
    readable_iterator_test(i2, val1);
    readable_iterator_test(i3, val1);

    EXPECT_TRUE(i == i2++);
    EXPECT_TRUE(i != ++i3);

    readable_iterator_test(i2, val2);
    readable_iterator_test(i3, val2);

    readable_iterator_test(i, val1);
}

// Preconditions: *i == v1, *++i == v2
template <typename Iterator, typename T>
void bidirectional_iterator_test(Iterator i, T v1, T v2)
{
    forward_iterator_test(i, v1, v2);
    ++i;

    Iterator i1 = i, i2 = i;

    EXPECT_TRUE(i == i1--);
    EXPECT_TRUE(i != --i2);

    trivial_iterator_test(i, i1, v2);
    trivial_iterator_test(i, i2, v2);

    --i;
    EXPECT_TRUE(i == i1);
    EXPECT_TRUE(i == i2);
    ++i1;
    ++i2;

    trivial_iterator_test(i, i1, v1);
    trivial_iterator_test(i, i2, v1);
}

// bidirectional
// Preconditions: *i == v1, *++i == v2
template <typename Iterator, typename T>
void bidirectional_readable_iterator_test(Iterator i, T v1, T v2)
{
    Iterator j(i);
    ++j;
    forward_readable_iterator_test(i, j, v1, v2);
    ++i;

    Iterator i1 = i, i2 = i;

    EXPECT_TRUE(i == i1--);
    EXPECT_TRUE(i != --i2);

    readable_iterator_test(i, v2);
    readable_iterator_test(i1, v1);
    readable_iterator_test(i2, v1);

    --i;
    EXPECT_TRUE(i == i1);
    EXPECT_TRUE(i == i2);
    ++i1;
    ++i2;

    readable_iterator_test(i, v1);
    readable_iterator_test(i1, v2);
    readable_iterator_test(i2, v2);
}

namespace detail {
template <typename T>
struct identity
{
    typedef T type;
};

// implementation originally suggested by C. Green in
// http://lists.boost.org/MailArchives/boost/msg00886.php

// The use of identity creates a non-deduced form, so that the
// explicit template argument must be supplied
template <typename T>
inline T implicit_cast(typename identity<T>::type x)
{
    return x;
}
}    // namespace detail

// Preconditions: [i,i+N) is a valid range, N >= 2
template <typename Iterator, typename TrueVals>
void random_access_iterator_test(Iterator i, int N, TrueVals vals)
{
    ASSERT_TRUE(N >= 2);
    bidirectional_iterator_test(i, vals[0], vals[1]);
    Iterator const j = i;
    int c;

    using value_type = typename std::iterator_traits<Iterator>::value_type;

    for (c = 0; c < N - 1; ++c)
    {
        EXPECT_TRUE(i == j + c);
        EXPECT_TRUE(*i == vals[c]);
        EXPECT_TRUE(*i == detail::implicit_cast<value_type>(j[c]));
        EXPECT_TRUE(*i == *(j + c));
        EXPECT_TRUE(*i == *(c + j));
        ++i;
        EXPECT_TRUE(j < i);
        EXPECT_TRUE(j <= i);
        EXPECT_TRUE(j <= i);
        EXPECT_TRUE(j < i);
    }

    Iterator k = j + N - 1;
    for (c = 0; c < N - 1; ++c)
    {
        EXPECT_TRUE(i == k - c);
        EXPECT_TRUE(*i == vals[N - 1 - c]);
        EXPECT_TRUE(*i == detail::implicit_cast<value_type>(j[N - 1 - c]));
        Iterator q = k - c;
        EXPECT_TRUE(*i == *q);
        EXPECT_TRUE(j < i);
        EXPECT_TRUE(j <= i);
        EXPECT_TRUE(j <= i);
        EXPECT_TRUE(j < i);
        --i;
    }
}

// random access
// Preconditions: [i,i+N) is a valid range
template <typename Iterator, typename TrueVals>
void random_access_readable_iterator_test(Iterator i, int N, TrueVals vals)
{
    ASSERT_TRUE(N >= 2);
    bidirectional_readable_iterator_test(i, vals[0], vals[1]);
    const Iterator j = i;
    int c;

    for (c = 0; c < N - 1; ++c)
    {
        EXPECT_TRUE(i == j + c);
        EXPECT_TRUE(*i == vals[c]);
        typename std::iterator_traits<Iterator>::value_type x = j[c];
        EXPECT_TRUE(*i == x);
        EXPECT_TRUE(*i == *(j + c));
        EXPECT_TRUE(*i == *(c + j));
        ++i;
        EXPECT_TRUE(j < i);
        EXPECT_TRUE(j <= i);
        EXPECT_TRUE(j <= i);
        EXPECT_TRUE(j < i);
    }

    Iterator k = j + N - 1;
    for (c = 0; c < N - 1; ++c)
    {
        EXPECT_TRUE(i == k - c);
        EXPECT_TRUE(*i == vals[N - 1 - c]);
        typename std::iterator_traits<Iterator>::value_type x = j[N - 1 - c];
        EXPECT_TRUE(*i == x);
        Iterator q = k - c;
        EXPECT_TRUE(*i == *q);
        EXPECT_TRUE(j < i);
        EXPECT_TRUE(j <= i);
        EXPECT_TRUE(j <= i);
        EXPECT_TRUE(j < i);
        --i;
    }
}

template <typename Iterator, typename T>
void writable_iterator_test(Iterator i, T v, T v2)
{
    Iterator i2(i);    // Copy Constructible
    *i2 = v;
    EXPECT_TRUE(*i2 == v);

    Iterator i3(i);    // Copy Constructible
    ++i3;              // we just wrote into that position
    *i3 = v;
    EXPECT_TRUE(*i3 == v);

    Iterator x(i++);
    EXPECT_TRUE(*x == v);
}
