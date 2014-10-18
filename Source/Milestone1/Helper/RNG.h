#if !defined FACTORY_RNG_INCLUDED
#define FACTOR_RNG_INCLUDED

#include <random>           // Required for everything!
#include <type_traits>      // Required for integral/floating point difference


template <typename T, bool = std::is_integral<T>::value>
/// <summary>
/// A class dedicated to the encapsulation of the C++ random generator and distribution. A convenience class more than anything.
/// </summary>
class RNG final
{
};


template <typename T>
/// <summary>
/// A class dedicated to the encapsulation of the C++ random generator and distribution. A convenience class more than anything.
/// </summary>
class RNG<T, true> final
{
    public:
        /// <summary> Sets up the required values of the RNG with an optional seed parameter. </summary>
        RNG (const T min, const T max, const unsigned int seed = 0)
        {
            m_distribution = std::uniform_int_distribution<T> (min, max);        
            m_generator.seed (seed);
        }

        RNG (const RNG& copy)                       = default;
        RNG<T, true>& operator= (const RNG& copy)   = default;

        ~RNG()                                      = default;
        

        /// <summary> Retrieves the next random number from the sequence. </summary>
        const T getRandom()                         { return m_distribution (m_generator); }

        const T getMin() const                      { return m_distribution.min(); }
        const T getMax() const                      { return m_distribution.max(); }


        /// <summary> Allows for the generator to be reseeded at will for different numbers. </summary>
        void reseed (const unsigned int seed)       { m_generator.seed (seed); }

        /// <summary> Changes the distribution used by the generator. </summary>
        void setDistribution (const std::uniform_int_distribution<T>& distribution)
        {
            m_distribution = distribution; 
        }

        /// <summary> Resets the distribution so that it doesn't rely on previous values. </summary>
        void resetDistribution()                    { m_distribution.reset(); }

    private:
        std::default_random_engine          m_generator;    //!< The random engine used for generating numbers.
        std::uniform_int_distribution<T>    m_distribution; //!< The distribution used for producing random integral numbers.
};


template <typename T>
/// <summary>
/// A class dedicated to the encapsulation of the C++ random generator and distribution. A convenience class more than anything.
/// </summary>
class RNG<T, false> final
{
    public:
        /// <summary> Sets up the required values of the RNG with an optional seed parameter. </summary>
        RNG (const T min, const T max, const unsigned int seed = 0)
        {
            m_distribution = std::uniform_real_distribution<T> (min, max);        
            m_generator.seed (seed);
        }

        RNG (const RNG& copy)                       = default;
        RNG<T, false>& operator= (const RNG& copy)  = default;

        ~RNG()                                      = default;
        

        /// <summary> Retrieves the next random number from the sequence. </summary>
        const T getRandom()                         { return m_distribution (m_generator); }

        const T getMin() const                      { return m_distribution.min(); }
        const T getMax() const                      { return m_distribution.max(); }


        /// <summary> Allows for the generator to be reseeded at will for different numbers. </summary>
        void reseed (const unsigned int seed)       { m_generator.seed (seed); }

        /// <summary> Changes the distribution used by the generator. </summary>
        void setDistribution (const std::uniform_real_distribution<T>& distribution)
        {
            m_distribution = distribution; 
        }

        /// <summary> Resets the distribution so that it doesn't rely on previous values. </summary>
        void resetDistribution()                    { m_distribution.reset(); }

    private:
        std::default_random_engine          m_generator;    //!< The random engine used for generating numbers.
        std::uniform_real_distribution<T>   m_distribution; //!< The distribution used for producing random integral numbers.
};


#endif // FACTORY_RNG_INCLUDED