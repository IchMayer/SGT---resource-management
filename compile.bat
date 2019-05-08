cls
set pathVC="NULLPATH"
if exist "C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\Tools\VsDevCmd.bat" set pathVC="C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\Tools\VsDevCmd.bat" 
if exist "C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\Tools\VsDevCmd.bat" set pathVC="C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\Tools\VsDevCmd.bat" 
if exist "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat" set pathVC="C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\VsDevCmd.bat"  
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat" set pathVC="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"

if /I %pathVC%=="NULLPATH" EXIT
call %pathVC%
if exist *.exe del *.exe 
if exist *.dll del *.dll 
cl /EHsc /utf-8 /Zc:wchar_t Window.cpp stdafx.cpp kernel32.lib user32.lib gdi32.lib advapi32.lib 
cl /LD dllmain.cpp kernel32.lib user32.lib gdi32.lib advapi32.lib /link /out:SGT.dll 
del *.obj *.lib *.exp

call Window.exe