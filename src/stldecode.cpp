/*
 * Copyright (c) 2012 Thomas Kemmer <tkemmer@computer.org>
 *
 * http://code.google.com/p/stlencoders/
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stlencoders/base16.hpp>
#include <stlencoders/base32.hpp>
#include <stlencoders/base64.hpp>

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <locale>
#include <string>

#ifdef STDCXX_TR1_HEADERS
# include <tr1/functional>
#endif

#if defined(HAVE_GETOPT_H)
# include <getopt.h>
#elif defined(HAVE_UNISTD_H)
# include <unistd.h>
#else
# include "getopt.hpp"
#endif

template<bool F>
struct predicate {
    template<class T> bool operator()(const T&) { return F; }
};

template<class Codec, class OutputIterator, class Predicate>
OutputIterator decode(std::istream& is, OutputIterator out, Predicate skip)
{
    std::istreambuf_iterator<char> in(is);
    std::istreambuf_iterator<char> end;
    return Codec::decode(in, end, out, skip);
}

template<class Codec, class OutputIterator, class Predicate>
OutputIterator fdecode(const char* filename, OutputIterator out,
                       Predicate skip)
{
    if (std::strcmp(filename, "-") == 0) {
        return decode<Codec>(std::cin, out, skip);
    }

    std::ifstream is(filename);
    if (!is) {
        throw std::runtime_error("cannot open file");
    }
    return decode<Codec>(is, out, skip);
}

template<class OutputIterator, class Predicate>
OutputIterator decode(const std::string& codec, const char* filename,
                      OutputIterator out, Predicate skip)
{
    using namespace stlencoders;

    if (codec == "base16") {
        typedef base16_traits<char> traits;
        return fdecode<base16<char, traits> >(filename, out, skip);
    } else if (codec == "base32") {
        typedef base32_traits<char> traits;
        return fdecode<base32<char, traits> >(filename, out, skip);
    } else if (codec == "base32hex") {
        typedef base32hex_traits<char> traits;
        return fdecode<base32<char, traits> >(filename, out, skip);
    } else if (codec == "base64") {
        typedef base64_traits<char> traits;
        return fdecode<base64<char, traits> >(filename, out, skip);
    } else if (codec == "base64url") {
        typedef base64url_traits<char> traits;
        return fdecode<base64<char, traits> >(filename, out, skip);
    } else {
        throw std::runtime_error("unknown encoding: '" + codec + "'");
    }
}

void usage(std::ostream& os, const char* progname)
{
    os << "Usage: " << progname << " [OPTION]... [FILE]\n"
       << "Decode FILE, or standard input, to standard output.\n"
       << "\n"
       << "  -c NAME    input encoding (default 'base64')\n"
       << "  -i         ignore non-alphabet characters\n"
       << "  -l         list supported encoding schemes\n"
       << "  -s         do not skip whitespace in input\n"
       << "\n"
       << "With no FILE, or when FILE is -, read standard input.\n";
}

int main(int argc, char* argv[])
{
    std::string codec = "base64";
    bool skipall = false;
    bool noskip = false;

    for (int c; (c = getopt(argc, argv, ":c:ils")) != -1; ) {
        switch (c) {
        case 'c':
            codec = optarg;
            break;

        case 'i':
            skipall = true;
            break;

        case 'l':
            std::cout << "base16 base32 base32hex base64 base64url\n";
            return EXIT_SUCCESS;

        case 's':
            noskip = true;
            break;

        default:
            usage(std::cout, argv[0]);
            return EXIT_FAILURE;
        }
    }

    const char* filename = optind != argc ? argv[optind] : "-";

    try {
        std::ostreambuf_iterator<char> out(std::cout);

        if (noskip) {
            decode(codec, filename, out, predicate<false>());
        } else if (skipall) {
            decode(codec, filename, out, predicate<true>());
        } else {
#ifdef STDCXX_TR1_HEADERS
            using namespace std::tr1::placeholders;
            decode(codec, filename, out,
                   std::tr1::bind(std::isspace<char>, _1, std::locale()));
#else
            decode(codec, filename, out,
                   std::bind2nd(std::ptr_fun(std::isspace<char>), std::locale()));

#endif
        }
    } catch (std::exception& e) {
        std::cerr << argv[0] << ": " << filename << ": " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
