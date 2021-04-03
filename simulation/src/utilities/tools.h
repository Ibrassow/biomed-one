
#ifndef TOOLS_H
#define TOOLS_H

#include "../typevector.h"


/*generates an array with first element = start, last element = stop, with increment = step*/
template <typename T>
std::vector<T> arange(T start, T stop, T step = 1);


/*return a sliced array*/
template <typename T>
std::vector<T> slicing(std::vector<T>&, int, int);

/*generates an array of "n" random double between "start" and "end"*/
vec1D random_nb_array(double start, double end, int n);

/*Compute the standard deviation of an array*/
double standard_deviation(vec1D arr);

//returns an array containing the interpolated points given in arguments
vec1D l_interp(vec1D& x, vec1D& xp, vec1D& fp); //in ecgsyn

//Resample multidimensional vectors
vec2D resample(vec2D vec, int res_factor);

template<typename T>
std::vector<T> arange(T start, T stop, T step) {
    std::vector<T> values;
    for (T value = start; value < stop; value += step)
        values.push_back(value);
    return values;
}

template<typename T>
std::vector<T> slicing(std::vector<T>& arr, int X, int Y)
{
    // Starting and Ending iterators 
    auto start = arr.begin() + X;
    auto end = arr.begin() + Y + 1;

    // To store the sliced vector 
    std::vector<T> result(Y - X + 1);

    // Copy vector using copy function() 
    copy(start, end, result.begin());

    // Return the final sliced vector 
    return result;
}

#endif