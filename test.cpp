#include "tplsets.h"

#include <iostream>

int main()
{
    using namespace tplsets;

    typedef NaturalCardinal<0> Zero;
    typedef NaturalCardinal<1> One;
    typedef NaturalCardinal<2> Two;

    static_assert(Set<Zero, One>::has_member<Zero>::value, "");
    static_assert(Set<Zero, One>::has_member<One>::value, "");
    static_assert(std::negation<Set<Zero, One>::has_member<Two>>::value, "");

    static_assert(Set<Zero, One>::is_subset_of<Set<Zero, One, Two>>::value, "");
    static_assert(Set<Zero, One>::is_subset_of<Set<Zero, One>>::value, "");
    static_assert(
        std::negation<Set<Zero, One>::is_subset_of<Set<Zero, Two>>>::value, "");

    static_assert(
        Insert<Zero, Set<One, Two>>::result::has_member<Zero>::value, "");
    static_assert(
        Insert<Zero, Set<One, Two>>::result::has_member<One>::value, "");
    static_assert(
        Insert<Zero, Set<One, Two>>::result::has_member<Two>::value, "");

    static_assert(
        Insert<Zero, Set<One, Two>>::result::cardinality::value == 3ull, "");
    static_assert(
        Insert<One, Set<One, Two>>::result::cardinality::value == 2ull, "");

    static_assert(
        Union<Set<Zero>, Set<One>>::result::has_member<One>::value, "");
    static_assert(
        Union<Set<Zero>, Set<One>>::result::has_member<Zero>::value, "");

    return 0;
}
