#include <random>


namespace tools{

    std::mt19937 rng;

    template <typename vli>
    typename vli::value_type rnd_digit(){
        static std::uniform_int_distribution<typename vli::value_type>  rnd(0,std::numeric_limits<typename vli::value_type>::max());
        return rnd(rng);
    }

    template <typename vli>
    int rnd_valid_int(){
        static std::uniform_int_distribution<int> rnd(0,std::abs(static_cast<int>(std::numeric_limits<typename vli::value_type>::max())));
        return rnd(rng);
    }
   
    template <typename vli>
    void vli_negate(vli& v, int random=vli::numwords){
        if(v[0]%random == 0)
            v.negate();
    }
   
    template <typename vli>
    void fill_random(vli& v){
        for(typename vli::size_type i=0; i < vli::numwords; ++i)
            v[i] = rnd_digit<vli>(); 
    }
   
    template <typename vli>
    void fill_random(vli& v, typename vli::size_type size){
        assert(size <= vli::size);
        for(typename vli::size_type i=0; i < size; ++i)
            v[i] = rnd_digit<vli>();
    }

    template <typename Polynomial>
    void fill_poly_random(Polynomial& p){
        for(typename Polynomial::iterator it= p.begin(); it != p.end(); ++it){
           fill_random(*it);
           vli_negate(*it);
        }
    } 
   
    template <class Vector>
    void fill_vector_random(Vector& v){
        for(typename Vector::size_type i=0; i < v.size(); ++i)
           fill_poly_random(v[i]);
    }

 }
