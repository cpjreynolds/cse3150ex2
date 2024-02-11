#include <iterator>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <stack>

#include "pushpop.hpp"

// read input stream of integers onto a stack or queue by pushing.
//
// throws exception if stream contains non-integers.
template<class C>
C read_into(std::istream& is)
{
    C container;
    // read space delimited strings
    auto iter = std::istream_iterator<std::string>(is);
    // convert them to integers and push onto container
    std::transform(iter, std::istream_iterator<std::string>(),
                   push_iterator<C>(container),
                   [](const std::string& s) { return std::stoi(s); });
    return container;
}

// returns the sum of the container if the sum is positive, throws an exception
// if the sum is negative.
//
// the container must be one of either `stack<int>` or `queue<int>`
//
// the assignment specified a non-negative *sum*, not prefix sum, so I can't
// reuse the prefix.hpp functions.
template<class C>
int non_neg_sum(C& container)
{
    auto it = pop_iterator(container);
    auto end_it = pop_iterator<C>();
    int sum = std::accumulate(it, end_it, 0);
    if (sum < 0) {
        throw std::domain_error("non-negative sum");
    }
    else {
        return sum;
    }
}

#ifndef TESTING

// simple output function to print the stack
static std::ostream& operator<<(std::ostream& os, std::stack<int> s)
{
    os << "{";
    auto sz = s.size();
    for (size_t i = 1; i < sz; ++i) {
        os << s.top() << ", ";
        s.pop();
    }
    if (s.size() == 1) {
        os << s.top();
        s.pop();
    }
    os << "}";
    return os;
}

// simple output function to print the queue
static std::ostream& operator<<(std::ostream& os, std::queue<int> q)
{
    os << "{";
    auto sz = q.size();
    for (size_t i = 1; i < sz; ++i) {
        os << q.front() << ", ";
        q.pop();
    }
    if (q.size() == 1) {
        os << q.front();
        q.pop();
    }
    os << "}";
    return os;
}

static constexpr std::string_view USAGE =
    "USAGE: ./ex2.out [filename=\"test.txt\"]";
static constexpr std::string_view DEFAULT_FNAME = "test.txt";

int main(int argc, char** argv)
{
    std::string_view fname = DEFAULT_FNAME;
    if (argc == 2) {
        fname = argv[1];
    }
    else if (argc != 1) {
        std::cerr << USAGE << std::endl;
        return 1;
    }

    auto fs = std::ifstream(fname);

    if (!fs.is_open()) {
        std::cerr << "failed to open file: " << fname << std::endl;
        std::cerr << std::endl << USAGE << std::endl;
        return 1;
    }

    std::stack<int> svals;
    std::queue<int> qvals;
    try {
        svals = read_into<std::stack<int>>(fs);
        fs.clear();
        fs.seekg(0);
        qvals = read_into<std::queue<int>>(fs);
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "invalid integer in file: " << fname << std::endl;
        std::cerr << std::endl << USAGE << std::endl;
        return 1;
    }
    std::cout << "stack: " << svals << std::endl;
    std::cout << "queue: " << qvals << std::endl;

    int ssum, qsum;
    try {
        ssum = non_neg_sum(svals);
        qsum = non_neg_sum(qvals);
    }
    catch (const std::domain_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    assert(ssum == qsum);
    std::cout << "sum: " << ssum << std::endl;
    return 0;
}

#else
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("read_into")
{
    std::istringstream indata{"1 2 3 4 5 6"};
    std::istringstream baddata{"1 2 3 x 4 5 6"};
    std::stack<int> stest;
    stest.push(1);
    stest.push(2);
    stest.push(3);
    stest.push(4);
    stest.push(5);
    stest.push(6);
    std::queue<int> qtest;
    qtest.push(1);
    qtest.push(2);
    qtest.push(3);
    qtest.push(4);
    qtest.push(5);
    qtest.push(6);
    SUBCASE("stack")
    {
        auto s = read_into<std::stack<int>>(indata);
        CHECK(s == stest);
    }
    SUBCASE("stack-bad")
    {
        CHECK_THROWS(read_into<std::stack<int>>(baddata));
    }
    SUBCASE("queue")
    {
        auto q = read_into<std::queue<int>>(indata);
        CHECK(q == qtest);
    }
    SUBCASE("queue-bad")
    {
        CHECK_THROWS(read_into<std::queue<int>>(baddata));
    }
}

TEST_CASE("non_neg_sum")
{
    std::istringstream indata{"1 2 3 4 5 6"};
    std::istringstream negdata{"10 -5 -5 -5 10 -5 -5"};
    SUBCASE("stack")
    {
        auto s = read_into<std::stack<int>>(indata);
        auto sum = non_neg_sum(s);
        CHECK(sum == 21);
    }
    SUBCASE("stack-bad")
    {
        auto sb = read_into<std::stack<int>>(negdata);
        CHECK_THROWS(non_neg_sum(sb));
    }
    SUBCASE("queue")
    {
        auto q = read_into<std::queue<int>>(indata);
        auto sum = non_neg_sum(q);
        CHECK(sum == 21);
    }
    SUBCASE("queue-bad")
    {
        auto qb = read_into<std::queue<int>>(negdata);
        CHECK_THROWS(non_neg_sum(qb));
    }
}

#endif
