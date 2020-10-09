#pragma once

#include "tplsets.h"

namespace tplsets {
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
/* Union overrides                                                      */
/************************************************************************/
// Pure predicate
template <typename L, typename R>
struct Union<PredicateSet<L>, PredicateSet<R>>
{
    struct _predicate
    {
        template <typename T>
        using test = typename std::disjunction<
            typename L::template test<T>,
            typename R::template test<T>>;
    };

    using result = PredicateSet<_predicate>;
};

// Mixed - union is widening, so we have to a predicate set
template <typename... L, typename R>
struct Union<Set<L...>, PredicateSet<R>>
{
    struct _predicate
    {
        template <typename T>
        using test = typename std::disjunction<
            typename Set<L...>::template has_member<T>,
            typename R::template test<T>>;
    };

    using result = PredicateSet<_predicate>;
};

template <typename L, typename... R>
struct Union<PredicateSet<L>, Set<R...>>
    : public Intersection<Set<R...>, PredicateSet<L>>::result
{};
/************************************************************************/
/* Intersection overrides                                               */
/************************************************************************/
// Pure predicate
template <typename L, typename R>
struct Intersection<PredicateSet<L>, PredicateSet<R>>
{
    struct _predicate
    {
        template <typename T>
        using test = typename std::conjunction<
            typename L::template test<T>,
            typename R::template test<T>>;
    };

    using result = PredicateSet<_predicate>;
};

// Mixed - intersection is narrowing, so we can reduce to an explicit set
template <typename R>
struct Intersection<Set<>, PredicateSet<R>>
{
    using result = Set<>;
};

template <typename... L, typename R>
struct Intersection<Set<L...>, PredicateSet<R>>
{
    using result = typename std::conditional<
        PredicateSet<R>::template has_member<typename Set<L...>::head>::value,
        typename _append<
            typename Set<L...>::head,
            typename Intersection<typename Set<L...>::tail, PredicateSet<R>>::
                result>::_result,
        typename Intersection<typename Set<L...>::tail, PredicateSet<R>>::
            result>::type;
};

template <typename L, typename... R>
struct Intersection<PredicateSet<L>, Set<R...>>
    : public Intersection<Set<R...>, PredicateSet<L>>::result
{};

/************************************************************************/
/* Difference overrides                                                 */
/************************************************************************/
// Pure predicate
template <typename L, typename R>
struct Difference<PredicateSet<L>, PredicateSet<R>>
{
    struct _predicate
    {
        template <typename T>
        using test = typename std::conjunction<
            typename L::template test<T>,
            std::negation<typename R::template test<T>>>;
    };

    using result = PredicateSet<_predicate>;
};

// Mixed - LHS determines result
template <typename R>
struct Difference<Set<>, PredicateSet<R>>
{
    using result = Set<>;
};

template <typename... L, typename R>
struct Difference<Set<L...>, PredicateSet<R>>
{
    using result = typename std::conditional<
        PredicateSet<R>::template has_member<typename Set<L...>::head>::value,
        typename Difference<typename Set<L...>::tail, PredicateSet<R>>::result,
        typename _append<
            typename Set<L...>::head,
            typename Difference<typename Set<L...>::tail, PredicateSet<R>>::
                result>::_result>::type;
};

template <typename L, typename... R>
struct Difference<PredicateSet<L>, Set<R...>>
    : public Difference<
          PredicateSet<L>,
          PredicateSet<_conversion_predicate<Set<R...>>>>
{};

/************************************************************************/
/* SymmetricDifference overrides                                        */
/************************************************************************/
// Pure predicate
template <typename L, typename R>
struct SymmetricDifference<PredicateSet<L>, PredicateSet<R>>
{
    using result = typename Union<
        typename Difference<PredicateSet<L>, PredicateSet<R>>::result,
        typename Difference<PredicateSet<R>, PredicateSet<L>>::result>::result;
};

// Mixed - unfortunately all have to be converted to predicate
template <typename... L, typename R>
struct SymmetricDifference<Set<L...>, PredicateSet<R>>
    : public SymmetricDifference<
          PredicateSet<_conversion_predicate<Set<L...>>>,
          PredicateSet<R>>
{};

template <typename L, typename... R>
struct SymmetricDifference<PredicateSet<L>, Set<R...>>
    : public SymmetricDifference<
          PredicateSet<L>,
          PredicateSet<_conversion_predicate<Set<R...>>>>
{};

/************************************************************************/
/* Misc experiments                                                     */
/************************************************************************/
template <typename Ts>
struct _is_natural
{
    using value_type = std::false_type;
};

template <unsigned long long N>
struct _is_natural<NaturalCardinal<N>>
{
    using value_type = std::true_type;
};

struct _natural_predicate
{
    template <typename T>
    using test = typename _is_natural<T>::value_type;
};

struct SetN : public PredicateSet<_natural_predicate>
{
    using cardinality = typename AlephCardinal<0ull>;
};

} // namespace tplsets
