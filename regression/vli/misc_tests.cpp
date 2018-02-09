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

VLI_STATIC_TEST(pointer_range_overflow) {
  integer_type a;
  init(a);
  integer_type b(a);
  integer_type a_orig(a);

  b -= integer_type(1);

  BOOST_CHECK_EQUAL(b, a - integer_type(1));

  integer_type *c = new integer_type[3];
  c[0] = integer_type(0);
  c[1] = a;
  c[2] = integer_type(0);

  c[1] *= b;

  a *= b;

  BOOST_CHECK_EQUAL(c[0], integer_type(0));
  BOOST_CHECK_EQUAL(c[1], a);
  BOOST_CHECK_EQUAL(c[2], integer_type(0));

  delete[] c;
}

VLI_STATIC_TEST(multiplies_by_two_not_equal_minus_assign_one) {
  integer_type a;
  for (std::size_t i = 0; i < integer_type::numwords; ++i)
    a[i] = std::numeric_limits<integer_type::value_type>::max();

  integer_type c(a);
  integer_type b(2);

  a *= b;
  c -= 1;

  BOOST_CHECK_EQUAL((a == c), true);
}
