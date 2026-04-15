# 🎮 Caro Game (C++ + raylib)

A simple Caro (Gomoku) game built with C++ using raylib and nlohmann/json.

---

## 📦 Project Structure

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
├── *.cpp / *.h
└── main.cpp
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

### 💻 Build manually via Terminal (Optional)

If you prefer using the terminal (PowerShell/CMD), you can compile the game directly from the project root by running:

```powershell
g++ -std=c++17 src/*.cpp -I. -Ilibs -Ilibs/nlohmann -Ilibs/raylib/include -Llibs/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm -o game.exe
```

Then run the game:

```powershell
./game.exe
```

---

## ❗ Notes

* All include paths and libraries are already configured in `tasks.json` for VSCode users.

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
