@ECHO OFF

cmake -DCMAKE_BUILD_TYPE=%1 -G "NMake Makefiles" -S . -B ./build/
cmake --build ./build/ 

xcopy /s /y ".\build\compile_commands.json" ".\"
