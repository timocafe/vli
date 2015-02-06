/*
 *Very Large Integer Library, License - Version 1.0 - May 3rd, 2012
 *
 *Timothee Ewart - University of Geneva,
 *Andreas Hehn - Swiss Federal Institute of technology Zurich.
 *
 *Permission is hereby granted, free of charge, to any person or organization
 *obtaining a copy of the software and accompanying documentation covered by
 *this license (the "Software") to use, reproduce, display, distribute,
 *execute, and transmit the Software, and to prepare derivative works of the
 *Software, and to permit third-parties to whom the Software is furnished to
 *do so, all subject to the following:
 *
 *The copyright notices in the Software and this entire statement, including
 *the above license grant, this restriction and the following disclaimer,
 *must be included in all copies of the Software, in whole or in part, and
 *all derivative works of the Software, unless such copies or derivative
 *works are solely in the form of machine-executable object code generated by
 *a source language processor.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 *SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 *FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 *ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *DEALINGS IN THE SOFTWARE.
 */

#include <regression/vli/test_header.hpp>

using namespace vli::test;

VLI_STATIC_TEST( divide_by_zero )
{
    integer_type a;
    integer_type b(0);
    init(a);
    bool ok = false;

    try{
        a /= b;
    } catch (vli::integer_division_by_zero_error& e){
        ok = true;
    }
    BOOST_CHECK_EQUAL(ok, true);

}

VLI_FUZZABLE_TEST( multiply_divide )
{
    integer_type a;
    integer_type b;
    init(a);
    init(b);

    integer_type c(a);

    a /= b;

    a *= b;

    BOOST_CHECK_EQUAL( a <= c, true);
    BOOST_CHECK_EQUAL( a > c-b, true);
}


VLI_FUZZABLE_TEST( divide_assign_divide_equivalence )
{
    integer_type a;
    integer_type b;

    init(a);
    init(b);

    integer_type b_orig(b);

    integer_type ab = a / b;
    a /= b;
    BOOST_CHECK_EQUAL(a,ab);

    //Check that b hasn't changed
    BOOST_CHECK_EQUAL(b,b_orig);
}
