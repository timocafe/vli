/*
 *Very Large Integer Library, License - Version 1.0 - May 3rd, 2012
 *
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

#ifndef VLI_POLYNOMIAL_NUMERIC_HPP
#define VLI_POLYNOMIAL_NUMERIC_HPP

namespace vli {

/* \cond I do not need this part in the doc*/

/*
  The following templates are used in the polynomial (and elsewhere)
  and can be specialized or overloaded for types to provide an optimized
  implementation.
*/

template <typename T>
bool is_zero(T t) {
    return t == 0;
}

template <typename T>
void negate_inplace(T& t) {
    t = -t;
}

template <typename T, typename T2, typename T3>
void multiply_add(T& t, T2 const& t2, T3 const& t3) {
    t += t2 * t3;
}

/* \endcond I do not need this part in the doc*/

} // end namespace vli

#endif //VLI_POLYNOMIAL_NUMERIC_HPP
