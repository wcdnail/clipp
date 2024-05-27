@echo off

rem Var section -------------------------------------------------------------------------

rem set BUILD_CONF=Release
set BUILD_CONF=Debug

call %CD%\mk-vars.cmd

rem Compiler section --------------------------------------------------------------------
set ACE_CMAKE_GENERATOR=%MSVC_CMAKE_GENERATOR%
set ACE_C_COMPILER=%MSVC_C_COMPILER%
set ACE_CXX_COMPILER=%MSVC_CXX_COMPILER%

rem -------------------------------------------------------------------------------------
pushd %SOURCE_DIR%
cd ..
mkdir %BUILD_DIR% 2>NUL
cd %BUILD_DIR%

rem -------------------------------------------------------------------------------------
rem -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" ^
chcp.com 65001 >NUL
cmake -G %ACE_CMAKE_GENERATOR% ^
-DCMAKE_BUILD_TYPE:STRING="%BUILD_CONF%" ^
-DCMAKE_CONFIGURATION_TYPES:STRING="%BUILD_CONF%" ^
-DCMAKE_C_COMPILER:FILEPATH="%ACE_C_COMPILER%" ^
-DCMAKE_CXX_COMPILER:FILEPATH="%ACE_CXX_COMPILER%" ^
-DCMAKE_RC_COMPILER:FILEPATH="%MSVC_RC_COMPILER%" ^
-DBLD_COMPUTER_NAME="%COMPUTER_NAME%" ^
-DCMAKE_INSTALL_PREFIX="INSTALLED" ^
-DBUILD_TESTING=1 ^
%SOURCE_DIR%

IF '%ERRORLEVEL%'=='0' GOTO CMAKE_OK
set RV=%ERRORLEVEL%
echo ERRORLEVEL: %RV%
pause
popd
exit /B %RV%

rem -------------------------------------------------------------------------------------
:CMAKE_OK
start "clipp" clipp.sln
popd
