#if !defined MATHS_UTILITY_INCLUDED
#define MATHS_UTILITY_INCLUDED


/// <summary> Returns the minimum value, parameters are passed by value. </summary>
template <typename T> T min (const T a, const T b)
{
    return a < b ? a : b;
}


/// <summary> Returns the maximum value, parameters are passed by value. </summary>
template <typename T> T max (const T a, const T b)
{
    return a > b ? a : b;
}


/// <summary> Returns the minimum value, parameters are passed by reference. </summary>
template <typename T> T& minR (const T& a, const T& b)
{
    return a < b ? a : b;
}


/// <summary> Returns the maximum value, parameters are passed by reference. </summary>
template <typename T> T& maxR (const T& a, const T& b)
{
    return a > b ? a : b;
}


#endif // MATHS_UTILITY_INCLUDED