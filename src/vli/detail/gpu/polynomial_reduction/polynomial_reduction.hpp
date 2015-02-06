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
    namespace detail {
    #define VLI_SIZE num_words<NumBits>::value 

    template <std::size_t NumBits, int Order, int NumVars>
    __global__ void
    __launch_bounds__(sum_block_size::value, 2)
    polynomial_sum_intermediate_full(
		    const boost::uint32_t * __restrict__ intermediate,
		    const boost::uint32_t element_count,
		    boost::uint32_t * __restrict__ out,
                    const std::size_t offset, const bool reuse)
    {
	    __shared__ boost::uint32_t buf[sum_block_size::value * size_pad<2*VLI_SIZE>::value]; 

	    boost::uint32_t local_thread_id = threadIdx.x;
	    boost::uint32_t coefficient_id = blockIdx.x + offset;

	    boost::uint32_t * t1 = buf + (local_thread_id * size_pad<2*VLI_SIZE>::value);
            #pragma unroll
	    for(boost::uint32_t i = 0; i < 2*VLI_SIZE; ++i)
		    t1[i] = 0;
//etape 1 tout metre dans la shared
	    const boost::uint32_t * in2 = intermediate + (coefficient_id * element_count *2*VLI_SIZE) + local_thread_id;
	    for(boost::uint32_t element_id = local_thread_id; element_id < element_count; element_id += sum_block_size::value)
	    {
                 asm( "add.cc.u32   %0 , %0 , %1 ; \n\t" : "+r"(t1[0]):"r"(in2[0])); 
                 #pragma unroll
                 for(int i=1; i < 2*VLI_SIZE; ++i)
                     asm( "addc.cc.u32  %0 , %0 , %1 ; \n\t" : "+r"(t1[i]):"r"(in2[i*element_count])); 

     	         in2 += sum_block_size::value;
	    }
//etape 2 la reduction en elle meme
        #pragma unroll
	    for(boost::uint32_t stride = sum_block_size::value >> 1; stride > 0; stride >>= 1) {
                __syncthreads();
                if (local_thread_id < stride) {
                    boost::uint32_t * t2 = buf + (local_thread_id + stride) * size_pad<2*VLI_SIZE>::value;
                    add<2*NumBits>(t1,t2);
                }
	    }

	    if (local_thread_id == 0) {
                boost::uint32_t * out2 = out+(coefficient_id*2*VLI_SIZE); 

                //final addition if the pbs does not fit into the mem, else it will sum 0
            if(reuse == true){
                 asm( "add.cc.u32   %0 , %0 , %1 ; \n\t" : "+r"(out2[0]):"r"(t1[0])); 
                #pragma unroll
                for(int i=1; i < 2*VLI_SIZE; ++i)
                    asm( "addc.cc.u32  %0 , %0 , %1 ; \n\t" : "+r"(out2[i]):"r"(t1[i])); 
            }else{
                #pragma unroll
		for(boost::uint32_t i=0; i<2*VLI_SIZE; ++i)
		    out2[i] = buf[i];
            }

	    }
    }

    #undef VLI_SIZE
    }
}
