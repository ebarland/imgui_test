@ECHO OFF

xcopy /s /y ".\src\gfx\shaders\" ".\build\src\gfx\shaders\"

cmake -DCMAKE_BUILD_TYPE=%1 -G "NMake Makefiles" -S . -B ./build/
cmake --build ./build/ 

if "%~1"=="Debug" xcopy /s /y ".\external\db\debug\libcrypto-3-x64.dll" ".\build\" && xcopy /s /y ".\external\db\debug\libssl-3-x64.dll" ".\build\"
if "%~1"=="Release" xcopy /s /y ".\external\db\release\libcrypto-3-x64.dll" ".\build\" && xcopy /s /y ".\external\db\release\libssl-3-x64.dll" ".\build\"

xcopy /s /y ".\build\compile_commands.json" ".\"
