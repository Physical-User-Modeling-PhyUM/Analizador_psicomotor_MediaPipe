cmake_minimum_required(VERSION 3.16)

project(pfg VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Qt6Test_DIR "/Users/MZT/Qt/6.8.2/macos/lib/cmake/Qt6Test")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Test Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test Core)
find_package(OpenCV REQUIRED)
find_package(nlohmann_json REQUIRED)

# Incluir directorios
include_directories(${Qt6Widgets_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(/usr/local/opt/sqlite/include)
link_directories(/usr/local/opt/sqlite/lib)


# Definir fuentes del ejecutable principal (SIN TESTS)
set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/profiles/user.h src/profiles/user.cpp
    src/profiles/fitnesstrainer.h src/profiles/fitnesstrainer.cpp

    src/profiles/client.h src/profiles/client.cpp
    src/profiles/admin.h src/profiles/admin.cpp
    src/profiles/usermanager.h src/profiles/usermanager.cpp
    src/profiles/userpreferences.h src/profiles/userpreferences.cpp

    mediapipePy/VideoCapture.py
    src/pose/posemanager.h src/pose/posemanager.cpp
    src/pose/pose.h src/pose/pose.cpp
    src/pose/poseanalyzer.h src/pose/poseanalyzer.cpp
    src/pose/feedback.h src/pose/feedback.cpp
    config/poseConfig.json
    src/pose/state.h src/pose/state.cpp


)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(pfg MANUAL_FINALIZATION ${PROJECT_SOURCES}
        src/utils/jsonutils.h src/utils/jsonutils.cpp
        src/utils/imageutils.h src/utils/imageutils.cpp
        src/workouts/exerciseespec.h src/workouts/exerciseespec.cpp
        src/pose/statemachine.h src/pose/statemachine.cpp

        src/pose/condition.h
        src/pose/angleconstraint.h src/pose/angleconstraint.cpp
        src/workouts/exercise.h src/workouts/exercise.cpp
        src/workouts/trainingsesion.h src/workouts/trainingsesion.cpp
        src/pose/sesionreport.h src/pose/sesionreport.cpp

        src/repositories/userrepository.h src/repositories/userrepository.cpp
        src/db/dbmanager.h src/db/dbmanager.cpp
        src/db/dbtable.h src/db/dbtable.cpp
        src/profiles/Userdatatypes.h
        src/profiles/profiles.h
        src/profiles/profileRecords.h)
else()
    add_executable(pfg ${PROJECT_SOURCES})
endif()

# Enlazar librerías al ejecutable principal
target_link_libraries(pfg PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    ${OpenCV_LIBS}
    nlohmann_json::nlohmann_json
    sqlite3
)

# -------------------
# COMPILAR TESTS COMO EJECUTABLES SEPARADOS
# -------------------

enable_testing()  # Habilitar pruebas en CMake

# Test Unitarios (Enlaza con pose.cpp)
add_executable(Test_Unit
    test/unit/testpose.cpp
    src/pose/pose.cpp
    src/utils/jsonutils.cpp
    src/utils/imageutils.cpp
    test/unit/main.cpp
    test/unit/Sqlite3_test.cpp


)
target_link_libraries(Test_Unit PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    nlohmann_json::nlohmann_json
    ${OpenCV_LIBS}
    sqlite3
)

# Test de Funcionales (Enlaza con posemanager.cpp y pose.cpp)
add_executable(Test_Integration
    test/unit/testposemanager.cpp
    src/pose/posemanager.cpp
    src/pose/pose.cpp
    src/pose/poseanalyzer.cpp
    src/pose/feedback.cpp
)
target_link_libraries(Test_Integration PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    nlohmann_json::nlohmann_json
    ${OpenCV_LIBS}
    sqlite3
)

# Test funcional (Enlaza con posemanager.cpp y pose.cpp)
add_executable(Test_Functional  src/pose/posemanager.cpp src/pose/pose.cpp src/pose/poseanalyzer.cpp src/pose/feedback.cpp

    test/functional/functional_test_pose.h test/functional/functional_test_pose.cpp
    test/functional/main.cpp
)
target_link_libraries(Test_Functional PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core nlohmann_json::nlohmann_json ${OpenCV_LIBS})


# Registrar pruebas en CTest (opcional)
add_test(NAME PoseTest COMMAND TestPose)
add_test(NAME PoseManagerTest COMMAND TestPoseManager)
add_test(NAME PoseFunctionalTest COMMAND TestPoseFunctional)

# Instalación del ejecutable principal
include(GNUInstallDirs)
install(TARGETS pfg
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(pfg)
endif()
