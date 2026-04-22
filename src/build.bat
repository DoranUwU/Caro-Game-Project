@echo off
setlocal enabledelayedexpansion

echo Đang gom các file C++ trong thu muc hien tai...

:: Tim truc tiep cac file .cpp (khong co chu src\ nua)
set SRCS=
for %%f in (*.cpp) do set SRCS=!SRCS! %%f

echo Cac file se bien dich: !SRCS!
echo ----------------------------------------

:: Truyen vao g++. Chu y cac duong dan libs da duoc them ..\ de lui ra ngoai
g++ -std=c++17 -O2 -I. -I..\libs\raylib\include -I..\libs\nlohmann !SRCS! -L..\libs\raylib\lib -lraylib -lopengl32 -lgdi32 -lwinmm -o game.exe

if %ERRORLEVEL% EQU 0 (
    echo ----------------------------------------
    echo BIEN DICH THANH CONG! Dang chay game...
    echo ----------------------------------------
    game.exe
) else (
    echo ----------------------------------------
    echo LOI BIEN DICH! Hay kiem tra lai code C++.
    echo ----------------------------------------
)