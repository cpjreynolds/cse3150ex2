#ifdef TESTING
#include "pushpop.hpp"
#include "doctest.h"

#include <stack>
#include <queue>
#include <algorithm>

TEST_CASE("push_iterator")
{
    // setup references to test against.
    std::stack<int> ts;
    std::queue<int> tq;
    for (auto i : {1, 2, 3, 4, 5}) {
        ts.push(i);
        tq.push(i);
    }
    auto dsame = {1, 2, 3, 4, 5};
    auto ddif = {1, 2, 3, 99, 5};

    SUBCASE("push_iterator<queue>")
    {
        std::queue<int> q1, q2;
        std::copy(dsame.begin(), dsame.end(), push_iterator(q1));
        std::copy(ddif.begin(), ddif.end(), push_iterator(q2));
        CHECK(q1 == tq);
        CHECK(q2 != tq);
        CHECK(q1 != q2);
    }
    SUBCASE("push_iterator<stack>")
    {
        std::stack<int> s1, s2;
        std::copy(dsame.begin(), dsame.end(), push_iterator(s1));
        std::copy(ddif.begin(), ddif.end(), push_iterator(s2));
        CHECK(s1 == ts);
        CHECK(s2 != ts);
        CHECK(s1 != s2);
    }
}

TEST_CASE("pop_iterator")
{
    std::stack<int> ts;
    std::queue<int> tq;
    for (auto i : {1, 2, 3, 4, 5}) {
        ts.push(i);
        tq.push(i);
    }

    SUBCASE("pop_iterator<queue>")
    {
        auto dsame = {1, 2, 3, 4, 5};
        auto dit = std::begin(dsame);
        SUBCASE("preincrement")
        {
            for (auto it = begin(tq); it != end(tq); ++it, ++dit) {
                CHECK(*it == *dit);
            }
            CHECK(tq.empty());
        }
        SUBCASE("postincrement")
        {
            auto qit = begin(tq);
            auto qit_end = end(tq);
            while (qit != qit_end) {
                CHECK(*dit++ == *qit++);
            }
            CHECK(tq.empty());
        }
    }
    SUBCASE("pop_iterator<stack>")
    {
        // stack is LIFO
        auto dsame = {5, 4, 3, 2, 1};
        auto dit = std::begin(dsame);
        SUBCASE("preincrement")
        {
            for (auto it = begin(ts); it != end(ts); ++it, ++dit) {
                CHECK(*it == *dit);
            }
            CHECK(ts.empty());
        }
        SUBCASE("postincrement")
        {
            auto sit = begin(ts);
            auto sit_end = end(ts);
            while (sit != sit_end) {
                CHECK(*sit++ == *dit++);
            }
            CHECK(ts.empty());
        }
    }
}

#endif
