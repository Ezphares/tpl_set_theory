#pragma once

#include <type_traits>

namespace tplsets {

template <unsigned long long N>
struct NaturalCardinal
{
    static constexpr unsigned long long value = N;
    static constexpr bool is_finite           = true;
};

template <typename, typename>
struct CardinalLess;

template <unsigned long long L, unsigned long long R>
struct CardinalLess<NaturalCardinal<L>, NaturalCardinal<R>>
    : public std::conditional<(L < R), std::true_type, std::false_type>::type
{};

// Empty set base case
template <typename... Tail>
struct Set
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

template <typename, typename>
struct _append;
template <typename, typename>
struct Insert;

template <typename Head, typename... Tail>
struct Set<Head, Tail...>
{
    using cardinality = NaturalCardinal<sizeof...(Tail) + 1ull>;

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

template <typename Head, typename... Tail>
struct _append<Head, Set<Tail...>>
{
    using result = typename Set<Head, Tail...>;
};

template <typename Head, typename... Tail>
struct Insert<Head, Set<Tail...>>
{
    using result = typename std::conditional<
        Set<Tail...>::template has_member<Head>::value,
        Set<Tail...>,
        typename _append<Head, Set<Tail...>>::result>::type;
};

template <typename, typename>
struct Union;
template <typename... L, typename... R>
struct Union<Set<L...>, Set<R...>>
{
    using result = typename Set<L..., R...>::unique;
};

} // namespace tplsets
