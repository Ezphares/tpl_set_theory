#pragma once

#include <type_traits>
#include <utility>

namespace tplsets {

// Basic explicitly enumerated set
template <typename... TElements>
struct Set;

// Number system
template <unsigned long long N>
struct NaturalCardinal
{
    static constexpr unsigned long long value = N;
    static constexpr bool is_finite           = true;
};

// Numeric operations
template <typename, typename>
struct CardinalLess;

// Basic operations
template <typename TSetL, typename TSetR>
struct Union;
template <typename TSetL, typename TSetR>
struct Intersection;
template <typename TSetL, typename TSetR>
struct Difference;
template <typename TSetL, typename TSetR>
struct SymmetricDifference;
template <typename TSetL, typename TSetR>
struct CartesianProduct;
template <typename TSet>
struct PowerSet;

// Helper function declarations
template <typename TElement, typename TSet>
struct _append;
template <typename TElement, typename TSet>
struct _prefix;
template <typename TElement, typename TSet>
struct _permutate;

// Empty set base case
template <>
struct Set<>
{
    using cardinality = NaturalCardinal<0>;

    template <typename T>
    using has_member = std::false_type;

    template <typename Other>
    using is_subset_of = std::true_type;

    template <typename Other>
    using is_proper_subset_of = std::conditional<
        CardinalLess<cardinality, typename Other::cardinality>::value,
        std::true_type,
        std::false_type>;

    using unique = Set<>;
};

/************************************************************************/
/* Set implementation                                                   */
/************************************************************************/
template <typename Head, typename... Tail>
struct Set<Head, Tail...>
{
    using cardinality = NaturalCardinal<sizeof...(Tail) + 1ull>;
    using head        = Head;
    using tail        = typename Set<Tail...>;

    template <typename T>
    using has_member = std::disjunction<
        std::is_same<T, Head>,
        typename Set<Tail...>::template has_member<T>>;

    template <typename Other>
    using is_subset_of = std::conjunction<
        typename Other::template has_member<Head>,
        typename Set<Tail...>::template is_subset_of<Other>>;

    template <typename Other>
    using is_proper_subset_of = std::conjunction<
        CardinalLess<cardinality, typename Other::cardinality>,
        is_subset_of<Other>>;

    using unique = typename std::conditional<
        Set<Tail...>::template has_member<Head>::value,
        typename Set<Tail...>::unique,
        typename _append<Head, typename Set<Tail...>::unique>::result>::type;
};

/************************************************************************/
/* Numeric ops implementations                                          */
/************************************************************************/
template <unsigned long long L, unsigned long long R>
struct CardinalLess<NaturalCardinal<L>, NaturalCardinal<R>>
    : public std::conditional<(L < R), std::true_type, std::false_type>::type
{};

/************************************************************************/
/* Basic operations implementation                                     */
/************************************************************************/
template <typename... L, typename... R>
struct Union<Set<L...>, Set<R...>>
{
    using result = typename Set<L..., R...>::unique;
};

template <typename... R>
struct Intersection<Set<>, Set<R...>>
{
    using result = Set<>;
};
template <typename... L, typename... R>
struct Intersection<Set<L...>, Set<R...>>
{
    using result = typename std::conditional<
        Set<R...>::template has_member<typename Set<L...>::head>::value,
        typename _append<
            typename Set<L...>::head,
            typename Intersection<typename Set<L...>::tail, Set<R...>>::
                result>::_result,
        typename Intersection<typename Set<L...>::tail, Set<R...>>::result>::
        type;
};

template <typename... R>
struct Difference<Set<>, Set<R...>>
{
    using result = Set<>;
};
template <typename... L, typename... R>
struct Difference<Set<L...>, Set<R...>>
{
    using result = typename std::conditional<
        Set<R...>::template has_member<typename Set<L...>::head>::value,
        typename Difference<typename Set<L...>::tail, Set<R...>>::result,
        typename _append<
            typename Set<L...>::head,
            typename Difference<typename Set<L...>::tail, Set<R...>>::result>::
            _result>::type;
};

template <typename... L, typename... R>
struct SymmetricDifference<Set<L...>, Set<R...>>
{
    using result = typename Union<
        typename Difference<Set<L...>, Set<R...>>::result,
        typename Difference<Set<R...>, Set<L...>>::result>::result;
};

template <typename... Rs>
struct CartesianProduct<Set<>, Set<Rs...>>
{
    using result = Set<>;
};
template <typename... Ls, typename... Rs>
struct CartesianProduct<Set<Ls...>, Set<Rs...>>
{
    using result = typename Union<
        typename _prefix<typename Set<Ls...>::head, Set<Rs...>>::result,
        typename CartesianProduct<typename Set<Ls...>::tail, Set<Rs...>>::
            result>::result;
};

template <>
struct PowerSet<Set<>>
{
    using result = Set<Set<>>;
};
template <typename... Ts>
struct PowerSet<Set<Ts...>>
{
    using result = typename Union<
        typename _permutate<
            typename Set<Ts...>::head,
            typename Set<Ts...>::tail>::result,
        typename PowerSet<typename Set<Ts...>::tail>::result>::result;
};

/************************************************************************/
/* Helper functions implementation                                      */
/************************************************************************/
template <typename TElement, typename... TSet>
struct _append<TElement, Set<TSet...>>
{
    using _result = typename Set<TElement, TSet...>;
    using result  = typename std::conditional<
        Set<TSet...>::template has_member<TElement>::value,
        typename Set<TSet...>,
        typename Set<TElement, TSet...>>::type;
};

template <typename T>
struct _prefix<T, Set<>>
{
    using result = Set<>;
};
template <typename T, typename... Ts>
struct _prefix<T, Set<Ts...>>
{
    using result = typename _append<
        typename std::pair<T, typename Set<Ts...>::head>,
        typename _prefix<T, typename Set<Ts...>::tail>::result>::result;
};

template <typename Head>
struct _permutate<Head, Set<>>
{
    using result = Set<Set<Head>>;
};
template <typename Head, typename... Tail>
struct _permutate<Head, Set<Tail...>>
{
    using result = typename Union<
        typename Set<typename _append<Head, Set<Tail...>>::result>,
        typename _permutate<Head, typename Set<Tail...>::tail>::result>::result;
};

/************************************************************************/
/* Predicate experiments                                                */
/************************************************************************/
template <typename TPredicate>
struct PredicateSet
{
    template <typename T>
    using has_member = typename TPredicate::template test<T>;
};

template <typename TSet>
struct _conversion_predicate
{
    template <typename T>
    using test = typename TSet::template has_member<T>;
};

/************************************************************************/
/* std::is_same overload for sets                                       */
/************************************************************************/
template <typename... Ls, typename... Rs>
struct std::is_same<tplsets::Set<Ls...>, tplsets::Set<Rs...>>
    : std::conjunction<
          typename std::is_same<
              typename tplsets::Set<Ls...>::cardinality,
              typename tplsets::Set<Rs...>::cardinality>,
          typename tplsets::Set<Ls...>::template is_subset_of<
              typename tplsets::Set<Rs...>>>
{};

} // namespace tplsets
