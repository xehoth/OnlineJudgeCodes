/*
 * created by xehoth on 20-03-2017
 */
//#define DBG
#define NODEBUG
#define USING_BITS
#ifndef XEHOTH_HEADER
#define XEHOTH_HEADER
#ifdef USING_BITS
#include <bits/stdc++.h>
#else
#ifndef _glibcxx_no_assert

#include <cassert>

#endif

#include <cctype>
#include <cerrno>
#include <cfloat>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#if __cplusplus >= 201103l
#include <ccomplex>
#include <cfenv>
#include <cinttypes>
#include <cstdalign>
#include <cstdbool>
#include <cstdint>
#include <ctgmath>
#include <cwchar>
#include <cwctype>
#endif

#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>

#if __cplusplus >= 201103l
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#endif
#endif
#ifdef USING_TR1
#include <tr1/array>
#include <tr1/cctype>
#include <tr1/cfenv>
#include <tr1/cfloat>
#include <tr1/cinttypes>
#include <tr1/climits>
#include <tr1/cmath>
#include <tr1/complex>
#include <tr1/cstdarg>
#include <tr1/cstdbool>
#include <tr1/cstdint>
#include <tr1/cstdio>
#include <tr1/cstdlib>
#include <tr1/ctgmath>
#include <tr1/ctime>
#include <tr1/cwchar>
#include <tr1/cwctype>
#include <tr1/functional>
#include <tr1/random>
#include <tr1/tuple>
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#include <tr1/utility>
#endif
#ifdef USING_EXT
#include <ext/array_allocator.h>
#include <ext/atomicity.h>
#include <ext/bitmap_allocator.h>
#include <ext/cast.h>
#include <ext/concurrence.h>
#include <ext/debug_allocator.h>
#include <ext/extptr_allocator.h>
#include <ext/malloc_allocator.h>
#include <ext/mt_allocator.h>
#include <ext/new_allocator.h>
#include <ext/pod_char_traits.h>
#include <ext/pointer.h>
#include <ext/pool_allocator.h>
#include <ext/stdio_filebuf.h>
#include <ext/stdio_sync_filebuf.h>
#include <ext/throw_allocator.h>
#include <ext/type_traits.h>
#include <ext/typelist.h>
#include <ext/vstring.h>
#include <ext/algorithm>
#include <ext/functional>
#include <ext/iterator>
#include <ext/memory>
#include <ext/numeric>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/exception.hpp>
#include <ext/pb_ds/hash_policy.hpp>
#include <ext/pb_ds/list_update_policy.hpp>
#include <ext/pb_ds/priority_queue.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/trie_policy.hpp>
#include <ext/rb_tree>
#include <ext/rope>
#include <ext/slist>
#endif

#ifdef CONSTANT
const double pi = acos(-1);
const double pi2 = 2 * acos(-1);
typedef long long ll;
typedef long long long;
typedef unsigned long long ull;
typedef unsigned long long ulong;
typedef unsigned int uint;
typedef long double ld;
typedef __float128 float128;
#endif

#ifdef FUNCTIONS

template <class t>
inline void clear(t *a) {
    memset(a, 0, sizeof(a));
}

template <class t>
inline void clear(t *a, const int n) {
    memset(a, 0, sizeof(t) * n);
}

template <class t>
inline void copy(t *s, t *t) {
    memcpy(t, s, sizeof(s));
}

template <class t>
inline void copy(t *s, t *t, const int n) {
    memcpy(s, t, sizeof(t) * n);
}

template <class t>
inline void addval(t &x, const t &v, const t &mod) {
    x += v;
    if (x >= mod) x -= mod;
    if (x < 0) x += mod;
}

inline int randomInt() {
    return (bool(rand() & 1) << 30) | (rand() << 15) + rand();
}

inline int random(const int l, const int r) {
    return randomInt() % (r - l + 1) + l;
}

inline bool readBits(const uint x, const int pos) { return (x >> pos) & 1; }

inline uint readBits(uint x, const int pos, const int cnt) {
    return x >> pos & ((1u << cnt) - 1);
}

inline int countBits(const uint x) { return __builtin_popcount(x); }
#endif

#ifdef STD_IO
inline void initio() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
}
#endif

#ifdef MEMORY_POOL
template <class t, size_t size>
struct MemoryPool {
    t buf[size], *tail, *end;
#ifdef RECYCLE
    int top;
    t *st[size];
    MemoryPool() : top(0), tail(buf), end(buf + size) {}
#else
    MemoryPool() : tail(buf), end(buf + size) {}
#endif
    inline t *alloc() {
#ifdef RECYCLE
        if (top) return st[--top];
#endif
        if (tail != end) return tail++;
        return new t;
    }
#ifdef RECYCLE
    inline void RECYCLE(t *x) {
        if (top > size)
            delete x;
        else
            st[top++] = x;
    }
#endif
};
#endif

#ifdef BOOST_ALGORITHM
//  (C) Copyright Herve Bronnimann 2004.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/*
 Revision history:
   1 July 2004
      Split the code into two headers to lessen dependence on
      Boost.tuple. (Herve)
   26 June 2004
      Added the code for the boost minmax library. (Herve)
*/

#ifndef BOOST_ALGORITHM_MINMAX_ELEMENT_HPP
#define BOOST_ALGORITHM_MINMAX_ELEMENT_HPP

/* PROPOSED STANDARD EXTENSIONS:
 *
 * minmax_element(first, last)
 * Effect: std::make_pair( std::min_element(first, last),
 *                         std::max_element(first, last) );
 *
 * minmax_element(first, last, comp)
 * Effect: std::make_pair( std::min_element(first, last, comp),
 *                         std::max_element(first, last, comp) );
 */

#include <utility>  // for std::pair and std::make_pair

namespace boost {

namespace detail {  // for obtaining a uniform version of minmax_element
// that compiles with VC++ 6.0 -- avoid the iterator_traits by
// having comparison object over iterator, not over dereferenced value

template <typename Iterator>
struct less_over_iter {
    bool operator()(Iterator const &it1, Iterator const &it2) const {
        return *it1 < *it2;
    }
};

template <typename Iterator, class BinaryPredicate>
struct binary_pred_over_iter {
    explicit binary_pred_over_iter(BinaryPredicate const &p) : m_p(p) {}
    bool operator()(Iterator const &it1, Iterator const &it2) const {
        return m_p(*it1, *it2);
    }

   private:
    BinaryPredicate m_p;
};

// common base for the two minmax_element overloads

template <typename ForwardIter, class Compare>
std::pair<ForwardIter, ForwardIter> basic_minmax_element(ForwardIter first,
                                                         ForwardIter last,
                                                         Compare comp) {
    if (first == last) return std::make_pair(last, last);

    ForwardIter min_result = first;
    ForwardIter max_result = first;

    // if only one element
    ForwardIter second = first;
    ++second;
    if (second == last) return std::make_pair(min_result, max_result);

    // treat first pair separately (only one comparison for first two elements)
    ForwardIter potential_min_result = last;
    if (comp(first, second))
        max_result = second;
    else {
        min_result = second;
        potential_min_result = first;
    }

    // then each element by pairs, with at most 3 comparisons per pair
    first = ++second;
    if (first != last) ++second;
    while (second != last) {
        if (comp(first, second)) {
            if (comp(first, min_result)) {
                min_result = first;
                potential_min_result = last;
            }
            if (comp(max_result, second)) max_result = second;
        } else {
            if (comp(second, min_result)) {
                min_result = second;
                potential_min_result = first;
            }
            if (comp(max_result, first)) max_result = first;
        }
        first = ++second;
        if (first != last) ++second;
    }

    // if odd number of elements, treat last element
    if (first != last) {  // odd number of elements
        if (comp(first, min_result)) {
            min_result = first;
            potential_min_result = last;
        } else if (comp(max_result, first))
            max_result = first;
    }

    // resolve min_result being incorrect with one extra comparison
    // (in which case potential_min_result is necessarily the correct result)
    if (potential_min_result != last && !comp(min_result, potential_min_result))
        min_result = potential_min_result;

    return std::make_pair(min_result, max_result);
}

}  // namespace detail

template <typename ForwardIter>
std::pair<ForwardIter, ForwardIter> minmax_element(ForwardIter first,
                                                   ForwardIter last) {
    return detail::basic_minmax_element(first, last,
                                        detail::less_over_iter<ForwardIter>());
}

template <typename ForwardIter, class BinaryPredicate>
std::pair<ForwardIter, ForwardIter> minmax_element(ForwardIter first,
                                                   ForwardIter last,
                                                   BinaryPredicate comp) {
    return detail::basic_minmax_element(
        first, last,
        detail::binary_pred_over_iter<ForwardIter, BinaryPredicate>(comp));
}

}  // namespace boost

/* PROPOSED BOOST EXTENSIONS
 * In the description below, [rfirst,rlast) denotes the reversed range
 * of [first,last). Even though the iterator type of first and last may
 * be only a Forward Iterator, it is possible to explain the semantics
 * by assuming that it is a Bidirectional Iterator. In the sequel,
 * reverse(ForwardIterator&) returns the reverse_iterator adaptor.
 * This is not how the functions would be implemented!
 *
 * first_min_element(first, last)
 * Effect: std::min_element(first, last);
 *
 * first_min_element(first, last, comp)
 * Effect: std::min_element(first, last, comp);
 *
 * last_min_element(first, last)
 * Effect: reverse( std::min_element(reverse(last), reverse(first)) );
 *
 * last_min_element(first, last, comp)
 * Effect: reverse( std::min_element(reverse(last), reverse(first), comp) );
 *
 * first_max_element(first, last)
 * Effect: std::max_element(first, last);
 *
 * first_max_element(first, last, comp)
 * Effect: max_element(first, last);
 *
 * last_max_element(first, last)
 * Effect: reverse( std::max_element(reverse(last), reverse(first)) );
 *
 * last_max_element(first, last, comp)
 * Effect: reverse( std::max_element(reverse(last), reverse(first), comp) );
 *
 * first_min_first_max_element(first, last)
 * Effect: std::make_pair( first_min_element(first, last),
 *                         first_max_element(first, last) );
 *
 * first_min_first_max_element(first, last, comp)
 * Effect: std::make_pair( first_min_element(first, last, comp),
 *                         first_max_element(first, last, comp) );
 *
 * first_min_last_max_element(first, last)
 * Effect: std::make_pair( first_min_element(first, last),
 *                         last_max_element(first, last) );
 *
 * first_min_last_max_element(first, last, comp)
 * Effect: std::make_pair( first_min_element(first, last, comp),
 *                         last_max_element(first, last, comp) );
 *
 * last_min_first_max_element(first, last)
 * Effect: std::make_pair( last_min_element(first, last),
 *                         first_max_element(first, last) );
 *
 * last_min_first_max_element(first, last, comp)
 * Effect: std::make_pair( last_min_element(first, last, comp),
 *                         first_max_element(first, last, comp) );
 *
 * last_min_last_max_element(first, last)
 * Effect: std::make_pair( last_min_element(first, last),
 *                         last_max_element(first, last) );
 *
 * last_min_last_max_element(first, last, comp)
 * Effect: std::make_pair( last_min_element(first, last, comp),
 *                         last_max_element(first, last, comp) );
 */

namespace boost {

// Min_element and max_element variants

namespace detail {  // common base for the overloads

template <typename ForwardIter, class BinaryPredicate>
ForwardIter basic_first_min_element(ForwardIter first, ForwardIter last,
                                    BinaryPredicate comp) {
    if (first == last) return last;
    ForwardIter min_result = first;
    while (++first != last)
        if (comp(first, min_result)) min_result = first;
    return min_result;
}

template <typename ForwardIter, class BinaryPredicate>
ForwardIter basic_last_min_element(ForwardIter first, ForwardIter last,
                                   BinaryPredicate comp) {
    if (first == last) return last;
    ForwardIter min_result = first;
    while (++first != last)
        if (!comp(min_result, first)) min_result = first;
    return min_result;
}

template <typename ForwardIter, class BinaryPredicate>
ForwardIter basic_first_max_element(ForwardIter first, ForwardIter last,
                                    BinaryPredicate comp) {
    if (first == last) return last;
    ForwardIter max_result = first;
    while (++first != last)
        if (comp(max_result, first)) max_result = first;
    return max_result;
}

template <typename ForwardIter, class BinaryPredicate>
ForwardIter basic_last_max_element(ForwardIter first, ForwardIter last,
                                   BinaryPredicate comp) {
    if (first == last) return last;
    ForwardIter max_result = first;
    while (++first != last)
        if (!comp(first, max_result)) max_result = first;
    return max_result;
}

}  // namespace detail

template <typename ForwardIter>
ForwardIter first_min_element(ForwardIter first, ForwardIter last) {
    return detail::basic_first_min_element(
        first, last, detail::less_over_iter<ForwardIter>());
}

template <typename ForwardIter, class BinaryPredicate>
ForwardIter first_min_element(ForwardIter first, ForwardIter last,
                              BinaryPredicate comp) {
    return detail::basic_first_min_element(
        first, last,
        detail::binary_pred_over_iter<ForwardIter, BinaryPredicate>(comp));
}

template <typename ForwardIter>
ForwardIter last_min_element(ForwardIter first, ForwardIter last) {
    return detail::basic_last_min_element(
        first, last, detail::less_over_iter<ForwardIter>());
}

template <typename ForwardIter, class BinaryPredicate>
ForwardIter last_min_element(ForwardIter first, ForwardIter last,
                             BinaryPredicate comp) {
    return detail::basic_last_min_element(
        first, last,
        detail::binary_pred_over_iter<ForwardIter, BinaryPredicate>(comp));
}

template <typename ForwardIter>
ForwardIter first_max_element(ForwardIter first, ForwardIter last) {
    return detail::basic_first_max_element(
        first, last, detail::less_over_iter<ForwardIter>());
}

template <typename ForwardIter, class BinaryPredicate>
ForwardIter first_max_element(ForwardIter first, ForwardIter last,
                              BinaryPredicate comp) {
    return detail::basic_first_max_element(
        first, last,
        detail::binary_pred_over_iter<ForwardIter, BinaryPredicate>(comp));
}

template <typename ForwardIter>
ForwardIter last_max_element(ForwardIter first, ForwardIter last) {
    return detail::basic_last_max_element(
        first, last, detail::less_over_iter<ForwardIter>());
}

template <typename ForwardIter, class BinaryPredicate>
ForwardIter last_max_element(ForwardIter first, ForwardIter last,
                             BinaryPredicate comp) {
    return detail::basic_last_max_element(
        first, last,
        detail::binary_pred_over_iter<ForwardIter, BinaryPredicate>(comp));
}

// Minmax_element variants -- comments removed

namespace detail {

template <typename ForwardIter, class BinaryPredicate>
std::pair<ForwardIter, ForwardIter> basic_first_min_last_max_element(
    ForwardIter first, ForwardIter last, BinaryPredicate comp) {
    if (first == last) return std::make_pair(last, last);

    ForwardIter min_result = first;
    ForwardIter max_result = first;

    ForwardIter second = ++first;
    if (second == last) return std::make_pair(min_result, max_result);

    if (comp(second, min_result))
        min_result = second;
    else
        max_result = second;

    first = ++second;
    if (first != last) ++second;
    while (second != last) {
        if (!comp(second, first)) {
            if (comp(first, min_result)) min_result = first;
            if (!comp(second, max_result)) max_result = second;
        } else {
            if (comp(second, min_result)) min_result = second;
            if (!comp(first, max_result)) max_result = first;
        }
        first = ++second;
        if (first != last) ++second;
    }

    if (first != last) {
        if (comp(first, min_result))
            min_result = first;
        else if (!comp(first, max_result))
            max_result = first;
    }

    return std::make_pair(min_result, max_result);
}

template <typename ForwardIter, class BinaryPredicate>
std::pair<ForwardIter, ForwardIter> basic_last_min_first_max_element(
    ForwardIter first, ForwardIter last, BinaryPredicate comp) {
    if (first == last) return std::make_pair(last, last);

    ForwardIter min_result = first;
    ForwardIter max_result = first;

    ForwardIter second = ++first;
    if (second == last) return std::make_pair(min_result, max_result);

    if (comp(max_result, second))
        max_result = second;
    else
        min_result = second;

    first = ++second;
    if (first != last) ++second;
    while (second != last) {
        if (comp(first, second)) {
            if (!comp(min_result, first)) min_result = first;
            if (comp(max_result, second)) max_result = second;
        } else {
            if (!comp(min_result, second)) min_result = second;
            if (comp(max_result, first)) max_result = first;
        }
        first = ++second;
        if (first != last) ++second;
    }

    if (first != last) {
        if (!comp(min_result, first))
            min_result = first;
        else if (comp(max_result, first))
            max_result = first;
    }

    return std::make_pair(min_result, max_result);
}

template <typename ForwardIter, class BinaryPredicate>
std::pair<ForwardIter, ForwardIter> basic_last_min_last_max_element(
    ForwardIter first, ForwardIter last, BinaryPredicate comp) {
    if (first == last) return std::make_pair(last, last);

    ForwardIter min_result = first;
    ForwardIter max_result = first;

    ForwardIter second = first;
    ++second;
    if (second == last) return std::make_pair(min_result, max_result);

    ForwardIter potential_max_result = last;
    if (comp(first, second))
        max_result = second;
    else {
        min_result = second;
        potential_max_result = second;
    }

    first = ++second;
    if (first != last) ++second;
    while (second != last) {
        if (comp(first, second)) {
            if (!comp(min_result, first)) min_result = first;
            if (!comp(second, max_result)) {
                max_result = second;
                potential_max_result = last;
            }
        } else {
            if (!comp(min_result, second)) min_result = second;
            if (!comp(first, max_result)) {
                max_result = first;
                potential_max_result = second;
            }
        }
        first = ++second;
        if (first != last) ++second;
    }

    if (first != last) {
        if (!comp(min_result, first)) min_result = first;
        if (!comp(first, max_result)) {
            max_result = first;
            potential_max_result = last;
        }
    }

    if (potential_max_result != last && !comp(potential_max_result, max_result))
        max_result = potential_max_result;

    return std::make_pair(min_result, max_result);
}

}  // namespace detail

template <typename ForwardIter>
inline std::pair<ForwardIter, ForwardIter> first_min_first_max_element(
    ForwardIter first, ForwardIter last) {
    return minmax_element(first, last);
}

template <typename ForwardIter, class BinaryPredicate>
inline std::pair<ForwardIter, ForwardIter> first_min_first_max_element(
    ForwardIter first, ForwardIter last, BinaryPredicate comp) {
    return minmax_element(first, last, comp);
}

template <typename ForwardIter>
std::pair<ForwardIter, ForwardIter> first_min_last_max_element(
    ForwardIter first, ForwardIter last) {
    return detail::basic_first_min_last_max_element(
        first, last, detail::less_over_iter<ForwardIter>());
}

template <typename ForwardIter, class BinaryPredicate>
inline std::pair<ForwardIter, ForwardIter> first_min_last_max_element(
    ForwardIter first, ForwardIter last, BinaryPredicate comp) {
    return detail::basic_first_min_last_max_element(
        first, last,
        detail::binary_pred_over_iter<ForwardIter, BinaryPredicate>(comp));
}

template <typename ForwardIter>
std::pair<ForwardIter, ForwardIter> last_min_first_max_element(
    ForwardIter first, ForwardIter last) {
    return detail::basic_last_min_first_max_element(
        first, last, detail::less_over_iter<ForwardIter>());
}

template <typename ForwardIter, class BinaryPredicate>
inline std::pair<ForwardIter, ForwardIter> last_min_first_max_element(
    ForwardIter first, ForwardIter last, BinaryPredicate comp) {
    return detail::basic_last_min_first_max_element(
        first, last,
        detail::binary_pred_over_iter<ForwardIter, BinaryPredicate>(comp));
}

template <typename ForwardIter>
std::pair<ForwardIter, ForwardIter> last_min_last_max_element(
    ForwardIter first, ForwardIter last) {
    return detail::basic_last_min_last_max_element(
        first, last, detail::less_over_iter<ForwardIter>());
}

template <typename ForwardIter, class BinaryPredicate>
inline std::pair<ForwardIter, ForwardIter> last_min_last_max_element(
    ForwardIter first, ForwardIter last, BinaryPredicate comp) {
    return detail::basic_last_min_last_max_element(
        first, last,
        detail::binary_pred_over_iter<ForwardIter, BinaryPredicate>(comp));
}

}  // namespace boost

#endif  // BOOST_ALGORITHM_MINMAX_ELEMENT_HPP
#endif

namespace IO {

template <class T>
inline T parseFloat(char *str) {
    char *s = str;
    if (*s == '0' || *s == '\0') return 0.0;
    register T sum = 0.0;
    register int flag = 1, pow = 0;
    if (*s == '-') flag = -1, s++;
    while (*s != '\0') {
        if (*s == '.') {
            pow = 1, s++;
            continue;
        }
        sum = *s - '0' + sum * 10, pow *= 10, s++;
    }
    return flag * sum / pow;
}

template <size_t size = 1000000>
struct BufferedInputStream {
    char buf[size], *s, *t;

    inline char read() {
        if (s == t) {
            t = (s = buf) + fread(buf, 1, size, stdin);
            if (s == t) return -1;
        }
        return *s++;
    }

    inline void read(char &c) { c = read(); }

    template <class T>
    inline void read(T &x) {
        static bool iosig;
        static char c;
        for (iosig = false, c = read(); !isdigit(c); c = read()) {
            if (c == '-') iosig = true;
            if (c == -1) return;
        }
        for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
        if (iosig) x = -x;
    }

    inline int read(char *buf) {
        register size_t s = 0;
        register char ch;
        while (ch = read(), isspace(ch) && ch != -1)
            ;
        if (ch == EOF) {
            *buf = '\0';
            return -1;
        }
        do
            buf[s++] = ch;
        while (ch = read(), !isspace(ch) && ch != -1);
        buf[s] = '\0';
        return s;
    }

    inline void read(float &x) {
        static char buf[64];
        read(buf);
        x = parseFloat<float>(buf);
    }

    inline void read(double &x) {
        static char buf[128];
        read(buf);
        x = parseFloat<float>(buf);
    }

    template <class T1, class T2>
    inline void read(T1 &a, T2 &b) {
        read(a), read(b);
    }

    template <class T1, class T2, class T3>
    inline void read(T1 &a, T2 &b, T3 &c) {
        read(a), read(b), read(c);
    }

    template <class T1, class T2, class T3, class T4>
    inline void read(T1 &a, T2 &b, T3 &c, T4 &d) {
        read(a), read(b), read(c), read(d);
    }

    inline int nextInt() {
        register int i;
        read(i);
        return i;
    }

    inline long nextLong() {
        register long i;
        read(i);
        return i;
    }

    inline float nextFloat() {
        register float i;
        read(i);
        return i;
    }

    inline double nextDouble() {
        register double i;
        read(i);
        return i;
    }
};

template <size_t size = 1000000>
struct BufferedOutputStream {
    char buf[size], *s;

    inline void print(char c) {
        if (s == buf + size) fwrite(buf, 1, size, stdout), s = buf;
        *s++ = c;
    }

    inline void print(const char *s) {
        char *p = s;
        while (*p != '\0') print(*p++);
    }

    template <class T>
    inline void println(T x) {
        print(x), print('\n');
    }

    template <class T>
    inline void print(T x) {
        static int buf[30], cnt;
        if (x == 0) {
            print('0');
        } else {
            if (x < 0) print('-'), x = -x;
            for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
            while (cnt) print((char)buf[cnt--]);
        }
    }

    template <class T1, class T2>
    inline void print(T1 a, T2 b) {
        print(a), print(b);
    }

    template <class T1, class T2, class T3>
    inline void print(T1 a, T2 b, T3 c) {
        print(a), print(b), print(c);
    }

    template <class T1, class T2, class T3, class T4>
    inline void print(T1 a, T2 b, T3 c, T4 d) {
        print(a), print(b), print(c), print(d);
    }

    template <class T1, class T2>
    inline void println(T1 a, T2 b) {
        print(a), println(b);
    }

    template <class T1, class T2, class T3>
    inline void println(T1 a, T2 b, T3 c) {
        print(a), print(b), println(c);
    }

    template <class T1, class T2, class T3, class T4>
    inline void println(T1 a, T2 b, T3 c, T4 d) {
        print(a), print(b), print(c), println(d);
    }

    BufferedOutputStream() : s(buf) {}

    ~BufferedOutputStream() { fwrite(buf, 1, s - buf, stdout); }
};
#ifdef FAST_IO
BufferedInputStream<> in;
BufferedOutputStream<> out;
#endif
}  // namespace IO

namespace MemoryPool {
struct Variable {
    std::vector<int> params;
    std::vector<int> value;

    inline int *find(const std::vector<int> &p) {
#ifdef DBG
        assert(params.size() == p.size());
#endif
        register int size = p.size();
        register int idx = 0;
        for (register int i = 0; i < size; i++) {
#ifdef DBG
            assert(params[i] > p[i]);
#endif
            idx *= params[i];
            idx += p[i];
        }
#ifdef DBG
        assert(idx >= 0 && idx < value.size());
#endif
        return &value[idx];
    }

    inline void set(const std::vector<int> &p, const int value) {
        *find(p) = value;
    }

    Variable() {}

    Variable(const std::vector<int> &vec) : params(vec) {
        register int size = 1;
        for (register int i = 0, r = vec.size(); i < r; i++) size *= vec[i];
        value.resize(size);
    }
};

std::map<std::string, std::stack<Variable> > store;
std::stack<std::vector<std::string> > history;

inline void mark() { history.push(std::vector<std::string>()); }

inline void clear() {
    std::vector<std::string> &v = history.top();
    for (register int i = 0, r = v.size(); i < r; i++) {
#ifdef DBG
        assert(!store[v[i]].empty());
#endif
        store[v[i]].pop();
    }
    history.pop();
}

inline int countParams(const std::string &str) {
#ifdef DBG
    assert(store.count(str));
#endif
    return store[str].top().params.size();
}

inline int *get(const std::string &str, const std::vector<int> ¶ms) {
#ifdef DBG
    assert(store.count(str));
#endif
    return store[str].top().find(params);
}

inline void set(const std::string &str, const std::vector<int> ¶ms,
                const int &value) {
#ifdef DBG
    assert(store.count(str));
#endif
    store[str].top().set(params, value);
}

inline void apply(const std::string &str, const std::vector<int> &vec) {
    store[str].push(Variable(vec));
#ifdef DBG
    assert(!history.empty());
#endif
    history.top().push_back(str);
}

inline int paramsCount(const std::string &str) {
    return store[str].top().params.size();
}
}  // namespace MemoryPool

namespace InputNumber {
int cur;
std::vector<int> list;

inline void init() {
    std::cin >> cur;
    list.resize(cur);
    for (register int i = 0; i < cur; i++) std::cin >> list[i];
    cur = 0;
}

inline int get() {
#ifdef DBG
    assert(cur < list.size());
#endif
    return list[cur++];
}
}  // namespace InputNumber

class CPlusPlusInterpreter {
   public:
    CPlusPlusInterpreter() {
        init();
        funcAndVar();
        runFunction(func["main"].first, std::vector<int>());
    }

   private:
    std::vector<std::string> code;

    inline void init() {
        std::ios::sync_with_stdio(false);
        std::cin.tie(NULL);
        std::cout.tie(NULL);
        InputNumber::init();
        std::string str, src;
        for (register int i = 0; i < 5; i++) std::cin >> str;
        while (std::cin >> str) src += str + '\n';

        register int p = 0, size = src.size();
        while (p < size) {
            while (p < size && isspace(src[p])) p++;
            if (p == size) break;

            std::string s;
            if (isalpha(src[p]) || isdigit(src[p]) || src[p] == '_') {
                for (; p < size &&
                       (isalpha(src[p]) || isdigit(src[p]) || src[p] == '_');
                     p++)
                    s += src[p];
                code.push_back(s);
            } else if (src[p] == '>' && p + 1 < size && src[p + 1] == '>') {
                code.push_back(">>");
                p += 2;
            } else if (src[p] == '<' && p + 1 < size && src[p + 1] == '<') {
                code.push_back("<<");
                p += 2;
            } else if (src[p] == '=' && p + 1 < size && src[p + 1] == '=') {
                code.push_back("==");
                p += 2;
            } else if (src[p] == '!' && p + 1 < size && src[p + 1] == '=') {
                code.push_back("!=");
                p += 2;
            } else if (src[p] == '<' && p + 1 < size && src[p + 1] == '=') {
                code.push_back("<=");
                p += 2;
            } else if (src[p] == '>' && p + 1 < size && src[p + 1] == '=') {
                code.push_back(">=");
                p += 2;
            } else if (src[p] == '&' && p + 1 < size && src[p + 1] == '&') {
                code.push_back("&&");
                p += 2;
            } else if (src[p] == '|' && p + 1 < size && src[p + 1] == '|') {
                code.push_back("||");
                p += 2;
            } else {
                if ((src[p] == '-' || src[p] == '+') &&
                    !isalpha(code.back()[0]) && !isdigit(code.back()[0]) &&
                    !(code.back()[0] == '_') && !(code.back()[0] == ')') &&
                    !(code.back()[0] == ']')) {
                    s = (std::string) "$" + src[p];
                } else {
                    s = src[p];
                }
                code.push_back(s);
                p++;
            }
        }
    }

    inline int matchBrachets(int i) {
        static std::vector<int> rem;
        if (rem.size() == 0) rem = std::vector<int>(code.size(), -1);
        if (rem[i] != -1) return rem[i];
        register int tmpi = i;
        register int count = 0;
#ifdef DBG
        assert(code[i] == "[" || code[i] == "(" || code[i] == "{");
#endif
        std::string left = code[i];
        std::string right = code[i] == "[" ? "]" : code[i] == "(" ? ")" : "}";
        for (;; i++) {
#ifdef DBG
            assert(i < code.size());
#endif
            if (code[i] == left)
                count++;
            else if (code[i] == right)
                count--;
            if (count == 0) return rem[tmpi] = i + 1;
        }
    }

    inline int nextSemicolon(int i) {
        static std::vector<int> rem;
        if (rem.size() == 0) rem = std::vector<int>(code.size(), -1);
        if (rem[i] != -1) return rem[i];
        register int tmpi = i;
        while (code[i] != ";") i++;
        return rem[tmpi] = i + 1;
    }

    inline int nextStatement(int i) {
        static std::vector<int> rem;
        if (rem.size() == 0) rem = std::vector<int>(code.size(), -1);
        if (rem[i] != -1) return rem[i];

        std::string &s = code[i];
        register int ret;
        if (s == "{") {
            ret = matchBrachets(i);
        } else if (s == "for" || s == "while") {
            ret = nextStatement(matchBrachets(i + 1));
        } else if (s == "if") {
            ret = nextStatement(matchBrachets(i + 1));
            if (code[ret] == "else") ret = nextStatement(ret + 1);
        } else {
            ret = nextSemicolon(i);
        }
        return rem[i] = ret;
    }

    std::map<std::string, std::pair<int, int> > func;

    inline bool isFunction(const std::string &str) { return func.count(str); }

    const static int COMMAND_RETURN = 1;

    struct Return {
        int command;
        int ending;
        int retValue;

        Return(int x = -1, int ret = 0)
            : command(0), ending(x), retValue(ret) {}
    };

    const static int STREAM_CIN = 1;
    const static int STREAM_COUT = 2;
    const static int STREAM_ENDL = 3;
    const static int SPECIAL_FUNCTION = -1;

    struct Variable {
        int constant;
        int *variable;
        int stream;

        inline void init() {
            variable = NULL;
            stream = 0;
        }

        Variable() : variable(NULL), stream(0) {}

        Variable(int x) : variable(NULL), stream(0), constant(x) {}

        Variable(int *x) : variable(x), stream(0) {}

        inline int get() {
#ifdef DBG
            assert(stream == 0);
#endif
            if (variable) return *variable;
            return constant;
        }
    };

    inline Return runStatement(int p) {
        if (code[p] == "{") {
            MemoryPool::mark();
            register int pp = matchBrachets(p);
            Return ret(pp);
            p++;
            while (code[p] != "}") {
                Return temp = runStatement(p);
                if (temp.command == COMMAND_RETURN) {
                    temp.ending = pp;
                    MemoryPool::clear();
                    return temp;
                }
                p = temp.ending;
            }
#ifdef DBG
            assert(p + 1 == pp);
#endif
            MemoryPool::clear();
            return ret;
        } else if (code[p] == "int") {
            p++;
            while (true) {
                std::string &name = code[p++];
                std::vector<int> vec;
                while (true) {
                    if (code[p] == ";" || code[p] == ",") break;
#ifdef DBG
                    assert(code[p] == "[");
#endif
                    p++;
                    vec.push_back(atoi(code[p].c_str()));
                    p++;
#ifdef DBG
                    assert(code[p] == "]");
#endif
                    p++;
                }
                MemoryPool::apply(name, vec);
                if (code[p] == ";") break;
                p++;
            }
            return Return(p + 1);
        } else if (code[p] == "if") {
#ifdef DBG
            assert(code[p + 1] == "(");
#endif
            register int limit = matchBrachets(p + 1);
            register int limit2 = nextStatement(limit);
            register int ending = nextStatement(p);
            if (code[limit2] == "else")
                limit2++;
            else
                limit2 = -1;

            register int value = calculator(p + 2, limit - 1).retValue;
            Return ret;
            if (value)
                ret = code[limit] == "int" ? Return() : runStatement(limit);
            else if (limit2 != -1)
                ret = code[limit2] == "int" ? Return() : runStatement(limit2);
            if (ret.command == COMMAND_RETURN) {
                ret.ending = ending;
                return ret;
            }
            return Return(ending);
        } else if (code[p] == "while") {
#ifdef DBG
            assert(code[p + 1] == "(");
#endif
            register int limit = matchBrachets(p + 1);
            register int ending = nextStatement(p);
            while (calculator(p + 2, limit - 1).retValue) {
                Return ret =
                    code[limit] == "int" ? Return() : runStatement(limit);
                if (ret.command == COMMAND_RETURN) {
                    ret.ending = ending;
                    return ret;
                }
            }
            return Return(ending);
        } else if (code[p] == "for") {
#ifdef DBG
            assert(code[p + 1] == "(");
#endif
            register int ending = nextStatement(p);
            register int limit = matchBrachets(p + 1);
            p = calculator(p + 2, -1).ending;
            register int limit2 = nextSemicolon(p);

            while (calculator(p, -1).retValue) {
                Return ret =
                    code[limit] == "int" ? Return() : runStatement(limit);
                if (ret.command == COMMAND_RETURN) {
                    ret.ending = ending;
                    return ret;
                }
                calculator(limit2, limit - 1);
            }
            return Return(ending);
        } else if (code[p] == "return") {
            Return result = calculator(p + 1, -1);
            Return temp = Return(result.ending, result.retValue);
            temp.command = COMMAND_RETURN;
            return temp;
        } else {
            p = calculator(p, -1).ending;
            return Return(p);
        }
#ifdef DBG
        assert(false);
#endif
        return Return();
    }

    inline Return runFunction(int p, const std::vector<int> &vec) {
        if (p == SPECIAL_FUNCTION) {
            std::cout << (char)vec[0];
#ifdef DBG
            assert(vec.size() == 1);
#endif
            return Return(-1, vec[0]);
        }

        p += 3;
        MemoryPool::mark();
        int vecP = 0;
        while (true) {
            if (code[p] == ")") {
                Return ret = runStatement(p + 1);
                MemoryPool::clear();
                return ret;
            }
#ifdef DBG
            assert(code[p] == "int");
#endif
            MemoryPool::apply(code[p + 1], std::vector<int>());
#ifdef DBG
            assert(vecP < vec.size());
#endif
            MemoryPool::set(code[p + 1], std::vector<int>(), vec[vecP++]);
            if (code[p + 2] == ",") {
                p += 3;
            } else {
#ifdef DBG
                assert(code[p + 2] == ")");
#endif
                p += 2;
            }
        }
    }

    inline void funcAndVar() {
        MemoryPool::mark();
        register int size = code.size();
        register int p = 0;
        while (p < size) {
#ifdef DBG
            assert(code[p] == "int");
#endif
            if (code[p + 2] == "(") {
                std::string &name = code[p + 1];
                register int tmpp = p;
                register int paramsCount = 0;

                p += 3;
                for (; code[p] != ")"; p++)
                    if (code[p] == "int") paramsCount++;
                func[name] = std::make_pair(tmpp, paramsCount);
                p = matchBrachets(p + 1);
            } else {
                p = runStatement(p).ending;
            }
        }

        func["putchar"] = std::make_pair(SPECIAL_FUNCTION, 1);
    }

    inline int optLevel(const std::string &s) {
        if (s == "(" || s == "[") return -1000;
        if (s == "!" || s == "$+" || s == "$-") return -2;
        if (s == "*" || s == "/" || s == "%") return -3;
        if (s == "+" || s == "-") return -4;
        if (s == "<=" || s == ">=" || s == "<" || s == ">") return -5;
        if (s == "==" || s == "!=") return -6;
        if (s == "^") return -7;
        if (s == "&&") return -8;
        if (s == "||") return -9;
        if (s == "=") return -10;
        if (s == "<<" || s == ">>") return -11;
        return 0;
    }

    inline bool rightCombine(const std::string &s) {
        if (isalpha(s[0]) || s[0] == '_') return true;
        if (s[0] == '$' || s == "!") return true;
        if (s == "=") return true;
        return false;
    }

    inline int paramsCount(const std::string &s) {
        if (isFunction(s)) return func[s].second;
        return MemoryPool::paramsCount(s);
    }

    inline void calculate(std::stack<std::string> &opt,
                          std::stack<Variable> &var) {
        std::string &o = opt.top();
        if (o == "!" || o == "$-" || o == "$+") {
            register int x = var.top().get();
            var.pop();
            var.push(
                Variable(o == "!" ? (x ? 0 : 1) : o == "$-" ? (-x) : (+x)));
        } else if (o == "*" || o == "/" || o == "%" || o == "+" || o == "-" ||
                   o == "<=" || o == ">=" || o == "<" || o == ">" ||
                   o == "==" || o == "!=" || o == "^" || o == "&&" ||
                   o == "||") {
            register int y = var.top().get();
            var.pop();
            register int x = var.top().get();
            var.pop();
            if (o == "/" || o == "%") assert(y != 0);

            var.push(Variable(
                o == "*"
                    ? (x * y)
                    : o == "/"
                          ? (x / y)
                          : o == "%"
                                ? (x % y)
                                : o == "+"
                                      ? (x + y)
                                      : o == "-"
                                            ? (x - y)
                                            : o == "<="
                                                  ? (x <= y ? 1 : 0)
                                                  : o == ">="
                                                        ? (x >= y ? 1 : 0)
                                                        : o == "<"
                                                              ? (x < y ? 1 : 0)
                                                              : o == ">"
                                                                    ? (x > y
                                                                           ? 1
                                                                           : 0)
                                                                    : o == "=="
                                                                          ? (x == y
                                                                                 ? 1
                                                                                 : 0)
                                                                          : o == "!="
                                                                                ? (x != y
                                                                                       ? 1
                                                                                       : 0)
                                                                                : o == "^"
                                                                                      ? ((x &&
                                                                                          !y) || (!x &&
                                                                                                  y)
                                                                                             ? 1
                                                                                             : 0)
                                                                                      : o == "&&"
                                                                                            ? (x && y
                                                                                                   ? 1
                                                                                                   : 0)
                                                                                            : (x || y
                                                                                                   ? 1
                                                                                                   : 0)));
        } else if (o == ">>") {
            Variable num = var.top();
            var.pop();
#ifdef DBG
            assert(var.top().stream == STREAM_CIN);

            assert(num.variable);
#endif
            *num.variable = InputNumber::get();
        } else if (o == "<<") {
            Variable num = var.top();
            var.pop();
#ifdef DBG
            assert(var.top().stream == STREAM_COUT);
#endif
            if (num.stream == STREAM_ENDL)
                std::cout << std::endl;
            else
                std::cout << num.get();
        } else if (o == "=") {
            register int x = var.top().get();
            var.pop();
            register int *v = var.top().variable;
#ifdef DBG
            assert(v);
#endif
            *v = x;
        } else {
            register int count =
                isFunction(o) ? func[o].second : MemoryPool::paramsCount(o);
            std::vector<int> vec;
            for (register int i = 0; i < count; i++) {
                vec.push_back(var.top().get());
                var.pop();
            }
            std::reverse(vec.begin(), vec.end());
            if (isFunction(o)) {
                register int temp = runFunction(func[o].first, vec).retValue;
                var.push(Variable(temp));
            } else {
                int *temp = MemoryPool::get(o, vec);
                var.push(Variable(temp));
            }
        }

        opt.pop();
    }

    inline Return calculator(int p, int limit) {
        if (code[p] == ";") return Return(p + 1, 1);
        std::stack<std::string> opt;
        std::stack<Variable> var;
        for (; (limit == -1 || p < limit) && code[p] != ";"; p++) {
            if (isdigit(code[p][0])) {
                var.push(Variable(atoi(code[p].c_str())));
            } else if (code[p] == "cin") {
                Variable temp;
                temp.stream = STREAM_CIN;
                var.push(temp);
            } else if (code[p] == "cout") {
                Variable temp;
                temp.stream = STREAM_COUT;
                var.push(temp);
            } else if (code[p] == "endl") {
                Variable temp;
                temp.stream = STREAM_ENDL;
                var.push(temp);
            } else if (code[p] == "[" || code[p] == "(") {
                opt.push(code[p]);
            } else if (code[p] == "]") {
                while (opt.top() != "[") calculate(opt, var);
                opt.pop();
            } else if (code[p] == ")") {
                while (opt.top() != "(") calculate(opt, var);
                opt.pop();
            } else if (code[p] == ",") {
                while (opt.top() != "(") calculate(opt, var);
            } else {
                register int levelCur = optLevel(code[p]);
                while (!opt.empty()) {
                    register int levelTop = optLevel(opt.top());
                    if (levelCur < levelTop ||
                        (levelCur == levelTop && !rightCombine(code[p]))) {
                        calculate(opt, var);
                    } else {
                        break;
                    }
                }
                opt.push(code[p]);
            }
        }
        while (!opt.empty()) calculate(opt, var);
#ifdef DBG
        assert(var.size() == 1);
#endif
        return Return(code[p] == ";" ? p + 1 : p,
                      var.top().stream ? 1 : var.top().get());
    }
} * interpreter;

int main() {
    interpreter = new CPlusPlusInterpreter();
    return 0;
}

#endif
