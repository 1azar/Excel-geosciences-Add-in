Attribute VB_Name = "LAS_magi_module"
'**********************************************************
'* This module contains UDF and DLL functions for working
'* with data of LAS files
'**********************************************************

Private LAS_POINTER_OBJ As LongPtr
Private GLOBAL_LAS_PATH As String


'---------------------------------------------------------- DLL:


'**********************************************************
'*                  [DEVELOPING]
'* Returns a 2d Double array containing following colomns:
'* NK, a, b, DTP_calc, +DTP_real, ZB.
'**********************************************************
Private Declare PtrSafe Function ctx_restore_acoustic_curve_part _
        Lib "C:\Users\User\source\repos\LasRW_DLL\x64\Debug\LasRW_DLL.dll" _
        (ByVal las_obj_pointer As LongPtr, _
         ByRef AK_mnemonic As String, _
         ByRef AK_start As Double, _
         ByRef AK_stop As Double, _
         ByRef NK_mnemonic As String, _
         ByRef ZB_mnemonic As String, _
         ByRef depth_step As Double, _
         ByRef info As Integer) As Double()


'**********************************************************
'*                  [STABLE]
'* Returns a 1d double array containing data for
'* corresponding curve in las
'**********************************************************
Private Declare PtrSafe Function ctx_get_curve_by_mnem _
        Lib "C:\Users\User\source\repos\LasRW_DLL\x64\Debug\LasRW_DLL.dll" _
        (ByVal las_obj_pointer As LongPtr, ByRef mnemonic As String) As Double()
        

'**********************************************************
'*                  [STABLE]
'* Returns 2d array containing only ~A section of las file
'**********************************************************
Private Declare PtrSafe Function ctx_get_curves_mtx_as_arr _
        Lib "C:\Users\User\source\repos\LasRW_DLL\x64\Debug\LasRW_DLL.dll" _
        (ByVal las_obj_pointer As LongPtr) As Double()


'**********************************************************
'*                  [REFRACTORING IS REQUIRED]
'*
'**********************************************************
Private Declare PtrSafe Function ctx_get_las_curves_as_arr _
        Lib "C:\Users\User\source\repos\LasRW_DLL\x64\Debug\LasRW_DLL.dll" _
        (ByVal las_obj_pointer As LongPtr) As String()
        
        
'**********************************************************
'*                  [STABLE]
'* Returns a 2d String array containing information about
'* all sections except ~ASCII LOG DATA.
'**********************************************************
Private Declare PtrSafe Function ctx_get_las_sections_as_arr _
        Lib "C:\Users\User\source\repos\LasRW_DLL\x64\Debug\LasRW_DLL.dll" _
        (ByVal las_obj_pointer As LongPtr) As String()
        

'**********************************************************
'*                  [STABLE]
'* DLL helper function to ensure reentrancy.
'* Creates las objects and returns a pointer to it.
'**********************************************************
Private Declare PtrSafe Function creat_context_las _
        Lib "C:\Users\User\source\repos\LasRW_DLL\x64\Debug\LasRW_DLL.dll" _
        (ByRef path_to_las_file As String) As LongPtr
        
        
'**********************************************************
'*                  [STABLE]
'* DLL helper function to ensure reentrancy.
'* Deletes las objects by pointer.
'**********************************************************
Private Declare PtrSafe Sub delete_context_las _
        Lib "C:\Users\User\source\repos\LasRW_DLL\x64\Debug\LasRW_DLL.dll" _
        (ByVal las_obj_pointer As LongPtr)


'---------------------------------------------------------- UDF:


'**********************************************************
'*                  [STABLE]
'* Proxy function to work with creat_context_las().
'* Orders to creat_context_las() to create las object and
'* assigns returned pointer to global var: LAS_POINTER_OBJ.
'**********************************************************
Private Sub e_create_context_las(path_to_las As String)
    LAS_POINTER_OBJ = creat_context_las(path_to_las)
End Sub


'**********************************************************
'*                  [TESTED]
'* Proxy function to work with delete_context_las().
'* Orders to delete_context_las() to delete las object by
'* pointer and erases global var value: LAS_POINTER_OBJ.
'**********************************************************
Private Sub e_delete_context_las()
    Call delete_context_las(LAS_POINTER_OBJ)
    LAS_POINTER_OBJ = 0^
End Sub


'**********************************************************
'*                  [STABLE]
'* Restores AK curve in ZB(AK_strt_restore, AK_stop_restore)
'* by using NK curve as a regressor
'* Sets on sheet a table containing following columns:
'* NK, a, b, DTP_calc, +DTP_real, ZB
'**********************************************************
Function context_las_restore_acoustic_curve(path_to_las As String, _
                                            AK_mnemonic As String, _
                                            AK_start As Double, _
                                            AK_stop As Double, _
                                            NK_mnemonic As String, _
                                            ZB_mnemonic As String, _
                                            depth_step As Double)
    If (path_to_las <> GLOBAL_LAS_PATH) Then
        If LAS_POINTER_OBJ <> 0^ Then Call e_delete_context_las
        GLOBAL_LAS_PATH = path_to_las
        Call e_create_context_las(path_to_las)
    End If
    Dim v() As Double
    Dim info As Integer
    v = ctx_restore_acoustic_curve_part(LAS_POINTER_OBJ, _
                                        AK_mnemonic, _
                                        AK_start, _
                                        AK_stop, _
                                        NK_mnemonic, _
                                        ZB_mnemonic, _
                                        depth_step, _
                                        info)
    Call update_data_restore_acoustic_curve(v, Application.ActiveCell.Row, Application.ActiveCell.Column, ActiveWorkbook.Name, ActiveSheet.Name)
    Evaluate ("place_restore_acoustic_curve()")
    Debug.Print ("bb")
End Function


'**********************************************************
'*                  [STABLE]
'* Places a column containing curve data for
'* corresponding mnemonic
'**********************************************************
Function context_las_curves_by_name(path_to_las As String, mnemonic As String)
    If (path_to_las <> GLOBAL_LAS_PATH) Then
        If LAS_POINTER_OBJ <> 0^ Then Call e_delete_context_las
        GLOBAL_LAS_PATH = path_to_las
        Call e_create_context_las(path_to_las)
    End If
    Dim s2() As Double
    s2 = ctx_get_curve_by_mnem(LAS_POINTER_OBJ, mnemonic)
    If UBound(s2) = -1 Then
        context_las_curves_by_name = "NO " + "[" + mnemonic + "] CURVE" + " IN LAS " + "[" + path_to_las + "]"
    Else
        Call set_double_mtx_on_cell(s2, Application.Caller.Address, ActiveWorkbook.Name, ActiveSheet.Name, "ver")
'        Call update_data_curve_by_name(s2, Application.ActiveCell.ROW, Application.ActiveCell.COLUMN, ActiveWorkbook.name, ActiveSheet.name)
'        Evaluate ("place_curveArr_by_name_in_cells()")
    End If
End Function


'**********************************************************
'*                  [STABLE]
'* Sets table containig only ~A section of las file
'* {NO including mnemonic as first row}
'**********************************************************
Function context_las_curves_matrix(path_to_las As String)
    If (path_to_las <> GLOBAL_LAS_PATH) Then
        If LAS_POINTER_OBJ <> 0^ Then Call e_delete_context_las
        GLOBAL_LAS_PATH = path_to_las
        Call e_create_context_las(path_to_las)
    End If
'    context_las_curves (path_to_las)
    Dim s2() As Double
    s2 = ctx_get_curves_mtx_as_arr(LAS_POINTER_OBJ)
    'Dim new_cell_incremented_row As String
    'new_cell_incremented_row = Cells(range(Application.Caller.Address).ROW + 1, range(Application.Caller.Address).COLUMN).Address
    Call set_double_mtx_on_cell(s2, Application.Caller.Address, ActiveWorkbook.Name, ActiveSheet.Name, "ver")
End Function


'**********************************************************
'*                  [STABLE]
'* Returns all mnemonics being contained in las
'**********************************************************
Function context_las_curves(path_to_las As String)
    If (path_to_las <> GLOBAL_LAS_PATH) Then
        If LAS_POINTER_OBJ <> 0^ Then Call e_delete_context_las
        GLOBAL_LAS_PATH = path_to_las
        Call e_create_context_las(path_to_las)
    End If
    Dim s1() As String
    s1 = ctx_get_las_curves_as_arr(LAS_POINTER_OBJ)
    Call set_string_mtx_on_cell(s1, Application.Caller.Address, ActiveWorkbook.Name, ActiveSheet.Name)
End Function

 
'**********************************************************
'*                  [STABLE]
'* Sets table on sheet with all sections info excep ~A
'**********************************************************
Function context_las_section_ordered(path_to_las As String)
    If (path_to_las <> GLOBAL_LAS_PATH) Then
        If LAS_POINTER_OBJ <> 0^ Then Call e_delete_context_las
        GLOBAL_LAS_PATH = path_to_las
        Call e_create_context_las(path_to_las)
    End If
    Dim s1() As String
    s1 = ctx_get_las_sections_as_arr(LAS_POINTER_OBJ)
    Call set_string_mtx_on_cell(s1, Application.Caller.Address, ActiveWorkbook.Name, ActiveSheet.Name)
End Function
 
