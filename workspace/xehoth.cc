/*******************************************************************************
 * Copyright (c) 2016-2017, xehoth
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name xehoth, nor the names of its contributors may be used
 *       to endorse or promote products derived from this software without
 *       specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY XEHOTH AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEHOTH AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/
#define XEHOTH_H
/**
 *  xehoth.h
 * OI template
 * only support g++
 */

#define DBG
#ifdef OPT
#pragma GCC optimize("Ofast")
#endif

#ifdef ONLINE_JUDGE
#define NDEBUG

#undef DBG

#endif

// stdc++.h
// C
#ifndef _GLIBCXX_NO_ASSERT
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

#if __cplusplus >= 201103L
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

// C++
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

#if __cplusplus >= 201103L
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
// end stdc++.h

// tr1
#ifndef NTR1
#if __cplusplus < 201103L
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
#endif
// end tr1

// ext
#ifndef NEXT
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
// end ext

// tr2
#ifndef NTR2
#if __cplusplus >= 201103L
#include <tr2/bool_set>
#include <tr2/dynamic_bitset>
#include <tr2/ratio>
#include <tr2/type_traits>
#endif
#endif
// endtr2

#ifdef OPT
#define inline inline __attribute__((optimize("Ofast"))) __attribute__( \
    (__always_inline__, __gnu_inline__, __artificial__)) __fastcall
#define assembly __as##m__ __volatile__
#endif

// IO
namespace IO {
#ifndef NFAST_IO

#ifndef IO_BUFFER_SIZE
#define IO_BUFFER_SIZE 1048576

#endif

struct InputOutputStream {
    char ibuf[IO_BUFFER_SIZE], obuf[IO_BUFFER_SIZE], *is, *it, *os;
    int buf[40];

    InputOutputStream() : is(ibuf), it(ibuf), os(obuf) {}

    inline char nextChar() {
        is == it ? it = (is = ibuf) + fread(ibuf, 1, IO_BUFFER_SIZE, stdin) : 0;
        return is == it ? *is++ : -1;
    }

    inline char peek() { return *is; }

    inline char next() { return nextChar(); }

    template <typename T>
    inline bool read(T &x) {
        register char c;
        register bool iosig = false;
        for (c = nextChar(); !isdigit(c); c = nextChar()) {
            if (c == -1) return false;
            c == '-' ? iosig = true : 0;
        }
        for (x = 0; isdigit(c); c = nextChar()) x = x * 10 + (c ^ '0');
        iosig ? x = -x : 0;
        return true;
    }

    inline void read(char &c) {
        while (c = nextChar(), isspace(c) && c != -1)
            ;
    }

    inline int readLine(char *buf) {
        register int s = 0;
        register char c;
        while (c = nextChar(), !isprint(c) && c != -1)
            ;
        if (c == -1) {
            *buf = 0;
            return -1;
        }
        do
            buf[s++] = c;
        while (c = nextChar(), isprint(c) && c != -1);
        buf[s] = 0;
        return s;
    }

    inline int read(char *buf) {
        register int s = 0;
        register char c;
        while (c = nextChar(), isspace(c) && c != -1)
            ;
        if (c == -1) {
            *buf = 0;
            return -1;
        }
        do
            buf[s++] = c;
        while (c = nextChar(), !isspace(c) && c != -1);
        buf[s] = 0;
        return s;
    }

    inline void print(char c) {
        os == obuf + IO_BUFFER_SIZE
            ? (fwrite(obuf, 1, IO_BUFFER_SIZE, stdout), os = obuf)
            : 0;
        *os++ = c;
    }

    template <typename T>
    inline void println(const T &x) {
        print(x), print('\n');
    }

    template <typename T>
    inline void print(T x) {
        register int cnt = 0;
        if (x == 0) {
            print('0');
        } else {
            x < 0 ? (print('-'), x = -x) : 0;
            for (; x; x /= 10) buf[++cnt] = x % 10 | 48;
            while (cnt) print((char)buf[cnt--]);
        }
    }

    inline void print(const char *s) {
        for (; *s; s++) print(*s);
    }

    inline void flush() { fwrite(obuf, 1, os - obuf, stdout), os = obuf; }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }

    ~InputOutputStream() { flush(); }
};
InputOutputStream io;
#undef IO_BUFFER_SIZE

#else
#ifndef FAST_CPP_IO

#else

#endif
#endif

inline void init() {}
}  // end IO

inline void xehothHInit() { IO::init(); }

#ifdef OPT
#undef inline
#undef assembly
#endif

#undef XEHOTH_H
// end xehoth.h

int main() { xehothHInit(); }
