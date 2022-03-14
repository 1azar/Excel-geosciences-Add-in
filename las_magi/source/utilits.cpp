#include "pch.h"
#include "utilits.h"


std::vector<double> las_utilits::get_values_from_line_of_las_log_data_section(const std::string& line) {
    /*
    "2229.20   -9999   -9999   -9999   -9999 " -> [2229.20, -9999, -9999, -9999, -9999]
    */
    std::stringstream ss1;
    ss1 << line;
    std::string word;
    std::vector<double> result;
    while (ss1 >> word) {
        result.push_back(std::stod(word));
    }
    return result;
}

int las_utilits::las_log_data_line_size(const std::string& line) {
    /*
    "2229.20   -9999   -9999   -9999   -9999 " -> [2229.20, -9999, -9999, -9999, -9999]
    */
    std::stringstream ss1;
    ss1 << line;
    std::string word;
    int result = 0;
    while (ss1 >> word) {
        result++;
    }
    return result;
}

void las_utilits::print_array(const std::vector<std::string>& vec)
{
    for (const auto& i : vec)
    {
        std::cout << i << std::endl;
    }
    std::cout << std::endl;
}

//std::string las_utilits::bstr_to_str(BSTR source) {
//    _bstr_t wrapped_bstr = _bstr_t(source);
//    int length = wrapped_bstr.length();
//    char* char_array = new char[length];
//    strcpy_s(char_array, length + 1, wrapped_bstr);
//    return char_array;
//}

