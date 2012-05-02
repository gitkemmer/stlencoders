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

#include <stlencoders/base32.hpp>

#include <cassert>
#include <cstdlib>
#include <string>

#include "util.hpp"

int main()
{
    typedef stlencoders::base32<char> base32;
    typedef stlencoders::base32<wchar_t> wbase32;

    typedef stlencoders::base32<char, stlencoders::base32hex_traits<char> > base32hex;
    typedef stlencoders::base32<wchar_t, stlencoders::base32hex_traits<wchar_t> > wbase32hex;

    // RFC 4648 test vectors

    assert(strenc<base32>("") == "");
    assert(strenc<base32>("f") == "MY======");
    assert(strenc<base32>("fo") == "MZXQ====");
    assert(strenc<base32>("foo") == "MZXW6===");
    assert(strenc<base32>("foob") == "MZXW6YQ=");
    assert(strenc<base32>("fooba") == "MZXW6YTB");
    assert(strenc<base32>("foobar") == "MZXW6YTBOI======");

    assert(strdec<base32>("") == "");
    assert(strdec<base32>("MY======") == "f");
    assert(strdec<base32>("MZXQ====") == "fo");
    assert(strdec<base32>("MZXW6===") == "foo");
    assert(strdec<base32>("MZXW6YQ=") == "foob");
    assert(strdec<base32>("MZXW6YTB") == "fooba");
    assert(strdec<base32>("MZXW6YTBOI======") == "foobar");

    assert(strenc<wbase32>("") == L"");
    assert(strenc<wbase32>("f") == L"MY======");
    assert(strenc<wbase32>("fo") == L"MZXQ====");
    assert(strenc<wbase32>("foo") == L"MZXW6===");
    assert(strenc<wbase32>("foob") == L"MZXW6YQ=");
    assert(strenc<wbase32>("fooba") == L"MZXW6YTB");
    assert(strenc<wbase32>("foobar") == L"MZXW6YTBOI======");

    assert(strdec<wbase32>(L"") == "");
    assert(strdec<wbase32>(L"MY======") == "f");
    assert(strdec<wbase32>(L"MZXQ====") == "fo");
    assert(strdec<wbase32>(L"MZXW6===") == "foo");
    assert(strdec<wbase32>(L"MZXW6YQ=") == "foob");
    assert(strdec<wbase32>(L"MZXW6YTB") == "fooba");
    assert(strdec<wbase32>(L"MZXW6YTBOI======") == "foobar");

    assert(strenc<base32hex>("") == "");
    assert(strenc<base32hex>("f") == "CO======");
    assert(strenc<base32hex>("fo") == "CPNG====");
    assert(strenc<base32hex>("foo") == "CPNMU===");
    assert(strenc<base32hex>("foob") == "CPNMUOG=");
    assert(strenc<base32hex>("fooba") == "CPNMUOJ1");
    assert(strenc<base32hex>("foobar") == "CPNMUOJ1E8======");

    assert(strdec<base32hex>("") == "");
    assert(strdec<base32hex>("CO======") == "f");
    assert(strdec<base32hex>("CPNG====") == "fo");
    assert(strdec<base32hex>("CPNMU===") == "foo");
    assert(strdec<base32hex>("CPNMUOG=") == "foob");
    assert(strdec<base32hex>("CPNMUOJ1") == "fooba");
    assert(strdec<base32hex>("CPNMUOJ1E8======") == "foobar");

    // test some special bit patterns

    assert(strenc<base32>(std::string(5, '\x00')) == "AAAAAAAA");
    assert(strdec<base32>("AAAAAAAA") == std::string(5, '\x00'));
    assert(strenc<base32>(std::string(5, '\xff')) == "77777777");
    assert(strdec<base32>("77777777") == std::string(5, '\xff'));

    assert(strenc<wbase32>(std::string(5, '\x00')) == L"AAAAAAAA");
    assert(strdec<wbase32>(L"AAAAAAAA") == std::string(5, '\x00'));
    assert(strenc<wbase32>(std::string(5, '\xff')) == L"77777777");
    assert(strdec<wbase32>(L"77777777") == std::string(5, '\xff'));

    assert(strenc<base32hex>(std::string(5, '\x00')) == "00000000");
    assert(strdec<base32hex>("00000000") == std::string(5, '\x00'));
    assert(strenc<base32hex>(std::string(5, '\xff')) == "VVVVVVVV");
    assert(strdec<base32hex>("VVVVVVVV") == std::string(5, '\xff'));

    assert(strenc<wbase32hex>(std::string(5, '\x00')) == L"00000000");
    assert(strdec<wbase32hex>(L"00000000") == std::string(5, '\x00'));
    assert(strenc<wbase32hex>(std::string(5, '\xff')) == L"VVVVVVVV");
    assert(strdec<wbase32hex>(L"VVVVVVVV") == std::string(5, '\xff'));

    // error handling

    assert_throw(strdec<base32>("?======="), stlencoders::invalid_character);
    assert_throw(strdec<base32>("A?======"), stlencoders::invalid_character);
    assert_throw(strdec<base32>("AA?====="), stlencoders::invalid_character);
    assert_throw(strdec<base32>("AAA?===="), stlencoders::invalid_character);
    assert_throw(strdec<base32>("AAAA?==="), stlencoders::invalid_character);
    assert_throw(strdec<base32>("AAAAA?=="), stlencoders::invalid_character);
    assert_throw(strdec<base32>("AAAAAA?="), stlencoders::invalid_character);
    assert_throw(strdec<base32>("AAAAAAA?"), stlencoders::invalid_character);

    assert_throw(strdec<base32>("A======="), stlencoders::invalid_length);
    assert_throw(strdec<base32>("AAA====="), stlencoders::invalid_length);
    assert_throw(strdec<base32>("AAAAAA=="), stlencoders::invalid_length);

    assert_throw(strdec<base32>("A"), stlencoders::invalid_length);
    assert_throw(strdec<base32>("AAA"), stlencoders::invalid_length);
    assert_throw(strdec<base32>("AAAAAA"), stlencoders::invalid_length);

    return EXIT_SUCCESS;
}
