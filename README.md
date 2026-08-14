# Caro Game (C++ + raylib)


## Thông tin thành viên

* 24120123 - Võ Hoàng Phúc
* 24120200 - Lê Bảo Minh
* 24120238 - Phạm Anh Tuấn

##  Cấu trúc file

```
src/
│
├── .vscode/
│   ├── c_cpp_properties.json
│   └── tasks.json
│
├── assets/
├── libs/
│   ├── raylib/
│   └── nlohmann/
│
├── src/
|   ├── *.cpp / *.h
|   └── main.cpp
```

---

## Cài đặt môi trường

* MinGW (g++)
* Raylib
* nlohmann/json

---

## Chạy chương trình

### Nhấn đúp chuột vào file game.exe

### Build và chạy trên VSCode

```
g++ -std=c++17 -O2 -I.\src -I.\libs\raylib\include -I.\libs\nlohmann .\src\*.cpp -L.\libs\raylib\lib -lraylib -lopengl32 -lgdi32 -lwinmm -o .\game.exe
```

3. Chạy trò chơi:

```
./game.exe
```
Hoặc  
```
Nhấn đúp chuột vào file game.exe ở trong thư mục chính.
```
---

#

## Tính năng

* Chơi game Caro (Gomoku)
* Save / Load trạng thái game (JSON)
* Giao diện với thư viện raylib 
---

