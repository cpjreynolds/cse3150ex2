#ifndef PUSHPOP_HPP
#define PUSHPOP_HPP

#include <cassert>
#include <iterator>
#include <memory>
#include <utility>

// push and pop iterators for std::stack and std::queue

template<class Container>
class push_iterator {
public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;

    explicit push_iterator(Container& ctr) : ctr(std::addressof(ctr)) {}

    push_iterator& operator=(const typename Container::value_type& value)
    {
        ctr->push(value);
        return *this;
    }

    push_iterator& operator=(const typename Container::value_type&& value)
    {
        ctr->push(std::move(value));
        return *this;
    }

    // all no-ops
    push_iterator& operator*() { return *this; }
    push_iterator& operator++() { return *this; }
    push_iterator operator++(int) { return *this; }

private:
    Container* ctr;
};

#include <stack>
#include <queue>
#include <type_traits>

template<class Container>
class pop_iterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = typename Container::container_type::value_type;
    using difference_type = void;
    using pointer = typename Container::container_type::pointer;
    using reference = typename Container::container_type::reference;

    static_assert(
        std::is_same<std::stack<value_type>, Container>::value ||
            std::is_same<std::queue<value_type>, Container>::value,
        "pop_iterator should only be used with std::stack or std::queue");

    explicit pop_iterator(Container& c) : ctr(std::addressof(c))
    {
        if (c.empty()) {
            ctr = nullptr;
        }
    }
    // constructs a sentinel
    pop_iterator() : ctr(nullptr) {}

    reference operator*()
    {
        // cant specialize a member function dependent on Container<T>
        // so this works.
        if constexpr (std::is_same<std::stack<value_type>, Container>::value) {
            return ctr->top();
        }
        else if constexpr (std::is_same<std::queue<value_type>,
                                        Container>::value) {
            return ctr->front();
        }
        else {
            // unreachable
            assert(false);
        }
    }

    // pre
    pop_iterator& operator++()
    {
        ctr->pop();
        if (ctr->empty()) {
            ctr = nullptr;
        }
        return *this;
    }

private:
    // postincrement nightmare stuff
    class proxy {
    public:
        proxy(value_type val) : value(val) {}
        value_type operator*() { return value; }

    private:
        value_type value;
    };

public:
    // post
    proxy operator++(int)
    {
        proxy p{operator*()};
        operator++();
        return p;
    }

    friend bool operator==(const pop_iterator& lhs, const pop_iterator& rhs)
    {
        return lhs.ctr == rhs.ctr;
    }

    friend bool operator!=(const pop_iterator& lhs, const pop_iterator& rhs)
    {
        return !(lhs == rhs);
    }

private:
    Container* ctr;
};

template<typename C>
pop_iterator<C> begin(C& c)
{
    return pop_iterator(c);
}

template<typename C>
pop_iterator<C> end(C& c)
{
    return pop_iterator<C>{};
}

#endif
