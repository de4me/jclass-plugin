
@echo off

set CURRENTDIR=%cd%

del "%CURRENTDIR%\jclass%1.zip"

7z a -mx9 "%CURRENTDIR%\jclass%1.zip" "%CURRENTDIR%\pluginst.inf"

cd "%CURRENTDIR%\bin\x64\Release\"

7z a -mx9 -w "%CURRENTDIR%/jclass%1.zip" jclass.wlx64

cd "%CURRENTDIR%\bin\win32\Release\"

7z a -mx9 -w "%CURRENTDIR%/jclass%1.zip" jclass.wlx

cd "%CURRENTDIR%"