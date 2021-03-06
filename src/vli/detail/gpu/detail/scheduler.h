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

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "boost/tuple/tuple.hpp"

/* \cond I do not need this part in the doc*/

namespace vli {
    namespace detail {

    class scheduler{
        public:
        scheduler(){
            gpu::cu_check_error(cudaMemGetInfo(&free_memory_card, &total_memory_card),__FILE__,__LINE__);
            total_memory_card = total_memory_card*0.9;// save 10% for the tasklistis, swap, and the final result
            tupple_data = boost::make_tuple(0,0,0);
        }

        std::size_t get_free_memory_card(){
            return free_memory_card;
        }

        std::size_t get_total_memory_card(){
            return total_memory_card;
        }

        boost::tuple<std::size_t, std::size_t, std::size_t>& set_tupple_data(){
            return tupple_data;
        }

        boost::tuple<std::size_t, std::size_t, std::size_t> const& get_tupple_data() const{
            return tupple_data;
        }

        void print(){
            std::cout << " --------- BEGIN DEBUG INFOS ----------------------------------" << std::endl;
            std::cout << " free_memory_card " << free_memory_card << " total_memory_card " << total_memory_card << std::endl;
            std::cout << " tupple 0 " << boost::get<0>(tupple_data) << std::endl;
            std::cout << " tupple 1 " << boost::get<1>(tupple_data) << std::endl;
            std::cout << " tupple 2 " << boost::get<2>(tupple_data) << std::endl;
            std::cout << " --------- END DEBUG INFOS ----------------------------------" << std::endl;
        }

        template <class function>
        void execute(function f, boost::uint32_t const* A, boost::uint32_t const* B, std::size_t full_value){
             f(boost::get<0>(tupple_data),A,B,false);
             std::size_t offset;
             for(std::size_t i(1); i<boost::get<1>(tupple_data); ++i){
                offset = boost::get<0>(tupple_data)*i*full_value;
                f(boost::get<0>(tupple_data),&A[offset],&B[offset],true);
             }
             if(boost::get<2>(tupple_data)!=0){
                offset = boost::get<0>(tupple_data)*boost::get<1>(tupple_data)*full_value;
                f(boost::get<2>(tupple_data),&A[offset],&B[offset],true);
             }
        }

        private:
        boost::tuple<std::size_t, std::size_t, std::size_t> tupple_data; // first chunk, number of chunk-1, last chunk. 1st chunk != last chunk
        std::size_t free_memory_card;
        std::size_t total_memory_card;
    };

    template <std::size_t NumBits, class MaxOrder, int NumVars>
    struct scheduler_helper{
    };

    template <std::size_t NumBits, int Order, int NumVars>
    struct scheduler_helper<NumBits, max_order_each<Order>, NumVars>{
        static void  determine_memory(scheduler& sch, const std::size_t vector_size){
             std::size_t single_size = 2 * num_words<NumBits>::value * stride<0,NumVars,Order>::value * stride<1,NumVars,Order>::value * stride<2,NumVars,Order>::value * stride<3,NumVars,Order>::value*sizeof(boost::uint32_t); // both inputs
             single_size += 2*num_words<NumBits>::value * result_stride<0, NumVars, Order>::value * result_stride<1, NumVars, Order>::value * result_stride<2, NumVars, Order>::value * result_stride<3, NumVars, Order>::value * sizeof(boost::uint32_t); // intermediate buffer
             std::size_t req_size(single_size * vector_size);
             std::size_t data_tupple0,data_tupple1;
             if(req_size < sch.get_total_memory_card() ){
                 data_tupple0 = vector_size > 16384 ? 16384 : vector_size;
                 data_tupple1 = (vector_size)/(data_tupple0);
             }else{
                 data_tupple0 = (sch.get_total_memory_card()/(single_size) > 16384 ) ? 16384  :sch.get_total_memory_card()/(single_size); // 65535 is maximum number of block, but 16384 is an optimum value
                 data_tupple1 = req_size/(data_tupple0*single_size);
             }
             boost::get<0>(sch.set_tupple_data()) = data_tupple0;
             boost::get<1>(sch.set_tupple_data()) = data_tupple1;
             boost::get<2>(sch.set_tupple_data()) = vector_size - data_tupple0*data_tupple1;
        }
    };

    template <std::size_t NumBits, int Order, int NumVars>
    struct scheduler_helper<NumBits, max_order_combined<Order>, NumVars>{
        static void  determine_memory(scheduler& sch, const std::size_t vector_size){
             std::size_t single_size =  2 * num_words<NumBits>::value * vli::detail::max_order_combined_helpers::size<NumVars+1, Order>::value*sizeof(boost::uint32_t); // both input
             single_size += 2*num_words<NumBits>::value * vli::detail::max_order_combined_helpers::size<NumVars+1, 2*Order>::value*sizeof(boost::uint32_t);

             std::size_t req_size(single_size * vector_size);
             std::size_t data_tupple0,data_tupple1;
             if(req_size < sch.get_total_memory_card() ){
                 data_tupple0 = vector_size > 16384 ? 16384 : vector_size;
                 data_tupple1 = (vector_size)/(data_tupple0);
             }else{
                 data_tupple0 = (sch.get_total_memory_card()/(single_size) > 16384 ) ? 16384  :sch.get_total_memory_card()/(single_size); // 65535 is maximum number of block, but 16384 is an optimum value
                 data_tupple1 = req_size/(data_tupple0*single_size);
             }
             boost::get<0>(sch.set_tupple_data()) = data_tupple0;
             boost::get<1>(sch.set_tupple_data()) = data_tupple1;
             boost::get<2>(sch.set_tupple_data()) = vector_size - data_tupple0*data_tupple1;
        }
    };

    } //end namespace detail
} //end namespce vli

/* \endcond I do not need this part in the doc*/


#endif
