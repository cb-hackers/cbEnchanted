mkdir build
cd build

nuget install AllegroDeps -Version 1.5.0.1 -ExcludeVersion -OutputDirectory .\packages
nuget install Allegro -Version 5.2.2.1 -ExcludeVersion -OutputDirectory .\packages

if "%BOOST_INCLUDE%" == "" goto install_boost
goto boost_installed
:install_boost
nuget install boost -Version 1.64.0 -ExcludeVersion -OutputDirectory .\packages
set BOOST_INCLUDE=%cd%\packages\boost\lib\native\include
:boost_installed

cmake -DWANT_TEST:BOOL=ON -G "Visual Studio 14 2015 Win64" ..