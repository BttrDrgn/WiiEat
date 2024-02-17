@echo off
setlocal enabledelayedexpansion

rem Set the directory containing the .dol files
set "dol_directory=build\bin"

rem Get the first .dol file in the directory
for %%f in ("%dol_directory%\*.dol") do (
    set "first_dol=%%~f"
    goto :found
)
:found

rem Check if a .dol file was found
if "%first_dol%"=="" (
    echo No .dol files found in %dol_directory%
    exit /b 1
)

rem Run Dolphin with the first .dol file
start Dolphin -b -e "%first_dol%"