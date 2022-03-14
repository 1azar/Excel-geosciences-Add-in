VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "BOOKs_EVENT_HANDLERS"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = True


Private WithEvents App As Application
Attribute App.VB_VarHelpID = -1

'Private Sub App_WorkbookOpen(ByVal Wb As Workbook)
'    MsgBox ("You have opend the " & Wb.Name)
'End Sub

Private Sub App_AfterCalculate()
    
    If ((-1 = Not GLOBAL_VARIANT_MTX) And (-1 = Not GLOBAL_STRING_MTX) And (-1 = Not GLOBAL_DOUBLE_MTX)) Then
        'Debug.Print ("GLOBAL_MTX is empty")
    Else
        Dim target_range As String
        Dim init_row As Integer
        Dim init_col As Integer
        init_row = Range(GLOBAL_CELL_ADDRESS).Row
        init_col = Range(GLOBAL_CELL_ADDRESS).Column
        
        If (-1 <> Not GLOBAL_VARIANT_MTX) Then
            If (GLOBAL_DEMENSION_MTX = 1) Then
                If (GLOBAL_ORIENTATION = "hor") Then
                    target_range = get_range_as_str(init_row, init_col, init_row, init_col + UBound(GLOBAL_VARIANT_MTX, 1))
                Else
                    target_range = get_range_as_str(init_row, init_col, init_row + UBound(GLOBAL_VARIANT_MTX, 1), init_col)
                End If
            Else
                If (GLOBAL_DEMENSION_MTX = 2) Then
                    target_range = get_range_as_str(init_row, init_col, init_row + UBound(GLOBAL_VARIANT_MTX, 1), init_col + UBound(GLOBAL_VARIANT_MTX, 2))
                End If
            End If
            If (GLOBAL_LPF_evented_first <> False) Then
                GLOBAL_LPF_evented_first = False
                If (GLOBAL_ORIENTATION = "hor") Then
                    Workbooks(GLOBAL_BOOK).Sheets(GLOBAL_SHEET).Range(target_range).Value2 = GLOBAL_VARIANT_MTX
                Else
                    Workbooks(GLOBAL_BOOK).Sheets(GLOBAL_SHEET).Range(target_range).Value2 = WorksheetFunction.Transpose(GLOBAL_VARIANT_MTX)
                End If
            End If
        End If
        
        If (-1 <> Not GLOBAL_STRING_MTX) Then
            If (GLOBAL_DEMENSION_MTX = 1) Then
                If (GLOBAL_ORIENTATION = "hor") Then
                    target_range = get_range_as_str(init_row, init_col, init_row, init_col + UBound(GLOBAL_STRING_MTX, 1))
                Else
                    target_range = get_range_as_str(init_row, init_col, init_row + UBound(GLOBAL_STRING_MTX, 1), init_col)
                End If
            Else
                If (GLOBAL_DEMENSION_MTX = 2) Then
                    target_range = get_range_as_str(init_row, init_col, init_row + UBound(GLOBAL_STRING_MTX, 1), init_col + UBound(GLOBAL_STRING_MTX, 2))
                End If
            End If
            If (GLOBAL_LPF_evented_first <> False) Then
                GLOBAL_LPF_evented_first = False
                If (GLOBAL_ORIENTATION = "hor") Then
                    Workbooks(GLOBAL_BOOK).Sheets(GLOBAL_SHEET).Range(target_range).Value2 = GLOBAL_STRING_MTX
                Else
                    Workbooks(GLOBAL_BOOK).Sheets(GLOBAL_SHEET).Range(target_range).Value2 = WorksheetFunction.Transpose(GLOBAL_STRING_MTX)
                End If
            End If
        End If
        
        If (-1 <> Not GLOBAL_DOUBLE_MTX) Then
            If (GLOBAL_DEMENSION_MTX = 1) Then
                If (GLOBAL_ORIENTATION = "hor") Then
                    target_range = get_range_as_str(init_row, init_col, init_row, init_col + UBound(GLOBAL_DOUBLE_MTX, 1))
                Else
                    target_range = get_range_as_str(init_row, init_col, init_row + UBound(GLOBAL_DOUBLE_MTX, 1), init_col)
                End If
            Else
                If (GLOBAL_DEMENSION_MTX = 2) Then
                    'target_range = get_range_as_str(init_row, init_col, init_row + UBound(GLOBAL_DOUBLE_MTX, 1), init_col + UBound(GLOBAL_DOUBLE_MTX, 2))
                    target_range = get_range_as_str(init_row, init_col, init_row + UBound(GLOBAL_DOUBLE_MTX, 2), init_col + UBound(GLOBAL_DOUBLE_MTX, 1)) 'returned logs arrays from dll are transposed (make transpose sub in macros or leave it as it is here)
                End If
            End If
            If (GLOBAL_LPF_evented_first <> False) Then
                GLOBAL_LPF_evented_first = False
                If (GLOBAL_ORIENTATION = "hor") Then
                    Workbooks(GLOBAL_BOOK).Sheets(GLOBAL_SHEET).Range(target_range).Value2 = GLOBAL_DOUBLE_MTX
                Else
                    Workbooks(GLOBAL_BOOK).Sheets(GLOBAL_SHEET).Range(target_range).Value2 = WorksheetFunction.Transpose(GLOBAL_DOUBLE_MTX)
                End If
            End If
        End If
        
        Erase GLOBAL_VARIANT_MTX
        Erase GLOBAL_STRING_MTX
        Erase GLOBAL_DOUBLE_MTX
        
        GLOBAL_ORIENTATION = ""
        GLOBAL_CELL_ADDRESS = ""
        GLOBAL_BOOK = ""
        GLOBAL_SHEET = ""
        GLOBAL_DEMENSION_MTX = 0
    End If
End Sub

Private Sub Workbook_Open()
    Set App = Application
End Sub

Function get_range_as_str(ByRef start_row As Integer, ByRef start_column As Integer, ByRef end_row As Integer, ByRef end_column As Integer) As String
    '1,2,10,20 -> "B1:T10
    Dim result_range As String
    result_range = Get_Letter_by_Number_(start_column) + CStr(start_row) + ":" + Get_Letter_by_Number_(end_column) + CStr(end_row)
    get_range_as_str = result_range
End Function

Private Function Get_Letter_by_Number_(lngCol As Integer) As String
    '1 -> "A", 2 -> "B", 4 -> "D", -3 -> "A"
    Dim vArr
    If lngCol >= 1 Then
    vArr = Split(Cells(1, lngCol).Address(True, False), "$")
    Get_Letter_by_Number_ = CStr(vArr(0))
    Else
        Debug.Print ("Cant define the letter for column nuber - " + CStr(lngCol) + "!")
        Get_Letter_by_Number_ = "A"
    End If
End Function

