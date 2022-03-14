Attribute VB_Name = "MTX_module"


Public GLOBAL_VARIANT_MTX() As Variant 'indices must start from 0
Public GLOBAL_STRING_MTX() As String 'indices must start from 0
Public GLOBAL_DOUBLE_MTX() As Double 'indices must start from 0

Public GLOBAL_CELL_ADDRESS As String
Public GLOBAL_BOOK As String
Public GLOBAL_SHEET As String
Public GLOBAL_LPF_evented_first As Boolean

Public GLOBAL_DEMENSION_MTX As Integer
Public GLOBAL_ORIENTATION As String


Public Sub set_variant_mtx_on_cell(ByRef mtx() As Variant, _
                            ByRef cell As String, _
                            ByRef book_name As String, _
                            ByRef sheet_name As String, _
                            Optional ByRef orientation As String = "hor") ' "hor" or "ver"
    GLOBAL_VARIANT_MTX = mtx
    GLOBAL_DEMENSION_MTX = getDimension_variant(mtx)
    GLOBAL_ORIENTATION = orientation
    
    GLOBAL_CELL_ADDRESS = cell
    GLOBAL_BOOK = book_name
    GLOBAL_SHEET = sheet_name
    GLOBAL_LPF_evented_first = True
    
    Erase GLOBAL_STRING_MTX ' this operations also carried in the BOOK.
    Erase GLOBAL_DOUBLE_MTX
End Sub

Public Sub set_string_mtx_on_cell(ByRef mtx() As String, _
                            ByRef cell As String, _
                            ByRef book_name As String, _
                            ByRef sheet_name As String, _
                            Optional ByRef orientation As String = "hor") ' "hor" or "ver"
    GLOBAL_STRING_MTX = mtx
    GLOBAL_DEMENSION_MTX = getDimension_string(mtx)
    GLOBAL_ORIENTATION = orientation
    
    GLOBAL_CELL_ADDRESS = cell
    GLOBAL_BOOK = book_name
    GLOBAL_SHEET = sheet_name
    GLOBAL_LPF_evented_first = True
    
    Erase GLOBAL_VARIANT_MTX ' this operations also carried in the BOOK.
    Erase GLOBAL_DOUBLE_MTX
End Sub

Public Sub set_double_mtx_on_cell(ByRef mtx() As Double, _
                            ByRef cell As String, _
                            ByRef book_name As String, _
                            ByRef sheet_name As String, _
                            Optional ByRef orientation As String = "hor") ' "hor" or "ver"
    GLOBAL_DOUBLE_MTX = mtx
    GLOBAL_DEMENSION_MTX = getDimension_double(mtx)
    GLOBAL_ORIENTATION = orientation
    
    GLOBAL_CELL_ADDRESS = cell
    GLOBAL_BOOK = book_name
    GLOBAL_SHEET = sheet_name
    GLOBAL_LPF_evented_first = True
    
    Erase GLOBAL_VARIANT_MTX ' this operations also carried in the BOOK.
    Erase GLOBAL_STRING_MTX
End Sub

Private Function getDimension_variant(ByRef var() As Variant) As Integer
    On Error GoTo Err
    Dim i As Integer
    Dim tmp As Integer
    i = 0
    Do While True
        i = i + 1
        tmp = UBound(var, i)
    Loop
Err:
    getDimension_variant = i - 1
End Function

Private Function getDimension_string(ByRef var() As String) As Integer
    On Error GoTo Err
    Dim i As Integer
    Dim tmp As Integer
    i = 0
    Do While True
        i = i + 1
        tmp = UBound(var, i)
    Loop
Err:
    getDimension_string = i - 1
End Function

Function getDimension_double(ByRef var() As Double) As Integer
    On Error GoTo Err
    Dim i As Integer
    Dim tmp As Integer
    i = 0
    Do While True
        i = i + 1
        tmp = UBound(var, i)
    Loop
Err:
    getDimension_double = i - 1
End Function

Private Sub Workbook_Open()

    ' DO NOT REMOVE THIS FUNCTION

End Sub



