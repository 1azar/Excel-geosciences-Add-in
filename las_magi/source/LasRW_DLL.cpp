// LasRW_DLL.cpp : Defines the exported functions for the DLL.
#include "pch.h"
#include "LasRW_DLL.h"

/*
TO DO IN FUTURE:
    >Now each function creates a new instance of the Las() object - that why, when any function is called, there will be a new call to the disk to read the file. Probably should be fixed somehow ¯\_(ツ)_/¯
        обычно делают так, функция открывает файл, передает хендл вызывающей стороне, а после этот хендл, ассоциированный с каким-то внутренним контекстом пихается в каждую функцию, которой требуется этот контекст
*/

/*
return BSTR (String in VBA) which includes las section information, but ASCII LOG DATA section will be cropped
*/


//=========================================================================================================================================================
//BSTR __stdcall get_las_sections_info_as_string_var(LPCSTR* las_path)
//{
//    std::string str_path(*las_path);
//    Las my_las(str_path);
//    std::string las_sections_info_str = my_las.print_las_to_string();
//    wchar_t* las_sections_info_wstr = (wchar_t*)las_sections_info_str.c_str();
//    return SysAllocString(las_sections_info_wstr);
//}
/*
return SAFEARRAY* (String [0:][0:3] in VBA) which includes las section information except ASCII LOG DATA section
*/
SAFEARRAY* __stdcall get_las_sections_as_arr(LPCSTR* las_path)
{
    std::string str_path(*las_path);
    Las my_las(str_path);
    std::vector<std::vector<std::string>> vectr_2d_of_las_sections_info = my_las.print_las_to_2d_vector();

    SAFEARRAYBOUND bound[2];
    bound[0].lLbound = 0;
    bound[0].cElements = vectr_2d_of_las_sections_info.size();
    bound[1].lLbound = 0;
    bound[1].cElements = vectr_2d_of_las_sections_info[0].size();

    SAFEARRAY* ret;
    LONG indexes[2];

    ret = SafeArrayCreate(
        VT_BSTR,
        2,
        bound
    );

    int min_row = bound[0].lLbound;
    int max_row = min_row + (int)bound[0].cElements - 1;

    int min_column = bound[1].lLbound;
    int max_column = min_column + (int)bound[1].cElements - 1;

    for (int row = min_row; row <= max_row; ++row)
    {
        indexes[0] = row;
        for (int column = min_column; column <= max_column; ++column)
        {
            indexes[1] = column;
            std::wstring wsTemp(vectr_2d_of_las_sections_info[row][column].begin(), vectr_2d_of_las_sections_info[row][column].end());
            SafeArrayPutElement(ret, indexes, SysAllocString(wsTemp.c_str()));
        }
    }
    return ret;
}

//SAFEARRAY* __stdcall get_curve_mnemonics_as_arr(LPCSTR* las_path)




//=============================test=================================


Las* __stdcall creat_context_las(LPCSTR* las_path)
{
    std::string str_path(*las_path);
    Las* my_las = new Las(str_path);
    return my_las;
}

void __stdcall delete_context_las(Las* my_las_heap_pointer)
{
    delete my_las_heap_pointer;
}

SAFEARRAY* __stdcall ctx_get_curve_by_mnem(Las* my_las_heap_pointer, LPCSTR* mnem)
{
    std::string mnemonic(*mnem);
    std::vector<double> curve = my_las_heap_pointer->get_curve_by_mnemonic(mnemonic);

    SAFEARRAYBOUND dim;
    SAFEARRAY* ret;
    dim.lLbound = 0;
    dim.cElements = curve.size();
    ret = SafeArrayCreate(
        VT_R8,
        1,
        &dim      
    );
    for (LONG index = 0; index < curve.size(); ++index)
    {
        double Dtmp = curve[index]; //round till 3d char 
        SafeArrayPutElement(ret, &index, &Dtmp);
    }
    return ret;
}

/*************************************************************************
info:
-1 - some of the curve arent found
*************************************************************************/
SAFEARRAY* __stdcall ctx_restore_acoustic_curve_part(   Las* my_las_heap_pointer,
                                                        LPCSTR* AK_mnem,
                                                        double& AK_strt,
                                                        double& AK_stop,
                                                        LPCSTR* NK_mnem,
                                                        LPCSTR* ZB_mnem,
                                                        double& depth_step,
                                                        int& info)
{
    std::string AK_mnemonic(*AK_mnem);
    std::string NK_mnemonic(*NK_mnem);
    std::string ZB_mnemonic(*ZB_mnem);
    std::vector<double> AK_curve = my_las_heap_pointer->get_curve_by_mnemonic(AK_mnemonic);
    std::vector<double> NK_curve = my_las_heap_pointer->get_curve_by_mnemonic(NK_mnemonic);
    std::vector<double> ZB_curve = my_las_heap_pointer->get_curve_by_mnemonic(ZB_mnemonic);

    if (AK_curve.size() == 0 || NK_curve.size() == 0 || ZB_curve.size() == 0)
    {
        info = -1;
        CComSafeArray<double> arr(1);
        return arr;
    }

    std::vector<std::vector<double>> res_cur_crop_a_b = my_las_heap_pointer->restor_AK_by_NK_ZB_singleWell(AK_curve, AK_strt, AK_stop, NK_curve, ZB_curve, depth_step, info);

    SAFEARRAYBOUND bound[2];
    bound[0].lLbound = 0;// эта фигня встречается слишком часто, надо бы отдельныу функции для преобразования сделать и их использовать 
    bound[0].cElements = res_cur_crop_a_b.size();
    bound[1].lLbound = 0;
    bound[1].cElements = res_cur_crop_a_b[0].size();

    SAFEARRAY* ret;
    LONG indexes[2];

    ret = SafeArrayCreate(
        VT_R8,
        2,
        bound
    );

    int min_row = bound[0].lLbound;
    int max_row = min_row + (int)bound[0].cElements - 1;

    int min_column = bound[1].lLbound;
    int max_column = min_column + (int)bound[1].cElements - 1;

    for (int row = min_row; row <= max_row; ++row)
    {
        indexes[0] = row;
        for (int column = min_column; column <= max_column; ++column)
        {
            if (column > res_cur_crop_a_b[row].size() - 1)
            {
                break;
            }
            indexes[1] = column;
            double Dtmp = res_cur_crop_a_b[row][column];
            SafeArrayPutElement(ret, indexes, &Dtmp);
        }
    }
    return ret;
}

SAFEARRAY* __stdcall ctx_get_curves_mtx_as_arr(Las* my_las_heap_pointer)
{
    //std::vector<std::string> mnemonics = my_las_heap_pointer->get_curves_mnemonics();

    std::vector<std::vector<double>> curve_mtx = my_las_heap_pointer->get_curves_mtx();

    SAFEARRAYBOUND bound[2];
    bound[0].lLbound = 0;
    bound[0].cElements = curve_mtx.size();
    bound[1].lLbound = 0;
    bound[1].cElements = curve_mtx[0].size();

    SAFEARRAY* ret;
    LONG indexes[2];

    ret = SafeArrayCreate(
        VT_R8,
        2,
        bound
    );

    int min_row = bound[0].lLbound;
    int max_row = min_row + (int)bound[0].cElements - 1;

    int min_column = bound[1].lLbound;
    int max_column = min_column + (int)bound[1].cElements - 1;

    for (int row = min_row; row <= max_row; ++row)
    {
        indexes[0] = row;
        for (int column = min_column; column <= max_column; ++column)
        {
            indexes[1] = column;
            //std::wstring wsTemp(vectr_2d_of_las_sections_info[row][column].begin(), vectr_2d_of_las_sections_info[row][column].end());
            //SafeArrayPutElement(ret, indexes, SysAllocString(wsTemp.c_str()));
            double Dtmp = curve_mtx[row][column]; //round till 3d char 
            SafeArrayPutElement(ret, indexes, &Dtmp);
        }
    }
    return ret;
}

SAFEARRAY* __stdcall ctx_get_las_curves_as_arr(Las* my_las_heap_pointer)
{
    std::vector<std::string> curves = my_las_heap_pointer->get_curves_mnemonics();

    SAFEARRAYBOUND dim;
    SAFEARRAY* ret;
    
    dim.lLbound = 0;
    dim.cElements = curves.size();

    ret = SafeArrayCreate(
        VT_BSTR,
        1, // Array to be returned has one dimension …
        &dim      // … which is described in this (dim) parameter
    );

    for (LONG index = 0; index < curves.size(); ++index)
    {
        std::wstring wsTemp(curves[index].begin(), curves[index].end());
        SafeArrayPutElement(ret, &index, SysAllocString(wsTemp.c_str()));
    }

    return ret;
}

SAFEARRAY* __stdcall ctx_get_las_sections_as_arr(Las* my_las_heap_pointer)
{
    std::vector<std::vector<std::string>> vectr_2d_of_las_sections_info = my_las_heap_pointer->print_las_to_2d_vector();

    SAFEARRAYBOUND bound[2];
    bound[0].lLbound = 0;
    bound[0].cElements = vectr_2d_of_las_sections_info.size();
    bound[1].lLbound = 0;
    bound[1].cElements = vectr_2d_of_las_sections_info[0].size();

    SAFEARRAY* ret;
    LONG indexes[2];

    ret = SafeArrayCreate(
        VT_BSTR,
        2,
        bound
    );

    int min_row = bound[0].lLbound;
    int max_row = min_row + (int)bound[0].cElements - 1;

    int min_column = bound[1].lLbound;
    int max_column = min_column + (int)bound[1].cElements - 1;

    for (int row = min_row; row <= max_row; ++row)
    {
        indexes[0] = row;
        for (int column = min_column; column <= max_column; ++column)
        {
            indexes[1] = column;
            std::wstring wsTemp(vectr_2d_of_las_sections_info[row][column].begin(), vectr_2d_of_las_sections_info[row][column].end());
            SafeArrayPutElement(ret, indexes, SysAllocString(wsTemp.c_str()));
        }
    }
    return ret;
}

void test_recive_laspointer(Las* my_las_pointer) {

}


//double global_a = 0;
//
//extern "C" LASRWDLL_API void __stdcall testdoubleset(double a)
//{
//    global_a = a * 10;
//}
//
//extern "C" LASRWDLL_API double __stdcall testdoubleget()
//{
//    return global_a;
//}