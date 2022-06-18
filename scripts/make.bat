@ECHO off
CLS

ECHO ---- Nebula Build Tool by Kevin Miller (2020) ----
ECHO --------------------------------------------------
ECHO.

IF [%1]==[] (
    ECHO Missing Parameter
    GOTO ErroneousInput
)

IF %1==c (
    GOTO NebClean
) ELSE IF %1==fc (
    GOTO NebFClean
) ELSE IF %1==g (
    GOTO NebGenerate
) ELSE IF %1==b (
    GOTO NebBuild
) ELSE IF %1==cg (
    GOTO NebCG    
) ELSE IF %1==cgb (
    GOTO NebCGB   
) ELSE IF %1==r (
    IF %2==ns ( 
        GOTO NebRunNS
    ) ELSE IF %2==ss (
        GOTO NebRunSS
    ) ELSE (
        ECHO Error! You have not entered a known build target. Try again.
        EXIT /B 0
    ) 
) ELSE (
    ECHO Unknown Parameter
    GOTO ErroneousInput
)

:ErroneousInput
ECHO -----------------------
ECHO Usage: make.bat [action] <flags>
ECHO.
ECHO Parameter - [action]
ECHO    c - Cleans project generated Files
ECHO    fc - Clean project genned files AND premake genned files
ECHO    g - Generate VS2019 Project
ECHO    b - Build the project
ECHO    cg - FULL Clean and Generate
ECHO    cgb - FULL Clean, Generate, and Build 
ECHO    r - Runs the project if it is made
ECHO Flags
ECHO    -r - Run a build (Only works if a build has been done)
ECHO.
EXIT /B 0
  

REM -----------------------------------------------------------------------------
:NebGenerate
REM Gen
PUSHD %~DP0
PUSHD ..\

ECHO - Making VS 2019 Project
ECHO.
ECHO ---- Premake Output ----
scripts\premake5.exe vs2019
ECHO ---- End Premake Output ----
ECHO.

POPD
POPD
EXIT /B 0

REM -----------------------------------------------------------------------------
:NebClean
REM Clean
PUSHD %~DP0
PUSHD ..\

ECHO - Cleaning up 
RMDIR /S /Q  bin bin-int .vs >nul 2>&1
ECHO - Done Cleaning 
ECHO.

POPD
POPD
EXIT /B 0

REM -----------------------------------------------------------------------------
:NebFClean
REM Clean
PUSHD %~DP0
PUSHD ..\

ECHO - (Full) Cleaning up 
RMDIR /S /Q bin bin-int .vs >nul 2>&1
DEL /Q *.exe *.ilk *.pdb *.sln *.vc* >nul 2>&1
ECHO - Done Cleaning 
ECHO.

POPD
POPD
EXIT /B 0

REM -----------------------------------------------------------------------------
:NebRunNS
PUSHD %~DP0
PUSHD ..\NebulaStudio
IF NOT EXIST ..\bin\Debug\windowsx86_64\NebulaStudio\NebulaStudio.exe (
    ECHO !! You have not compiled the application yet or complilation was unsuccessful !!
    ECHO.
    EXIT /B 0
)
START CMD /C ..\bin\Debug\windowsx86_64\NebulaStudio\NebulaStudio.exe
EXIT /B 0

:NebRunSS
PUSHD %~DP0
PUSHD ..\SpaceSim
IF NOT EXIST ..\bin\Debug\windowsx86_64\SpaceSim\SpaceSim.exe (
    ECHO !! You have not compiled the application yet or complilation was unsuccessful !!
    ECHO.
    EXIT /B 0
)
START /WAIT ..\bin\Debug\windowsx86_64\SpaceSim\SpaceSim.exe
EXIT /B 0

REM -----------------------------------------------------------------------------
:NebCG
REM Clean and Generate
PUSHD %~DP0
PUSHD ..\

ECHO - Cleaning up 
RMDIR /S /Q project_data bin bin-int .vs >nul 2>&1
DEL /Q *.exe *.ilk *.pdb *.sln *.vcxproj* >nul 2>&1
ECHO - Done Cleaning
ECHO - Making VS 2019 Project
ECHO.
ECHO ---- Premake Output ----
scripts\premake5.exe vs2019
ECHO ---- End Premake Output ----
ECHO.

POPD
POPD
EXIT /B 0


REM -----------------------------------------------------------------------------
:NebCGB
REM Clean, Generate, and Build
PUSHD %~DP0
PUSHD ..\

ECHO - Cleaning up 
RMDIR /S /Q project_data bin bin-int .vs >nul 2>&1
DEL /Q *.exe *.ilk *.pdb *.sln *.vcxproj* >nul 2>&1
ECHO - Done Cleaning
ECHO - Making VS 2019 Project
ECHO.
ECHO ---- Premake Output ----
ECHO BITCHES
.\scripts\premake5.exe vs2019
ECHO ---- End Premake Output ----
ECHO.

POPD
POPD

REM THIS FUNCTION GOE INTO THE BUILD FUNCTION INTENTIONALY BUT DO NOT MOVE THE BELOW FUNCTION
REM -----------------------------------------------------------------------------
:NebBuild
REM Build

PUSHD %~DP0
PUSHD ..\

REM If the vs proj dne, try to build it
IF NOT EXIST "Nebula.sln" (
    ECHO Error! You forgot to generate the VS2019 project first.
    ECHO Please run [make.bat gen] before proceeding
    ECHO.
    EXIT /B 0
)

IF EXIST ..\bin\Debug\windowsx86_64\NebulaStudio\NebulaStudio.exe (
    DEL /Q *.exe
)

REM Build using msbuild
ECHO - Building with MSVC
IF EXIST "c:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" (
    "c:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" /m Nebula.sln
) ELSE IF EXIST "c:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\MSBuild\Current\Bin\MSBuild.exe" (
    "c:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\MSBuild\Current\Bin\MSBuild.exe" /m Nebula.sln
)
POPD
POPD

IF [%3]==[] (
    ECHO Error! You have not entered a known build target. Try again.
    EXIT /B 0
) 

IF %3==ns ( 
    GOTO NebRunNS
) ELSE IF %3==ss (
    GOTO NebRunSS
) ELSE (
    ECHO Error! You have not entered a known build target. Try again.
)   
EXIT /B 0