Attribute VB_Name = "test_mtx_module"


Public Function generate_matrix(h As Integer, w As Integer)
    Dim my_arr() As Variant
    ReDim my_arr(0 To w, 0 To h)
    For i = 0 To UBound(my_arr, 1)
        For j = 0 To UBound(my_arr, 2)
            my_arr(i, j) = CStr(j) & " " & CStr(i)
        Next j
    Next i
    
    Call set_variant_mtx_on_cell(my_arr, Application.Caller.Address, ActiveWorkbook.Name, ActiveSheet.Name)

End Function

