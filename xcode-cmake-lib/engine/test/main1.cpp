#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Calculator
#include <boost/test/unit_test.hpp>

#include <iostream>

BOOST_AUTO_TEST_SUITE(CalculatorTests)

BOOST_AUTO_TEST_CASE(PlusTest)
{
    BOOST_CHECK_EQUAL(1 + 2, 3);
}

BOOST_AUTO_TEST_CASE(MinusTestFailed)
{
    BOOST_CHECK_EQUAL(2 - 1, 2);
}

BOOST_AUTO_TEST_SUITE_END()
