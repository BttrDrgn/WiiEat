@echo off
setlocal

rem Get the directory of the batch script
for %%I in ("%~dp0.") do set "script_dir=%%~fI"

rem Define source directories
set "src_dir=%script_dir%\src"
set "assets_dir=%script_dir%\assets"

rem Define temporary file to store current file structure
set "temp_file=%script_dir%\.files.tmp"

rem Check if the temporary file exists
if not exist "%temp_file%" (
    rem If the temporary file doesn't exist, create it and dump the file structure
    dir /s /b "%src_dir%" > "%temp_file%"
    dir /s /b "%assets_dir%" >> "%temp_file%"
    rem Set the temporary file as hidden
    attrib +h "%temp_file%"
    echo Initial file structure dumped.
) else (
    rem If the temporary file exists, compare it with the current file structure
    dir /s /b "%src_dir%" > "%temp_file%.new"
    dir /s /b "%assets_dir%" >> "%temp_file%.new"
    
    rem Compare the two file structures
    fc /b "%temp_file%" "%temp_file%.new" > nul
    
    rem If there's a difference, run "make clean"
    if errorlevel 1 (
        echo Changes detected. Running make clean...
        make clean
        rem Update the temporary file with the new file structure
        move /y "%temp_file%.new" "%temp_file%" > nul
    ) else (
        echo No changes detected.
        del "%temp_file%.new" > nul
    )
)

endlocal