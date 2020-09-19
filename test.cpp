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
        Set<Zero, One>::is_proper_subset_of<Set<Zero, One, Two>>::value, "");
    static_assert(
        std::negation<
            Set<Zero, One>::is_proper_subset_of<Set<Zero, One>>>::value,
        "");

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

    static_assert(
        std::is_same<
            Set<One>,
            Intersection<Set<Zero, One>, Set<One, Two>>::result>::value,
        "");

    static_assert(
        std::is_same<
            Set<Zero>,
            Difference<Set<Zero, One>, Set<One, Two>>::result>::value,
        "");

    static_assert(
        std::is_same<
            Set<Zero, Two>,
            SymmetricDifference<Set<Zero, One>, Set<One, Two>>::result>::value,
        "");

    {
        typedef CartesianProduct<Set<Zero, One>, Set<One, Two>>::result
            TestCartesianProduct;

        static_assert(
            std::is_same<
                TestCartesianProduct::cardinality,
                NaturalCardinal<4>>::value,
            "");
        static_assert(
            TestCartesianProduct::template has_member<
                std::pair<Zero, One>>::value,
            "");
        static_assert(
            TestCartesianProduct::template has_member<
                std::pair<Zero, Two>>::value,
            "");
        static_assert(
            TestCartesianProduct::template has_member<
                std::pair<One, One>>::value,
            "");
        static_assert(
            TestCartesianProduct::template has_member<
                std::pair<One, Two>>::value,
            "");
    }
    return 0;
}
