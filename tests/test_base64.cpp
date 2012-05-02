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

#include <stlencoders/base64.hpp>

#include <cassert>
#include <cstdlib>
#include <string>

#include "util.hpp"

int main()
{
    typedef stlencoders::base64<char> base64;
    typedef stlencoders::base64<wchar_t> wbase64;

    typedef stlencoders::base64<char, stlencoders::base64url_traits<char> > base64url;
    typedef stlencoders::base64<wchar_t, stlencoders::base64url_traits<wchar_t> > wbase64url;

    // RFC 4648 test vectors

    assert(strenc<base64>("") == "");
    assert(strenc<base64>("f") == "Zg==");
    assert(strenc<base64>("fo") == "Zm8=");
    assert(strenc<base64>("foo") == "Zm9v");
    assert(strenc<base64>("foob") == "Zm9vYg==");
    assert(strenc<base64>("fooba") == "Zm9vYmE=");
    assert(strenc<base64>("foobar") == "Zm9vYmFy");

    assert(strdec<base64>("") == "");
    assert(strdec<base64>("Zg==") == "f");
    assert(strdec<base64>("Zm8=") == "fo");
    assert(strdec<base64>("Zm9v") == "foo");
    assert(strdec<base64>("Zm9vYg==") == "foob");
    assert(strdec<base64>("Zm9vYmE=") == "fooba");
    assert(strdec<base64>("Zm9vYmFy") == "foobar");

    assert(strenc<wbase64>("") == L"");
    assert(strenc<wbase64>("f") == L"Zg==");
    assert(strenc<wbase64>("fo") == L"Zm8=");
    assert(strenc<wbase64>("foo") == L"Zm9v");
    assert(strenc<wbase64>("foob") == L"Zm9vYg==");
    assert(strenc<wbase64>("fooba") == L"Zm9vYmE=");
    assert(strenc<wbase64>("foobar") == L"Zm9vYmFy");

    assert(strdec<wbase64>(L"") == "");
    assert(strdec<wbase64>(L"Zg==") == "f");
    assert(strdec<wbase64>(L"Zm8=") == "fo");
    assert(strdec<wbase64>(L"Zm9v") == "foo");
    assert(strdec<wbase64>(L"Zm9vYg==") == "foob");
    assert(strdec<wbase64>(L"Zm9vYmE=") == "fooba");
    assert(strdec<wbase64>(L"Zm9vYmFy") == "foobar");

    // test some special bit patterns

    assert(strenc<base64>(std::string(3, '\x00')) == "AAAA");
    assert(strdec<base64>("AAAA") == std::string(3, '\x00'));
    assert(strenc<base64>(std::string(3, '\xff')) == "////");
    assert(strdec<base64>("////") == std::string(3, '\xff'));

    assert(strenc<wbase64>(std::string(3, '\x00')) == L"AAAA");
    assert(strdec<wbase64>(L"AAAA") == std::string(3, '\x00'));
    assert(strenc<wbase64>(std::string(3, '\xff')) == L"////");
    assert(strdec<wbase64>(L"////") == std::string(3, '\xff'));

    assert(strenc<base64url>(std::string(3, '\x00')) == "AAAA");
    assert(strdec<base64url>("AAAA") == std::string(3, '\x00'));
    assert(strenc<base64url>(std::string(3, '\xff')) == "____");
    assert(strdec<base64url>("____") == std::string(3, '\xff'));

    assert(strenc<wbase64url>(std::string(3, '\x00')) == L"AAAA");
    assert(strdec<wbase64url>(L"AAAA") == std::string(3, '\x00'));
    assert(strenc<wbase64url>(std::string(3, '\xff')) == L"____");
    assert(strdec<wbase64url>(L"____") == std::string(3, '\xff'));

    // error handling

    assert_throw(strdec<base64>("?==="), stlencoders::invalid_character);
    assert_throw(strdec<base64>("A?=="), stlencoders::invalid_character);
    assert_throw(strdec<base64>("AA?="), stlencoders::invalid_character);
    assert_throw(strdec<base64>("AAA?"), stlencoders::invalid_character);

    assert_throw(strdec<base64>("A==="), stlencoders::invalid_length);
    assert_throw(strdec<base64>("A"), stlencoders::invalid_length);

    return EXIT_SUCCESS;
}
