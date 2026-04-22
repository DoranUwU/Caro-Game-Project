@echo off
setlocal enabledelayedexpansion

echo Đang gom các file C++...

:: Vòng lặp tìm tất cả các file .cpp trong thư mục src và gom vào biến SRCS
set SRCS=
for %%f in (src\*.cpp) do set SRCS=!SRCS! %%f

echo Các file sẽ biên dịch: !SRCS!
echo ----------------------------------------

:: Gọi g++ với danh sách file đã gom
g++ -std=c++17 -O2 -Isrc -Ilibs\raylib\include -Ilibs\nlohmann !SRCS! -Llibs\raylib\lib -lraylib -lopengl32 -lgdi32 -lwinmm -o game.exe

:: Kiểm tra xem biên dịch có thành công không
if %ERRORLEVEL% EQU 0 (
    echo ----------------------------------------
    echo BIÊN DỊCH THÀNH CÔNG! Đang chạy game...
    echo ----------------------------------------
    game.exe
) else (
    echo ----------------------------------------
    echo LỖI BIÊN DỊCH! Hãy kiểm tra lại code C++.
    echo ----------------------------------------
)