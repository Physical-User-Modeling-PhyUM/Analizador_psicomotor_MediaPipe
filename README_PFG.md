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
---

## Empaquetado del Proyecto

El proyecto incluye un sistema de empaquetado mediante **CPack**, que genera automáticamente un archivo `.zip` listo para distribución.

### 🔧 Requisitos

Antes de empaquetar, asegúrate de:

1. Haber compilado el proyecto al menos una vez.
2. Tener `CMake` ≥ 3.16 correctamente instalado.
3. Que la instalación de Qt y OpenCV esté detectada por `cmake`.

###  ¿Cómo generar el paquete ZIP?

1. Abre una terminal y accede al **directorio de compilación creado por Qt Creator**. En macOS, normalmente se encuentra en:

```bash
cd build/Desktop_x86_darwin_generic_mach_o_64bit-Debug
```

2. Ejecuta el siguiente comando:

```bash
cmake --build . --target package
```

> Asegúrate de usar `.` como directorio actual. **No uses `--build build` desde dentro del build.**

3. El archivo ZIP generado se ubicará en:

```
build/Desktop_x86_darwin_generic_mach_o_64bit-Debug/package/pfg-0.1.0-*.zip
```

###  El paquete contiene:

- El ejecutable `pfg`
- La base de datos `data.db` con usuarios y datos de prueba
- Script de captura `VideoCapture.py`
- Archivos de configuración (`poseConfig.json`, `schema.sql`)
- Carpeta `sounds/` con sonidos de retroalimentación

---

## Base de Datos de Prueba

El archivo `data.db` incluido contiene usuarios por defecto para propósitos de evaluación y demostración:

| Usuario    | Tipo         | Contraseña |
|------------|--------------|------------|
| cliente1   | Cliente      | 1234       |
| cliente2   | Cliente      | 1234       |
| cliente3   | Cliente      | 1234       |
| trainer1   | Entrenador   | 1234       |
| admin1     | Administrador| 1234       |

Estos usuarios pueden utilizarse tras el login inicial para explorar las funcionalidades desde diferentes roles.

---

## Documentación Generada

El proyecto incluye documentación técnica generada automáticamente con **Doxygen**. Esta documentación proporciona:

- Descripción estructurada del código (clases, métodos, atributos).
- Diagramas UML generados a partir del código fuente.
- Referencias cruzadas y navegación interactiva.

Puedes acceder a la documentación desde el siguiente archivo HTML:

 [`docs/html/index.html`](docs/html/index.html)
 
[`UML/html/index.html`](docs/html/index.html)

Para regenerar la documentación localmente, ejecuta:

```bash
doxygen Doxyfile
```

> Requiere tener instalado Doxygen (`brew install doxygen` o `sudo apt install doxygen`).

