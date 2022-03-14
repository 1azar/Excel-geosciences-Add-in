#pragma once
#include "pch.h"
#include "math_utilits.h"

/*************************************************************************
function for ngk normalization
*************************************************************************/
static std::vector<double> ngk_normalization()
{

}

/*************************************************************************
the function creates a curve: result = a*x^b
*************************************************************************/
std::vector<double> math_utilits::sintez_curve_pow(const std::vector<double>& x, const double& a, const double& b)
{
	std::vector<double> result;
	for (const double& i : x)
	{
		result.push_back(b * std::pow(i,a));
	}
	return result;
}

/*************************************************************************
converts 2 float vectors to alglib::real_2d_array
*************************************************************************/
alglib::real_2d_array math_utilits::two_dvector_to_real_2d_array(const std::vector<double>& y_vector, const std::vector<double>& x_vector)
{
	if (y_vector.size() != x_vector.size())
	{
		throw std::invalid_argument("vectors must be the same size");
	}

	std::vector<double> dvector_xy; // (y_vector.size() + x_vector.size());
	for (unsigned int i = 0; i < y_vector.size(); ++i)
	{
		dvector_xy.push_back(x_vector[i]);
		dvector_xy.push_back(y_vector[i]);
	}
	const int number_of_rows = dvector_xy.size() / 2;
	const int number_of_cols = 2;

	double* darr_xy = &dvector_xy[0];
	alglib::real_2d_array r_2d_arr;

	r_2d_arr.setcontent(number_of_rows, number_of_cols, darr_xy);

	return r_2d_arr;
}

/*************************************************************************
returns the coefficients (a, b) in std::array for equation y = a*x^b
array[0]=a;
array[1]=b;
*************************************************************************/
void math_utilits::pr_coefficients(const std::vector<double>& y, const std::vector<double>& x, double& a, double& b, int& info_code)
{

	std::vector<double> curr_y = y;
	std::vector<double> curr_x = x;

	for (double& i : curr_y)
	{
		i = log(i);
	}

	for (double& i : curr_x)
	{
		i = log(i);
	}

	alglib::real_2d_array r_2d_arr_xy = math_utilits::two_dvector_to_real_2d_array(curr_y, curr_x);
	alglib::ae_int_t info;
	alglib::ae_int_t nvars;
	alglib::linearmodel model;
	alglib::lrreport rep;
	alglib::real_1d_array c;

	lrbuild(r_2d_arr_xy, r_2d_arr_xy.rows(), 1, info, model, rep);
	lrunpack(model, c, nvars);
	a = c[0];
	b = exp(c[1]);
	info_code = info;
}

std::vector<double> math_utilits::Linear_interp(const double& f_v, const double& s_v, const int& num_p)
{
	double min_v;
	min_v = f_v <= s_v ? f_v : s_v;
	std::vector<double> res;
	double gap = std::abs(s_v - f_v) / (num_p + 1);
	for (int i = 1; i <= num_p; ++i)
	{
		res.push_back(min_v + i*gap);
	}
	return res;
}

/*************************************************************************
calculate rmse=\sqrt{\frac{1}{n}\Sigma_{i=1}^{n}{\Big({d_i -f_i}\Big)^2}}
for 2 vectors
*************************************************************************/
double math_utilits::rmse(const std::vector<double>& vect_a, const std::vector<double>& vect_b)
{
	if (vect_a.size() != vect_b.size())
	{
		throw std::invalid_argument("vectors must have equal size");
	}
	double result = -1;

	double tmp_1 = 0;
	for (int i = 0; i < vect_a.size(); ++i)
	{
		tmp_1 += (vect_a.at(i)- vect_b.at(i)) * (vect_a.at(i) - vect_b.at(i));//sum (vect_a[i]-vect_b[i])^2
	}
	result = std::sqrt(tmp_1 / (vect_a.size() - 1));
	return result;
}