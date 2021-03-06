/*
 *Very Large Integer Library, License - Version 1.0 - May 3rd, 2012
 *
 *Timothee Ewart - University of Geneva,
 *Andreas Hehn - Swiss Federal Institute of technology Zurich.
 *Maxim Milakov - NVIDIA Corporation
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


namespace vli {
    namespace detail  {
                        
    template<class T>
    __device__ inline const T& min(T const& a, const T& b){
       return !(b<a)?a:b; 
    } 

    template<class T>
    __device__ inline const T& max(T const& a, const T& b){
       return !(b>a)?a:b; 
    } 

    template <std::size_t NumBits>
    __device__ inline void multiplies(boost::uint32_t* res, boost::uint32_t* res1, boost::uint32_t* c1, boost::uint32_t* c2);

    } // end namespace detail
} // end namespace vli



