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

#ifndef KERNELS_CPU_GENERIC_ADD_ASM_H
#define KERNELS_CPU_GENERIC_ADD_ASM_H
/* \cond I do not need this part in the doc*/

namespace vli{
    namespace detail{
                    template<std::size_t NumWords>
                    struct helper_inline_add{
                        static void inline inline_add(uint64_t* x, uint64_t const* y){
                           boost::uint32_t const* cy = const_cast<boost::uint32_t*>(reinterpret_cast<boost::uint32_t const*>(y));
                           boost::uint32_t* cx = reinterpret_cast<boost::uint32_t*>(x);
                           boost::uint32_t carry(0);
                           for(int i(0); i<2*NumWords;++i){
                              uint64_t tmp = static_cast<uint64_t>(cx[i]) + cy[i] + carry;
                              cx[i] = tmp;
                              carry = tmp >> (std::numeric_limits<boost::uint32_t>::digits);
                           }
                        }
                        
                        static void inline inline_add(uint64_t* x, uint64_t const b){
                            boost::uint32_t cb = static_cast<boost::uint32_t>(b);
                            boost::uint32_t* cx = reinterpret_cast<boost::uint32_t*>(x);
                            boost::uint32_t sign = cb >>(std::numeric_limits<boost::uint32_t>::digits - 1);
                            sign = -sign;
                            boost::uint32_t carry(0);
                            uint64_t tmp = static_cast<uint64_t>(cx[0]) + cb;
                            cx[0] = tmp;
                            carry = tmp >> (std::numeric_limits<boost::uint32_t>::digits);
                            for(int i(1); i<2*NumWords;++i){
                               uint64_t tmp = static_cast<uint64_t>(cx[i]) + sign + carry;
                               cx[i] = tmp;
                               carry = tmp >> (std::numeric_limits<boost::uint32_t>::digits);
                            }
                        }
                        
                        static void inline_add_extend(uint64_t* x, uint64_t const* y, uint64_t const* w){ 
                             boost::uint32_t const* cw = const_cast<boost::uint32_t*>(reinterpret_cast<boost::uint32_t const*>(w));
                             boost::uint32_t const* cy = const_cast<boost::uint32_t*>(reinterpret_cast<boost::uint32_t const*>(y));
                             boost::uint32_t* cx = reinterpret_cast<boost::uint32_t*>(x);
                             boost::uint32_t sign_y = - (cy[2*(NumWords)-1] >>( std::numeric_limits<boost::uint32_t>::digits - 1));
                             boost::uint32_t sign_z = - (cw[2*(NumWords)-1] >>( std::numeric_limits<boost::uint32_t>::digits - 1));
                             boost::uint32_t carry(0);
                             for(int i(0); i<2*(NumWords);++i){
                                  uint64_t tmp = static_cast<uint64_t>(cw[i]) + cy[i] + carry;
                                  cx[i] = tmp;
                                  carry = tmp >> (std::numeric_limits<boost::uint32_t>::digits);
                             }
                             uint64_t tmp =   static_cast<uint64_t>(sign_y) + sign_z + carry;
                             cx[2*(NumWords)] = tmp;
                             carry = tmp >> (std::numeric_limits<boost::uint32_t>::digits);
                             tmp =   static_cast<uint64_t>(sign_y) + sign_z + carry;
                             cx[2*(NumWords)+1] = tmp;
                        }
                    };
    } // end namespace detail
} // end namespace vli

/* \endcond I do not need this part in the doc*/
#endif
