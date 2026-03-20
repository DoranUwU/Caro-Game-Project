# Caro Game (C++ + raylib)


## Thông tin thành viên

* 24120123 - Võ Hoàng Phúc
* 24120200 - Lê Bảo Minh
* 24120238 - Phạm Anh Tuấn

## 📦 Cấu trúc file

```
Caro_vscode/
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

## ⚙️ Requirements

* MinGW (g++)
* VSCode with C/C++ extension

---

## 🚀 How to Run

### ✅ Build & Run (Recommended)

1. Open project in VSCode
2. Press:

```
Ctrl + Shift + B
```

→ This will automatically compile the project using the predefined `tasks.json`

3. Run the game:

```
./game.exe
```

---

## ❗ Notes

* No need to compile manually with `g++` commands
* All include paths and libraries are already configured in:

  * `.vscode/tasks.json`
  * `.vscode/c_cpp_properties.json`

---

## 📚 Libraries Used

* raylib (graphics & input)
* nlohmann/json (save/load game data)

---

## 💾 Features

* Play Caro (Gomoku)
* Save / Load game state (JSON)
* Basic UI rendering with raylib

---

## 🛠️ Troubleshooting

### ❌ Cannot find `json.hpp`

Make sure folder structure is:

```
libs/nlohmann/nlohmann/json.hpp
```

---

### ❌ Build errors

* Ensure MinGW is installed correctly
* Check `compilerPath` in `c_cpp_properties.json`

---
