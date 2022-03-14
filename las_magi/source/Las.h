#pragma once
#include "utilits.h"
#include "math_utilits.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

struct Las_line
{
    Las_line();
    Las_line(const std::string& line);
    std::string mnemonic;
    std::string unit;
    std::string value;
    std::string comment;
    bool mne = false;
    bool uni = false;
    bool val = false;
    bool com = false;
};

struct VERSION_section
{
    /*
    * REQUIRED

    This section is required and is usually found at the very beginning of the file.
    It is used to indicate the version of the LAS file as well as the line break option.
    The section should contain the following mnemonics: VERS, WRAP

    ~Version Information Section
    VERS. 2.0 : CWLS log ASCII Standart – VERSION 2.0
    WRAP.  NO : One line per depth step

    */
public:
    VERSION_section();
    void VERSION_section_parser(std::fstream& file_stream, std::string& last_line);
    std::string GetSectionData(const int& width_for_column);
    std::map<std::string, std::string> VERS = { {"value", ""},{"description", ""} };//required field in the section
    std::map<std::string, std::string> WARP = { {"value", ""},{"description", ""} };//required field in the section
    std::vector<std::map< std::string, std::string >> optional_lines; //[{{"mnemonic", ""}, {"unit", ""}, {"value", ""},{"description", ""}}, {..}, ..] ; optional fields in the section
};

struct WELL_section
{
    /*
    * REQUIRED

    Required. It is intended for listing the borehole identifiers,
    parameters of its location, as well as the initial and final depth in the file.
    The section must contain the following mandatory mnemonics: STRT, STOP

    ~Well Information Section
    #MNEM.UNIT      DATA TYPE           INFORMATION
    #------------   -----------------   ---------------------
    STRT.M          900.200             : Начальная глубина
    STOP.M          1110.000            : Конечная глубина
    STEP.M          0.100               : Шаг квантования по глубине
    NULL.           -9999.25            : Отсутствие данных
    COMP.           Заказчик            : Компания-заказчик
    ..
    */
public:
    WELL_section();
    void WELL_section_parser(std::fstream& file_stream, std::string& last_line);
    std::string GetSectionData(const int& width_for_column) const;
    std::map<std::string, std::string> STRT = { {"unit",""}, {"value", ""},{"description", ""} };//required field in the section
    std::map<std::string, std::string> STOP = { {"unit",""}, {"value", ""},{"description", ""} };//required field in the section
    std::vector<std::map< std::string, std::string >> optional_lines;//[{{"mnemonic", ""}, {"unit", ""}, {"value", ""},{"description", ""}}, {..}, ..] ; optional fields in the section
};

struct CURVE_section
{
    /*
    * REQUIRED

    the curves are described (with units of measurement) in the order in which they are
    written in data sections. The API codes are optional in the mnemonic API CODE portion.
    As a rule, the first column is the depth curve.

    ~Curve Information Section
    #MNEM.UNIT   API CODE     CURVE DESCRIPTION
    #---------   --------     -----------------
    DEPT.M                    : DEPTH
    ILD.OHMM     05 070 46 00 : INDUCTION LATEROLOG
    SP .MV       06 010 01 00 : SPONTANEOUS POTENTIAL
    GR .API      42 310 00 00 : GAMMA RAY
    CALI.MM      42 280 00 00 : CALIPER
    */
public:
    CURVE_section();
    void CURVE_section_parser(std::fstream& file_stream, std::string& last_line);
    std::string GetSectionData(const int& width_for_column) const;
    void AddCurve(const std::string& curve_mnemonic,
        const std::string& curve_unit,
        const std::string& API_code,
        const std::string& curve_description);
    std::vector<std::string> get_curves_mnemonics_unit() const;
    std::vector<std::string> get_curves_mnemonics() const;

    std::vector<std::map< std::string, std::string >> CURVES; //[{{"mnemonic", ""}, {"unit", ""}, {"value", ""},{"description", ""}}, {..}, ..] ; optional fields in the section
};

struct Parameters_section
{
    /*
    * OPTIONAL

    This section is optional.
    It is intended to describe various types of parameters related to a well.
    These parameters can be either numeric or text.
    There are no restrictions on the number of mnemonics in this section.

    ~Parameter Information Section
    #MNEM.UNI Value     Description
    #-------- -----     -----------
    BHT .DEG  114.0000  : Температура на забоеew
    MDEN.K/M3 1180.0000 : Плотность бурового раствора
    MRo .MV   1.2       : Сопротивление бур. раствора
    */
public:
    Parameters_section();
    void Parameters_section_parser(std::fstream& file_stream, std::string& last_line);
    std::string GetSectionData(const int& width_for_column) const;
    void AddParameter(const std::string& parameter_mnemonic,
        const std::string& parameter_unit,
        const std::string& parameter_value,
        const std::string& parameter_description);
    /*
    parameters data
    exp: {{param1, {{"unit"," "}, {"value", " "},{"description", " "}}}, {param2, {{"unit"," "}, {"value", " "},{"description", " "}}}, etc..}
    */
    std::vector<std::map< std::string, std::string >> PARAMETERS; //[{{"mnemonic", ""}, {"unit", ""}, {"value", ""},{"description", ""}}, {..}, ..] ; optional fields in the section
};

struct Other_section
{
    /*
    * OPTIONAL

    It is intended for various kinds of comments related to the well.
    The length of the comment is not limited by anything.
    A comment can span multiple lines.

    ~Other Information Section
    При интерпретации использовались данные геолого-геофизического
    обобщения. Для уточнения характера насыщения пласта 5
    рекомендуется провести ОПК.
    */
public:
    Other_section();
    void Other_section_parser(std::fstream& file_stream, std::string& last_line);
    std::string GetSectionData() const;
    void AddOther(const std::string& other_text);
    std::string Other_data;
};

struct CURVE_stru
{
    /*
    Curve data structure
    */
public:
    CURVE_stru();
    void add_value(const double& val);
    void update_curve();
    std::vector<double> get_log() const;
    double get_LOG_value_by_index(const int& idx);
private:
    double TOP;
    double BOT;
    double MAX;
    double MIN;
    std::vector<double> LOG;
};

struct ASCII_LOG_DATA_section
{
    /*
    * REQUIRED

    This section is designed to store information about the curves
    described in the “Curve Information Section” in the form of separate columns.
    A line of data less than 256 characters long is usually written in WRAP mode as “NO” (ie, no line break). Otherwise, the WRAP mnemonic value must be set to “YES”;

    ~ASCII Log Data
    1509.0000 3.5085 30.2272 -999.2500 276.0280
    1509.2000 3.5325 30.2290    7.0754 276.0280
    1509.4000 3.6083 30.2307    8.0754 278.3948
    */
public:
    ASCII_LOG_DATA_section();
    void ASCII_LOG_DATA_section_parser(std::fstream& file_stream, std::string& last_line);
    std::string GetSectionData(const int& width_for_column, const int& line_count_for_result_str);
    std::vector<std::vector<double>> get_curves_as_double_mtx();
    std::vector<double> get_curve_by_index(const int& index);
private:
    std::vector<CURVE_stru> data;
    int value_count = 0;
};

class Las
{
public:

    Las();

    Las(const std::string& path);

    void print_las();

    std::string print_las_to_string();

    std::vector<std::vector<std::string>> print_las_to_2d_vector();

    std::vector<std::string> get_curves_mnemonics() const;

    std::vector<std::vector<double>> get_curves_mtx();

    std::vector<double> get_curve_by_mnemonic(const std::string& mnemonic);

    double get_STEP_value();

    std::vector<std::vector<double>> restor_AK_by_NK_ZB_singleWell( const std::vector<double>& AK_curve,
                                                                    double& AK_strt_restore_depth,
                                                                    double& AK_stop_restore_depth,
                                                                    const std::vector<double>& NK_curve,
                                                                    const std::vector<double>& ZB_curve,
                                                                    const double& depth_step,
                                                                    int& info);

private:

    void Section_reader(std::fstream& stream, const std::string& line);

    int Index_for_first_apearing_value(const std::vector<double>& depth_curve, const double& value);

    VERSION_section VERSION_;
    WELL_section WELL_;
    CURVE_section CURVE_;
    Parameters_section PARAMETERS_;
    Other_section OTHERS_;
    ASCII_LOG_DATA_section LOG_;

    std::map<std::string, bool> LasRequiredSectionsFlags = { {"~V",false},{"~W",false} ,{"~C",false} ,{"~A",false} };
    std::map<std::string, bool> LasOptionSectionsFlags = { {"~P",false},{"~O",false} };
};

