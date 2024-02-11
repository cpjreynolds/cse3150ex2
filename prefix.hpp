#ifndef PREFIX_HPP
#define PREFIX_HPP

/*
 * Takes any integer iterable and tests its prefix sums for non-negativity
 */
template<class I>
bool non_neg_prefix_sum(const I& lst)
{
    int sum = 0;
    for (const int& i : lst) {
        sum += i;
        if (sum < 0) {
            return false;
        }
    }
    return true;
}

/*
 * Takes any integer iterable and tests its prefix sums for non-positivity
 */
template<class I>
bool non_pos_prefix_sum(const I& lst)
{
    int sum = 0;
    for (const int& i : lst) {
        sum += i;
        if (sum > 0) {
            return false;
        }
    }
    return true;
}

// alternate overloads taking iterators directly
template<class It>
bool non_neg_prefix_sum(It first, It last)
{
    int sum = 0;
    while (first != last) {
        sum += *first;
        ++first;
        if (sum < 0) {
            return false;
        }
    }
    return true;
}

template<class It>
bool non_pos_prefix_sum(It first, It last)
{
    int sum = 0;
    while (first != last) {
        sum += *first;
        ++first;
        if (sum > 0) {
            return false;
        }
    }
    return true;
}

#endif
