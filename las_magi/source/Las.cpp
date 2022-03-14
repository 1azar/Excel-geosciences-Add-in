#pragma once
#include "pch.h"
//made by Azat in 2021
//Log ASCII Standard reading/writing lib [1.2 and 2.0 version of las supported] 
#include "Las.h"

Las_line::Las_line()
{
    std::string mnemonic = "NONE";
    std::string unit = "NONE";
    std::string value = "NONE";
    std::string comment = "NONE";
};

Las_line::Las_line(const std::string& line) {

    for (const auto& iter : line) {
        if (!mne) {
            if (iter == '.') {
                mne = true;
                continue;
            }
            mnemonic.push_back(iter);
        }
        else if (!uni) {
            if (iter == ' ') {
                uni = true;
                continue;
            }
            unit.push_back(iter);
        }
        else if (!val) {
            if (iter == ':') {
                val = true;
                continue;
            }
            if (iter != ' ') {
                value.push_back(iter);
            }
        }
        else {
            comment.push_back(iter);
        }
    }

}



VERSION_section::VERSION_section() {}

void VERSION_section::VERSION_section_parser(std::fstream& file_stream, std::string& last_line) {
    std::string line2;
    while (std::getline(file_stream, line2)) {
        if (line2.substr(0, 1) != "~") {
            if (line2.substr(0, 1) != "#") {
                /*
                Pars line2 here
                */

                Las_line current_las_line(line2);

                if (current_las_line.mnemonic == "VERS")
                {
                    VERS["value"] = current_las_line.value;
                    VERS["description"] = current_las_line.comment;
                }
                else if (current_las_line.mnemonic == "WRAP")
                {
                    WARP["value"] = current_las_line.value;
                    WARP["description"] = current_las_line.comment;
                }
                else {
                    optional_lines.push_back({ {"mnemonic", current_las_line.mnemonic},
                                                {"unit", current_las_line.unit},
                                                {"value", current_las_line.value},
                                                {"description", current_las_line.comment} });
                }
            }

        }
        else
        {
            last_line = line2;
            break;
        }
    }
}

std::string VERSION_section::GetSectionData(const int& width_for_column) {
    /*
    returns section data in formatted form
    */

    std::string s1;
    std::stringstream ss;
    ss << "VERS" << "." << "" << std::fixed << std::setw(width_for_column) << VERS.at("value") << ":" << VERS.at("description") << "\n";
    ss << "WARP" << "." << "" << std::fixed << std::setw(width_for_column) << WARP.at("value") << ":" << WARP.at("description") << "\n";
    for (const auto& ol : optional_lines) {
        ss << ol.at("mnemonic") << "." << ol.at("unit") << std::fixed << std::setw(width_for_column) << ol.at("value") << ":" << ol.at("description") << "\n";
    }
    s1 = ss.str();
    return s1;
}



WELL_section::WELL_section() {}

void WELL_section::WELL_section_parser(std::fstream& file_stream, std::string& last_line) {
    std::string line2;
    while (std::getline(file_stream, line2)) {
        if (line2.substr(0, 1) != "~") {
            if (line2.substr(0, 1) != "#") {
                /*
                Pars line2 here
                */

                Las_line current_las_line(line2);

                if (current_las_line.mnemonic == "STRT")
                {
                    STRT["unit"] = current_las_line.unit;
                    STRT["value"] = current_las_line.value;
                    STRT["description"] = current_las_line.comment;
                }
                else if (current_las_line.mnemonic == "STOP")
                {
                    STOP["unit"] = current_las_line.unit;
                    STOP["value"] = current_las_line.value;
                    STOP["description"] = current_las_line.comment;
                }
                else {
                    optional_lines.push_back({ {"mnemonic", current_las_line.mnemonic},
                                                {"unit", current_las_line.unit},
                                                {"value", current_las_line.value},
                                                {"description", current_las_line.comment} });
                }
            }

        }
        else
        {
            last_line = line2;
            break;
        }
    }
}

std::string WELL_section::GetSectionData(const int& width_for_column) const {
    /*
    returns section data in formatted form
    */

    std::string s1;
    std::stringstream ss;
    ss << "STRT" << "." << STRT.at("unit") << std::fixed << std::setw(width_for_column) << STRT.at("value") << ":" << STRT.at("description") << "\n";
    ss << "STOP" << "." << STOP.at("unit") << std::fixed << std::setw(width_for_column) << STOP.at("value") << ":" << STOP.at("description") << "\n";
    for (const auto& ol : optional_lines) {
        ss << ol.at("mnemonic") << "." << ol.at("unit") << std::fixed << std::setw(width_for_column) << ol.at("value") << ":" << ol.at("description") << "\n";
    }
    s1 = ss.str();
    return s1;
}




CURVE_section::CURVE_section() {}

void CURVE_section::CURVE_section_parser(std::fstream& file_stream, std::string& last_line) {
    std::string line2;
    while (std::getline(file_stream, line2)) {
        if (line2.substr(0, 1) != "~") {
            if (line2.substr(0, 1) != "#") {
                /*
                Pars line2 here
                */

                Las_line current_las_line(line2);
                CURVES.push_back({ {"mnemonic", current_las_line.mnemonic},
                                                {"unit", current_las_line.unit},
                                                {"value", current_las_line.value},
                                                {"description", current_las_line.comment} });
            }
        }
        else
        {
            last_line = line2;
            break;
        }
    }
}

std::string CURVE_section::GetSectionData(const int& width_for_column) const {
    /*
    returns section data in formatted form
    */

    std::string s1;
    std::stringstream ss;
    for (const auto& ol : CURVES) {
        ss << ol.at("mnemonic") << "." << ol.at("unit") << std::fixed << std::setw(width_for_column) << ol.at("value") << ":" << ol.at("description") << "\n";
    }
    s1 = ss.str();
    return s1;
}

void CURVE_section::AddCurve(const std::string& curve_mnemonic,
    const std::string& curve_unit,
    const std::string& API_code,
    const std::string& curve_description) {

    /*
    inserts a new curve to CURVES_MAP
    Checking for matches and taking some action
    HAS NOT FINISHED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    */

}

std::vector<std::string> CURVE_section::get_curves_mnemonics_unit() const
{
    std::vector<std::string> mnemonics;
    for (const auto i : CURVES)
    {
        //mnemonics.push_back(i.at("mnemonic")+"."+ i.at("unit"));
        //input.erase(remove_if(input.begin(), input.end(), isspace),input.end());
        std::string result_mnemonic = i.at("mnemonic");
        std::string result_unit = i.at("unit");

        result_mnemonic.erase(remove_if(result_mnemonic.begin(), result_mnemonic.end(), isspace), result_mnemonic.end());
        result_unit.erase(remove_if(result_unit.begin(), result_unit.end(), isspace), result_unit.end());

        mnemonics.push_back(result_mnemonic + "." + result_unit);
        //mnemonics.push_back(i.at("mnemonic") + "." + i.at("unit"));
    }
    return mnemonics;
}

std::vector<std::string> CURVE_section::get_curves_mnemonics() const
{
    std::vector<std::string> mnemonics;
    for (const auto i : CURVES)
    {
        std::string result_mnemonic = i.at("mnemonic");

        result_mnemonic.erase(remove_if(result_mnemonic.begin(), result_mnemonic.end(), isspace), result_mnemonic.end());

        mnemonics.push_back(result_mnemonic);
    }
    return mnemonics;
}





Parameters_section::Parameters_section() {}

void Parameters_section::Parameters_section_parser(std::fstream& file_stream, std::string& last_line) {
    std::string line2;
    while (std::getline(file_stream, line2)) {
        if (line2.substr(0, 1) != "~") {
            if (line2.substr(0, 1) != "#") {
                /*
                Pars line2 here
                */

                Las_line current_las_line(line2);
                PARAMETERS.push_back({ {"mnemonic", current_las_line.mnemonic},
                                                {"unit", current_las_line.unit},
                                                {"value", current_las_line.value},
                                                {"description", current_las_line.comment} });
            }
        }
        else
        {
            last_line = line2;
            break;
        }
    }
}

std::string Parameters_section::GetSectionData(const int& width_for_column) const {
    /*
    returns section data in formatted form
    */

    std::string s1;
    std::stringstream ss;
    for (const auto& ol : PARAMETERS) {
        ss << ol.at("mnemonic") << "." << ol.at("unit") << std::fixed << std::setw(width_for_column) << ol.at("value") << ":" << ol.at("description") << "\n";
    }
    s1 = ss.str();
    return s1;
}

void Parameters_section::AddParameter(const std::string& parameter_mnemonic,
    const std::string& parameter_unit,
    const std::string& parameter_value,
    const std::string& parameter_description) {
    /*
    inserts a new curve to PARAMETERS_MAP
    Checking for matches and taking some action
    HAS NOT FINISHED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    */
}



Other_section::Other_section() {}

void Other_section::Other_section_parser(std::fstream& file_stream, std::string& last_line) {
    std::string line2;
    while (std::getline(file_stream, line2)) {
        if (line2.substr(0, 1) != "~") {
            if (line2.substr(0, 1) != "#") {
                /*
                Pars line2 here
                */

                Other_data += line2;
            }
        }
        else
        {
            last_line = line2;
            break;
        }
    }
}

std::string Other_section::GetSectionData() const {
    /*
    returns section data in formatted form
    */
    return Other_data + "\n";
}

void Other_section::AddOther(const std::string& other_text) {
    /*
    inserts a new curve to CURVES_MAP
    Checking for matches and taking some action
    HAS NOT FINISHED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    */
}



CURVE_stru::CURVE_stru() {
    //MNEMONIC = const std::string& mnem. mb it should contain a field for curves name. 
    // xz poka kak ludshe sdelat' 
    TOP = -1;
    BOT = -1;
    MAX = -1;
    MIN = -1;
}

void CURVE_stru::add_value(const double& val) {
    LOG.push_back(val);
}

void CURVE_stru::update_curve() {
    /*
    calculate here: MAX, TOP, etc
    HAS NOT FINISHED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    */
}

std::vector<double> CURVE_stru::get_log() const{
    return LOG;
}

double CURVE_stru::get_LOG_value_by_index(const int& idx) {
    /*
    returns log by its index
    */

    return LOG[idx];
}




ASCII_LOG_DATA_section::ASCII_LOG_DATA_section() {}

void ASCII_LOG_DATA_section::ASCII_LOG_DATA_section_parser(std::fstream& file_stream, std::string& last_line) {
    std::string line2;
    bool first_log_line_flag = true;

    while (std::getline(file_stream, line2)) {
        if (line2.substr(0, 1) != "~") {
            if (line2.substr(0, 1) != "#") {
                /*
                Pars line2 here
                */

                if (first_log_line_flag) {// to define how many curves are there then resizing the data
                    /*
                    execute once on the first line
                    */

                    data.resize(las_utilits::las_log_data_line_size(line2));
                    for (unsigned int i = 0; i < data.size(); ++i) {
                        data[i] = CURVE_stru();
                    }
                    first_log_line_flag = false;
                }

                std::vector<double> tmp = las_utilits::get_values_from_line_of_las_log_data_section(line2);
                for (unsigned int cu = 0; cu < data.size(); ++cu) {
                    data[cu].add_value(tmp[cu]);
                }
                value_count++;
            }
        }
        else
        {
            last_line = line2;
            break;
        }
    }
    for (auto& cu : data) cu.update_curve();
}

std::string ASCII_LOG_DATA_section::GetSectionData(const int& width_for_column, const int& line_count_for_result_str) {
    /*
    returns section data in formatted form
    */

    std::string s1;
    std::stringstream ss;
    if (value_count < line_count_for_result_str) {
        for (int k = 0; k < value_count; ++k) {
            for (unsigned int k2 = 0; k2 < data.size(); ++k2) {
                ss << std::fixed << std::left << std::setw(width_for_column) << data[k2].get_LOG_value_by_index(k);
            }
            ss << "\n";
        }
    }
    else {
        for (int k = 0; k < line_count_for_result_str; ++k) {
            for (unsigned int k2 = 0; k2 < data.size(); ++k2) {
                ss << std::fixed << std::left << std::setw(width_for_column) << data[k2].get_LOG_value_by_index(k);
            }
            ss << "\n";
        }
        ss << ". . ." << "\n";
    }
    s1 = ss.str();
    return s1;
}

std::vector<std::vector<double>> ASCII_LOG_DATA_section::get_curves_as_double_mtx()
{
    std::vector<std::vector<double>> curves_mtx;
    for (const CURVE_stru& i : data)
    {
        curves_mtx.push_back(i.get_log());
    }
    return curves_mtx;
}

std::vector<double> ASCII_LOG_DATA_section::get_curve_by_index(const int& index)
{
    return data[index].get_log();
}

Las::Las() {}

Las::Las(const std::string& path) {
    /*
    class constructor, takes the path to the Las file.
    */


    /*
    instantiating a stream object
    */
    std::fstream input(path);
    std::string line;

    /*
    search for required sections in the file and create corresponding structures
    */
    if (input) {
        while (std::getline(input, line))
        {
            Section_reader(input, line);
        }
    }
    else {
        /*
        THROW ERROR HERE. REASON: NONEXISTING PATH
        */
    }
}

/*
Creating a stringstream object.
supplementing it with various strings, which are formed by the corresponding structures for each
section of the las file.
at the end information output
*/
void Las::print_las() {

    std::stringstream ss;
    ss << "~VERSION INFORMATION \n";
    ss << VERSION_.GetSectionData(20);

    ss << "~WELL INFORMATION \n";
    ss << WELL_.GetSectionData(20);

    ss << "~PARAMETER INFORMATION BLOCK \n";
    ss << PARAMETERS_.GetSectionData(20);

    ss << "~OTHER INFORMATION \n";
    ss << OTHERS_.GetSectionData();

    ss << "~CURVE INFORMATION \n";
    ss << CURVE_.GetSectionData(20);

    ss << "~ASCII Log Data \n";
    ss << LOG_.GetSectionData(20, 4);

    std::cout << ss.str() << std::endl;
}

/*
returns std::string with las info
*/
std::string Las::print_las_to_string() {
/*
Creating a stringstream object.
supplementing it with various strings, which are formed by the corresponding structures for each
section of the las file. at the end returns std::string
*/
    std::stringstream ss;
    ss << "~VERSION INFORMATION \n";
    ss << VERSION_.GetSectionData(20);

    ss << "~WELL INFORMATION \n";
    ss << WELL_.GetSectionData(20);

    ss << "~PARAMETER INFORMATION BLOCK \n";
    ss << PARAMETERS_.GetSectionData(20);

    ss << "~OTHER INFORMATION \n";
    ss << OTHERS_.GetSectionData();

    ss << "~CURVE INFORMATION \n";
    ss << CURVE_.GetSectionData(20);

    ss << "~ASCII Log Data \n";
    ss << LOG_.GetSectionData(20, 4);

    std::string result = ss.str();
    return result;
}

std::vector<std::vector<std::string>> Las::print_las_to_2d_vector()
{
    //дальше будет костыль полнейший, поэтому рекомендуется чиtать с закрытыми глазами (это пока тестовая функция). Можно всё компактнее сделать, но надо весь код переписывать, а именно сделать универсальную структуру для всех секций и использовать только ее
    
    std::vector<std::vector<std::string>> result;
    
    std::vector<std::string> vTemp = { "","","",""};

    //~VERION
    vTemp = { "~VERSION INFORMATION","","", ""};
    result.push_back(vTemp);

    vTemp = { "VERS.","",VERSION_.VERS.at("value"),':'+VERSION_.VERS.at("description")};
    result.push_back(vTemp);

    vTemp = { "WARP.","",VERSION_.WARP.at("value"),':' + VERSION_.WARP.at("description") };
    result.push_back(vTemp);

    for (const auto& i : VERSION_.optional_lines) 
    {
        vTemp = { i.at("mnemonic") + ".", i.at("unit"), i.at("value"), ':' + i.at("description")};
        result.push_back(vTemp);
    }

    //~WELL
    vTemp = { "~WELL INFORMATION","","", "" };
    result.push_back(vTemp);

    vTemp = { "STRT.", WELL_.STRT.at("unit"), WELL_.STRT.at("value"),':' + WELL_.STRT.at("description")};
    result.push_back(vTemp);

    vTemp = { "STOP.", WELL_.STOP.at("unit"), WELL_.STOP.at("value"),':' + WELL_.STOP.at("description") };
    result.push_back(vTemp);

    for (const auto& i : WELL_.optional_lines)
    {
        vTemp = { i.at("mnemonic") + ".", i.at("unit"), i.at("value"), ':' + i.at("description")};
        result.push_back(vTemp);
    }

    //~PARAMETER 
    vTemp = { "~PARAMETER INFORMATION BLOCK","","",""};
    result.push_back(vTemp);

    for (const auto& i : PARAMETERS_.PARAMETERS)
    {
        vTemp = { i.at("mnemonic") + ".", i.at("unit"), i.at("value"), ':' + i.at("description")};
        result.push_back(vTemp);
    }

    //~OTHER
    vTemp = { "~OTHER INFORMATION","","" , "" };
    result.push_back(vTemp);

    vTemp = { OTHERS_.Other_data,"","" , "" };
    result.push_back(vTemp);

    //~CURVE
    vTemp = { "~CURVE INFORMATION","","", "" };
    result.push_back(vTemp);

    for (const auto& i : CURVE_.CURVES)
    {
        vTemp = { i.at("mnemonic") + ".", i.at("unit"), i.at("value"), ':' + i.at("description")};
        result.push_back(vTemp);
    }

    return result;
}

std::vector<std::string> Las::get_curves_mnemonics() const
{
    return CURVE_.get_curves_mnemonics_unit();
}

std::vector<std::vector<double>> Las::get_curves_mtx() 
{
    return LOG_.get_curves_as_double_mtx();
}

std::vector<double> Las::get_curve_by_mnemonic(const std::string& mnemonic)
{
    std::vector<std::string> data_mnemonics = CURVE_.get_curves_mnemonics();
    int index = -1;
    for (unsigned int i = 0; i < data_mnemonics.size(); ++i)
    {
        if (data_mnemonics[i] == mnemonic)
        {
            index = i;
        }
    }
    std::vector<std::string> data_mnemonics_unit = CURVE_.get_curves_mnemonics_unit();
    for (unsigned int i = 0; i < data_mnemonics_unit.size(); ++i)
    {
        if (data_mnemonics_unit[i] == mnemonic)
        {
            index = i;
        }
    }
    if (index == -1)
    {
        std::vector<double> empty_vector;
        return empty_vector;
    }
    return LOG_.get_curve_by_index(index);
}

double Las::get_STEP_value()
{
    std::string step_mnemonic = "STEP";
    double result_STEP_value = -1;//-1 means tha no STEP field has been found
    for (const auto& i : WELL_.optional_lines)
    {
        if (i.at("mnemonic") == step_mnemonic)
        {
            result_STEP_value = std::stod(i.at("value"));
            break;
        }
    }
    return result_STEP_value;
}

/*************************************************************************
returns the index for the special value
*************************************************************************/
int Las::Index_for_first_apearing_value(const std::vector<double>& depth_curve, const double& value)
{
    int counter = 0;
    for (const auto& i : depth_curve)
    {
        if (i >= value)
        {
            break;
        }
        ++counter;
    }
    return counter;
}


/*************************************************************************
restore AK curve in ZB(AK_strt_restore, AK_stop_restore) 
by using NK curve as a regressor
*************************************************************************/
std::vector<std::vector<double>> Las::restor_AK_by_NK_ZB_singleWell(const std::vector<double>& AK_curve,
                                                                   double& AK_strt_restore_depth,
                                                                   double& AK_stop_restore_depth,
                                                                   const std::vector<double>& NK_curve,
                                                                   const std::vector<double>& ZB_curve,
                                                                   const double& depth_step,
                                                                   int& info)
{
    if (AK_stop_restore_depth<AK_strt_restore_depth)
    {
        throw std::invalid_argument("stop depth is less than start depth");
    }

    AK_strt_restore_depth = (AK_strt_restore_depth < ZB_curve.at(0)) ? ZB_curve.at(0) : AK_strt_restore_depth; 
    AK_stop_restore_depth = (AK_stop_restore_depth > ZB_curve.at(ZB_curve.size()-1)) ? ZB_curve.at(ZB_curve.size() - 1) : AK_stop_restore_depth;

    int size_for_top = Las::Index_for_first_apearing_value(ZB_curve, AK_strt_restore_depth);
    int steps_in_indexes = depth_step / (ZB_curve.at(1) - ZB_curve.at(0));
    int numb_fited_steps_for_top_part = size_for_top / steps_in_indexes;
    std::vector<std::array<double, 2>> ab_top;
    for (int i = 0; i < numb_fited_steps_for_top_part; ++i)
    {
        std::vector<double> curr_AK = std::vector<double>(AK_curve.begin() + i * steps_in_indexes, AK_curve.begin() + (i + 1) * steps_in_indexes);
        std::vector<double> curr_NK = std::vector<double>(NK_curve.begin() + i * steps_in_indexes, NK_curve.begin() + (i + 1) * steps_in_indexes);
        double curr_a = 0;
        double curr_b = 0;
        int curr_info = -1;
        math_utilits::pr_coefficients(curr_AK, curr_NK, curr_a, curr_b, curr_info);
        std::array<double, 2> curr_ab;
        curr_ab[0] = curr_a;
        curr_ab[1] = curr_b;
        ab_top.push_back(curr_ab);
    }
    //intervals from AK and NK at (numb_fited_steps_for_top_part*step_in_indexes) to AK_strt_restore_depth are not processed in purpose, as there may be not enough points for collecting statistics

    //bot part
    int index_for_stop_ak = Las::Index_for_first_apearing_value(ZB_curve, AK_stop_restore_depth);
    int size_for_bot = (ZB_curve.size()-1) - index_for_stop_ak;
    steps_in_indexes = depth_step / (ZB_curve.at(ZB_curve.size()-1) - ZB_curve.at(ZB_curve.size() - 2));
    int numb_fited_steps_for_bot_part = size_for_bot / steps_in_indexes;
    std::vector<std::array<double, 2>> ab_bot;

    int gap = index_for_stop_ak;

    for (int i = 0; i < numb_fited_steps_for_bot_part; ++i)
    {
        std::vector<double> curr_AK = std::vector<double>(AK_curve.begin() + gap + i * steps_in_indexes, AK_curve.begin() + gap + (i + 1) * steps_in_indexes);
        std::vector<double> curr_NK = std::vector<double>(NK_curve.begin() + gap + i * steps_in_indexes, NK_curve.begin() + gap + (i + 1) * steps_in_indexes);
        double curr_a = 0;
        double curr_b = 0;
        int curr_info = -1;
        math_utilits::pr_coefficients(curr_AK, curr_NK, curr_a, curr_b, curr_info);
        std::array<double, 2> curr_ab;
        curr_ab[0] = curr_a;
        curr_ab[1] = curr_b;
        ab_bot.push_back(curr_ab);
    }

    int num_p = (AK_stop_restore_depth - AK_strt_restore_depth) / depth_step;
    AK_strt_restore_depth = (AK_strt_restore_depth < ZB_curve.at(0)) ? ZB_curve.at(0) : AK_strt_restore_depth;
    num_p = num_p == 0 ? 1 : num_p;

    std::vector<std::vector<double>> ab_vecs; //NK, a, b, DTP_calc, +DTP_real, ZB
    ab_vecs.push_back(std::vector<double>(  NK_curve.begin() +
                                            Las::Index_for_first_apearing_value(ZB_curve, AK_strt_restore_depth),
                                            NK_curve.begin() +
                                            Las::Index_for_first_apearing_value(ZB_curve, AK_stop_restore_depth)));
    if (ab_top.size() > 0 && ab_bot.size() > 0) 
    {
        ab_vecs.push_back(math_utilits::Linear_interp(ab_top[ab_top.size() - 1][0], ab_bot[0][0], num_p));
        ab_vecs.push_back(math_utilits::Linear_interp(ab_top[ab_top.size() - 1][1], ab_bot[0][1], num_p));
        ab_vecs.push_back(math_utilits::sintez_curve_pow(ab_vecs[0], ab_vecs[1][0], ab_vecs[2][0]));
    }
    ab_vecs.push_back(std::vector<double>(  AK_curve.begin() +
                                            Las::Index_for_first_apearing_value(ZB_curve, AK_strt_restore_depth),
                                            AK_curve.begin() +
                                            Las::Index_for_first_apearing_value(ZB_curve, AK_stop_restore_depth)));
    ab_vecs.push_back(std::vector<double>(  ZB_curve.begin() +
                                            Las::Index_for_first_apearing_value(ZB_curve, AK_strt_restore_depth),
                                            ZB_curve.begin() +
                                            Las::Index_for_first_apearing_value(ZB_curve, AK_stop_restore_depth)));
    std::vector<double> rmse_1;
    rmse_1.push_back(math_utilits::rmse(ab_vecs.at(3), ab_vecs.at(4)));
    ab_vecs.push_back(rmse_1);
    return ab_vecs;

}

void Las::Section_reader(std::fstream& stream, const std::string& line) {

    std::string section_name_flag = line.substr(0, 2);
    std::string lastLine;

    /*
    REQUIRED SECTIONS:
    */
    if (section_name_flag == "~V")
    {
        VERSION_.VERSION_section_parser(stream, lastLine);
        Section_reader(stream, lastLine);
        LasRequiredSectionsFlags[section_name_flag] = true;
    }

    if (section_name_flag == "~W")
    {
        WELL_.WELL_section_parser(stream, lastLine);
        Section_reader(stream, lastLine);
        LasRequiredSectionsFlags[section_name_flag] = true;
    }

    if (section_name_flag == "~C")
    {
        CURVE_.CURVE_section_parser(stream, lastLine);
        Section_reader(stream, lastLine);
        LasRequiredSectionsFlags[section_name_flag] = true;
    }

    if (section_name_flag == "~A")
    {
        LOG_.ASCII_LOG_DATA_section_parser(stream, lastLine);
        Section_reader(stream, lastLine);
        LasRequiredSectionsFlags[section_name_flag] = true;
    }

    /*
    OPTIONAL SECTIONS:
    */
    if (section_name_flag == "~P")
    {
        PARAMETERS_.Parameters_section_parser(stream, lastLine);
        Section_reader(stream, lastLine);
        LasOptionSectionsFlags[section_name_flag] = true;
    }

    if (section_name_flag == "~O")
    {
        OTHERS_.Other_section_parser(stream, lastLine);
        Section_reader(stream, lastLine);
        LasOptionSectionsFlags[section_name_flag] = true;
    }
}