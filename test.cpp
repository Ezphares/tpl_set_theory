#include "tplsets.h"
#include "tplsets_predicate.h"

#include <iostream>

int main()
{
    using namespace tplsets;

    typedef NaturalCardinal<0> Zero;
    typedef NaturalCardinal<1> One;
    typedef NaturalCardinal<2> Two;

    {
        // Basic property test
        typedef Set<Zero, One> TestHasMember;

        static_assert(
            std::is_same<TestHasMember::cardinality, NaturalCardinal<2u>>::
                value,
            "");

        static_assert(TestHasMember::has_member<Zero>::value, "");
        static_assert(TestHasMember::has_member<One>::value, "");
        static_assert(std::negation<TestHasMember::has_member<Two>>::value, "");
    }

    {
        // Test unordered-ness

        static_assert(std::is_same<Set<Zero, One>, Set<One, Zero>>::value, "");
    }

    {
        // Subset test
        static_assert(
            Set<Zero, One>::is_subset_of<Set<Zero, One, Two>>::value, "");
        static_assert(Set<Zero, One>::is_subset_of<Set<Zero, One>>::value, "");
        static_assert(
            std::negation<Set<Zero, One>::is_subset_of<Set<Zero, Two>>>::value,
            "");
    }

    {
        // Proper subset test
        static_assert(
            Set<Zero, One>::is_proper_subset_of<Set<Zero, One, Two>>::value,
            "");
        static_assert(
            std::negation<
                Set<Zero, One>::is_proper_subset_of<Set<Zero, One>>>::value,
            "");
    }

    {
        // Mutation test

        static_assert(
            _append<Zero, Set<One, Two>>::result::unique::has_member<
                Zero>::value,
            "");
        static_assert(
            _append<Zero, Set<One, Two>>::result::unique::has_member<
                One>::value,
            "");
        static_assert(
            _append<Zero, Set<One, Two>>::result::unique::has_member<
                Two>::value,
            "");

        static_assert(
            _append<Zero, Set<One, Two>>::result::unique::cardinality::value
                == 3ull,
            "");
        static_assert(
            _append<One, Set<One, Two>>::result::unique::cardinality::value
                == 2ull,
            "");
    }

    {
        // Union test

        static_assert(
            Union<Set<Zero>, Set<One>>::result::has_member<One>::value, "");
        static_assert(
            Union<Set<Zero>, Set<One>>::result::has_member<Zero>::value, "");
    }

    {
        // Intersection test

        static_assert(
            std::is_same<
                Set<One>,
                Intersection<Set<Zero, One>, Set<One, Two>>::result>::value,
            "");
    }

    {
        // Test difference

        static_assert(
            std::is_same<
                Set<Zero>,
                Difference<Set<Zero, One>, Set<One, Two>>::result>::value,
            "");
    }

    {
        // Test symmetric difference

        static_assert(
            std::is_same<
                Set<Zero, Two>,
                SymmetricDifference<Set<Zero, One>, Set<One, Two>>::result>::
                value,
            "");
    }

    {
        // Cartesian product test

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

    {
        // Test permutation helper

        typedef _permutate<Zero, Set<One, Two>>::result TestPermutate;

        static_assert(
            std::is_same<TestPermutate::cardinality, NaturalCardinal<3u>>::
                value,
            "");

        static_assert(TestPermutate::template has_member<Set<Zero>>::value, "");

        static_assert(
            TestPermutate::template has_member<Set<Zero, Two>>::value, "");

        static_assert(
            TestPermutate::template has_member<Set<Zero, One, Two>>::value, "");
    }

    {
        // Test power set

        typedef PowerSet<Set<Zero, One>>::result TestPowerset;

        static_assert(
            std::is_same<TestPowerset::cardinality, NaturalCardinal<4u>>::value,
            "");

        static_assert(TestPowerset::template has_member<Set<>>::value, "");
        static_assert(TestPowerset::template has_member<Set<Zero>>::value, "");
        static_assert(TestPowerset::template has_member<Set<One>>::value, "");
        static_assert(
            TestPowerset::template has_member<Set<Zero, One>>::value, "");
    }

    {
        // Test conversion to predicate based set

        typedef PredicateSet<_conversion_predicate<Set<Zero, One>>>
            TestConversionPredicate;

        static_assert(
            TestConversionPredicate::template has_member<Zero>::value, "");
        static_assert(
            TestConversionPredicate::template has_member<One>::value, "");
        static_assert(
            std::negation<
                TestConversionPredicate::template has_member<Two>>::value,
            "");
    }

    return 0;
}
