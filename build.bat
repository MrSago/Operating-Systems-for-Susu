
@echo off

if ["%~1"] == [""] (
  echo "Syntax: .\build.bat [target]"
  exit
)

mkdir build 2>nul

cmake -DCMAKE_BUILD_TYPE:STRING=Release -H.\ -B.\build -G "MinGW Makefiles"
cmake --build build\ --config Release --target %~1 -j 4
