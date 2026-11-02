#ifndef BOOST_UUID_DETAIL_TYPE_LIST_HPP_INCLUDED
#define BOOST_UUID_DETAIL_TYPE_LIST_HPP_INCLUDED

// Copyright 2026 Andrey Semashev
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/detail/config.hpp>

namespace boost {
namespace uuids {
namespace detail {

//! A list of arbitrary types
template< typename... Elems >
struct type_list;

//! Returns an element of the type list at the gimen index
template< typename List, unsigned int Index >
struct elem_at;

template< template< typename... > class List, typename Head, typename... Tail, unsigned int Index >
struct elem_at< List< Head, Tail... >, Index >
{
    using type = typename detail::elem_at< List< Tail... >, Index - 1u >::type;
};

template< template< typename... > class List, typename Head, typename... Tail >
struct elem_at< List< Head, Tail... >, 0u >
{
    using type = Head;
};

//! Adds an element at the beginning of the list
template< typename List, typename Elem >
struct push_front;

template< template< typename... > class List, typename... Tail, typename Elem >
struct push_front< List< Tail... >, Elem >
{
    using type = List< Elem, Tail... >;
};

/*!
 * Takes a list of types and a binary predicate and produces an ordered list of types, where the predicate returns `true`
 * for each pair of adjacent elements. A list of one element is returned unchanged.
 */
template< typename List, template< typename, typename > class Order >
struct sort;

template< typename List, typename Elem, template< typename, typename > class Order, bool AsFirst >
struct sort_insert;

template< template< typename... > class List, typename Elem, template< typename, typename > class Order, bool AsFirst >
struct sort_insert< List< >, Elem, Order, AsFirst >
{
    using type = List< Elem >;
};

template< template< typename... > class List, typename... Rest, typename Elem, template< typename, typename > class Order >
struct sort_insert< List< Rest... >, Elem, Order, true >
{
    using type = List< Elem, Rest... >;
};

template< template< typename... > class List, typename First, typename Elem, template< typename, typename > class Order >
struct sort_insert< List< First >, Elem, Order, false >
{
    using type = List< First, Elem >;
};

template< template< typename... > class List, typename First, typename Second, typename... Rest, typename Elem, template< typename, typename > class Order >
struct sort_insert< List< First, Second, Rest... >, Elem, Order, false >
{
    using type = typename detail::push_front<
        typename detail::sort_insert< List< Second, Rest... >, Elem, Order, Order< Elem, Second >::value >::type,
        First
    >::type;
};

template< template< typename... > class List, typename First, template< typename, typename > class Order >
struct sort< List< First >, Order >
{
    using type = List< First >;
};

template< template< typename... > class List, typename First, typename Second, typename... Rest, template< typename, typename > class Order >
struct sort< List< First, Second, Rest... >, Order >
{
    using type = typename detail::sort_insert<
        typename detail::sort< List< Second, Rest... >, Order >::type,
        First,
        Order,
        Order< First, Second >::value
    >::type;
};

} // namespace detail
} // namespace uuids
} // namespace boost

#endif // BOOST_UUID_DETAIL_TYPE_LIST_HPP_INCLUDED
