#pragma once

#include "../typevector.h"



const double PI = 3.141592653589793238460;

namespace fft {

	/*Vector adaptation of the fast Fourier Transform code from Rosetta Code based on Cooley-Tukey algorithm*/
	void fft(C_Vec& x);

	void ifft(C_Vec& x);



}