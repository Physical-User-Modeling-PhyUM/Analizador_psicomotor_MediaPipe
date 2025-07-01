# PFG - Sistema Psicofísico de Análisis de Ejercicios

Este proyecto implementa una aplicación centrada en el análisis y corrección de ejercicios físicos utilizando visión por computador, inteligencia artificial y Qt.

---

## Requisitos Generales

- CMake >= 3.16
- C++17
- Qt 6 (Widgets, Core, Sql, Multimedia, Charts)
- OpenCV
- SQLite3
- nlohmann_json

---

## Compilación

### macOS

```bash
brew install cmake qt@6 opencv sqlite nlohmann-json
```

Luego:

```bash
cmake -B build -DCMAKE_PREFIX_PATH="$(brew --prefix qt@6);$(brew --prefix opencv)"
cmake --build build
```

> Asegúrese de usar el entorno correcto (Xcode o Ninja).

---

### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install cmake build-essential qt6-base-dev qt6-multimedia-dev                  qt6-charts-dev libopencv-dev libsqlite3-dev nlohmann-json-dev
```

```bash
cmake -B build
cmake --build build
```

---

### Windows (recomendado con vcpkg)

Instale [vcpkg](https://github.com/microsoft/vcpkg) y luego:

```bash
vcpkg install qt6-base qt6-multimedia qt6-charts opencv sqlite3 nlohmann-json
```

Luego configure CMake con el toolchain:

```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE=C:/ruta/a/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

> Use `Visual Studio` o `Ninja` como generador.

---

## Ejecutables

- `pfg`: aplicación principal
- `Test_Unit`: pruebas unitarias
- `Test_Integration`: pruebas de integración
- `Test_Functional`: pruebas funcionales con `FakeDBManager`

Puede ejecutar pruebas con:

```bash
./build/Test_Unit
./build/Test_Integration
./build/Test_Functional
```

---

## Estructura

- `src/`: código principal
- `test/unit/`: pruebas unitarias
- `test/Integracion/`: pruebas de integración
- `test/functional/`: pruebas funcionales
- `config/`: configuraciones necesarias (json/sql)
- `resources.qrc`: recursos embebidos

---

## Notas

- Asegúrese de que `config/`, `sounds/`, `VideoCapture.py` y `data.db` se encuentren en la ruta que espera tu aplicación (`AppController` lo gestiona).
- En Windows, asegúrese de que las librerias necesarias (Qt/OpenCV/sqlite) estén en el PATH o en el mismo directorio.

---
