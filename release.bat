::Usage: release.bat <qmake_path> <upx_path>
@echo off
set ALLEGRO_INCLUDE=D:\Programming\Libs\allegro-5.0.9\include
set ALLEGRO_LIB=D:\Programming\Libs\allegro-5.0.9\lib
set BOOST_LIB=D:\Programming\Libs\boost_1_53_0_msvc10\stage\lib
set BOOST_INCLUDE=D:\Programming\Libs\boost_1_53_0_msvc10\

echo QMake: %1
echo UPX: %2
set qmake=%1
set upx_path=%2
set /p version_number=< VERSION
set base_dir=cbEnchanted_%version_number%
mkdir %base_dir%
cd %base_dir%
mkdir debug
mkdir release
cd ..

mkdir build_temp
echo Building release version 
cd build_temp
%qmake% ../qtcreator/cbEnchanted.pro -nocache -r -spec win32-msvc2010 "CBE_CONFIG=release no_testing"
nmake
xcopy release\cbEnchanted.exe ..\%base_dir%\release /Y

nmake clean
%qmake% ../qtcreator/cbEnchanted.pro -nocache -r -spec win32-msvc2010 "CBE_CONFIG=release"
nmake
xcopy release\cbEnchanted_test.exe ..\%base_dir%\release /Y
nmake clean

echo Building debug version
%qmake% ../qtcreator/cbEnchanted.pro -nocache -r -spec win32-msvc2010 "CBE_CONFIG=release_debug no_testing" "RELEASE_DIR=%base_dir%"
nmake
xcopy release\cbEnchanted.exe ..\%base_dir%\debug /Y
nmake clean
%qmake% ../qtcreator/cbEnchanted.pro -nocache -r -spec win32-msvc2010 "CBE_CONFIG=release_debug" "RELEASE_DIR=%base_dir%"
nmake
xcopy release\cbEnchanted_test.exe ..\%base_dir%\debug /Y
nmake clean
cd ..
echo Compressing release executable
%upx_path% ..\%base_dir%\release\cbEnchanted.exe --best --ultra-brute
%upx_path% ..\%base_dir%\release\cbEnchanted_test.exe --best --ultra-brute

xcopy tests\include\customfunctions.cb %base_dir% /Y
xcopy README-release.txt %base_dir% /Y

rmdir /s /q build_temp