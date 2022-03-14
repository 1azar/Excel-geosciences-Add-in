#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <wtypes.h>
#include <comutil.h>

class las_utilits {
public:

	static std::vector<double> get_values_from_line_of_las_log_data_section(const std::string& line);

	static int las_log_data_line_size(const std::string& line);

	static void print_array(const std::vector<std::string>& vec);

};


