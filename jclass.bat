
@echo off


setlocal


set CURRENTDIR=%cd%
set VERSION=%1


if [%VERSION%] == [] (
  set SUFFIX=
) else (
  set SUFFIX=-%VERSION:.=_%
)


set OUTPUTZIP=%CURRENTDIR%\jclass%SUFFIX%.zip
set OUTPUTINF=%CURRENTDIR%\pluginst.inf


if exist "%OUTPUTZIP%" (
  del /Q "%OUTPUTZIP%"
)


echo [plugininstall] > "%OUTPUTINF%"
echo description=JClass plugin allows Total Commander to view java .class files. >> "%OUTPUTINF%"
echo descriptionRus=JClass плагин для Total Commander позволяет просматривать содержимое файлов java .class. >> "%OUTPUTINF%"
echo type=wlx >> "%OUTPUTINF%"
echo file=jclass.wlx >> "%OUTPUTINF%"
echo defaultdir=jclass >> "%OUTPUTINF%"

if "%VERSION%" neq "" (
  echo version=%VERSION% >> "%OUTPUTINF%"
)


7z a -mx9 "%OUTPUTZIP%" "%OUTPUTINF%"

cd "%CURRENTDIR%\bin\x64\Release\"

7z a -mx9 "%OUTPUTZIP%" jclass.wlx64

cd "%CURRENTDIR%\bin\win32\Release\"

7z a -mx9 "%OUTPUTZIP%" jclass.wlx

cd "%CURRENTDIR%"


:exit


endlocal
