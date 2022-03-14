#pragma once
#include "Las.h"

#include <string>
#include <WTypes.h>
#include <atlbase.h>
#include <windows.h>
#include <atlsafe.h>


#ifdef LASRWDLL_EXPORTS
#define LASRWDLL_API __declspec(dllexport)// Модификатор для объявлений функции. Этот модификатор предписывает компилятору и компоновщику экспортировать функцию или переменную из библиотеки DLL для использования другими приложениями.
#else
#define LASRWDLL_API __declspec(dllimport)// Этот модификатор оптимизирует импорт функции или переменной в приложении.
#endif

// This function return all data about sections of las file (crops the log section)


//extern "C" LASRWDLL_API BSTR __stdcall get_las_sections_info_as_string_var(LPCSTR* las_path);
	
extern "C" LASRWDLL_API SAFEARRAY* __stdcall get_las_sections_as_arr(LPCSTR* las_path);
//extern "C" LASRWDLL_API SAFEARRAY* __stdcall get_las_sections_info_as_string_array(LPCWSTR las_path);

//extern "C" LASRWDLL_API void __stdcall las_builder(LPCSTR * las_path);

//================test===========================================

extern "C" LASRWDLL_API Las * __stdcall creat_context_las(LPCSTR * las_path);

extern "C" LASRWDLL_API void __stdcall delete_context_las(Las * my_las_heap_pointer);

extern "C" LASRWDLL_API SAFEARRAY * __stdcall ctx_get_las_sections_as_arr(Las * my_las_heap_pointer);

extern "C" LASRWDLL_API void __stdcall test_recive_laspointer(Las * my_las_pointer);

extern "C" LASRWDLL_API SAFEARRAY * __stdcall ctx_get_las_curves_as_arr(Las * my_las_heap_pointer);

extern "C" LASRWDLL_API SAFEARRAY * __stdcall ctx_get_curves_mtx_as_arr(Las * my_las_heap_pointer);

extern "C" LASRWDLL_API SAFEARRAY * __stdcall ctx_get_curve_by_mnem(Las * my_las_heap_pointer, LPCSTR * mnem);

extern "C" LASRWDLL_API SAFEARRAY * __stdcall ctx_restore_acoustic_curve_part(	Las * my_las_heap_pointer, 
																				LPCSTR * AK_mnem, 
																				double& AK_strt, 
																				double& AK_stop,
																				LPCSTR * NK_mnem,
																				LPCSTR * ZB_mnem,
																				double& depth_step,
																				int& info);

// 
//extern "C" LASRWDLL_API void __stdcall testdoubleset(double a);
//extern "C" LASRWDLL_API double __stdcall testdoubleget();


//extern "C" LASRWDLL_API BSTR __stdcall bstr_c();

//extern "C" LASRWDLL_API BSTR __stdcall bstr();

//extern "C" LASRWDLL_API double get_square(double &x);