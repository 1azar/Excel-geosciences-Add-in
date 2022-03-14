#pragma once
#include "ap.h"
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "dataanalysis.h"
#include <array>
#include <exception>
#include "interpolation.h"


class math_utilits {
public:

	static std::vector<double> ngk_normalization(); //<-do

	static void pr_coefficients(const std::vector<double>& y, const std::vector<double>& x, double& a, double& b, int& info);

	static std::vector<double> Linear_interp(const double& f_v, const double& s_v, const int& num_p);

	static std::vector<double> sintez_curve_pow(const std::vector<double>& x, const double& a, const double& b);

	static double rmse(const std::vector<double>& vect_a, const std::vector<double>& vect_b);

private:

	static alglib::real_2d_array two_dvector_to_real_2d_array(const std::vector<double>& y_vector, const std::vector<double>& x_vector);
	//static std::vector<float> get_values_from_line_of_las_log_data_section(const std::string& line);

};
