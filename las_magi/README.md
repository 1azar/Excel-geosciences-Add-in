Las Magi - an Excel add-in that allows you to work with .LAS files and perform calculus. 
All add-in features descriptions are on the [wiki](https://github.com/1azar/Excel-geosciences-Add-in/wiki/LAS-MAGI#functions)
### Instalation
* Create a folder on drive ```C:\``` called ```Excel-geosciences-Add-in```
* Create a folder ```las_magi``` inside the previous folder
* Download [LasRW_DLL.dll](https://github.com/1azar/Excel-geosciences-Add-in/blob/main/las_magi/LasRW_DLL.dll) and [Las_magi_add-in.xlam](https://github.com/1azar/Excel-geosciences-Add-in/blob/main/las_magi/Las_magi_add-in.xlam) then save them in ```C:\Excel-geosciences-Add-in\las_magi\```
* Open the Excel workbook. In menu bar go to: ```File -> Options -> Add-ins -> Go.. -> Browse``` and select ```C:\Excel-geosciences-Add-in\las_magi\Las_magi_add-in.xlam```
* Make sure the checkbox next to the ```Las_Magi_Add-in``` is enabled and click ```OK```
* At this point, the add-in should be installed and able to work in any open workbook. To check the correctness of the add-in, write ```=Generate_matrix(3;3)``` in any cell, this function will insert 4x4 matrix on the sheet.

### Removal
* In Excels menu bar go to: ```File -> Options -> Add-ins -> Go..``` and unable the check box next to ```Las_Magi_Add-in``` then click ```OK```
