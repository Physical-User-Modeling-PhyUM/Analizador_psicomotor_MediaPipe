cmake_minimum_required(VERSION 3.16)
project(pfg VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(APPLE)
    set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk")
    set(Qt6Test_DIR "$ENV{HOME}/Qt/6.8.2/macos/lib/cmake/Qt6Test")
elseif(WIN32)

elseif(UNIX)
    include_directories(/usr/local/opt/sqlite/include)
endif()

set(CMAKE_BUILD_RPATH_USE_ORIGIN ON)

# Dependencias
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Test Core Sql Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test Core Sql Multimedia Sql Charts)
find_package(OpenCV REQUIRED)
find_package(nlohmann_json REQUIRED)

# Includes
include_directories(${Qt6Widgets_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src)

set(PROJECT_SOURCES
    src/main.cpp
    src/profiles/user.h
    src/profiles/user.cpp
    src/profiles/fitnesstrainer.h
    src/profiles/fitnesstrainer.cpp
    src/profiles/client.h
    src/profiles/client.cpp
    src/profiles/admin.h
    src/profiles/admin.cpp
    src/profiles/userpreferences.h
    src/profiles/userpreferences.cpp
    mediapipePy/VideoCapture.py
    src/pose/pose.h
    src/pose/pose.cpp
    src/pose/feedback.h
    src/pose/feedback.cpp
    src/pose/state.h
    src/pose/state.cpp
    config/poseConfig.json
    src/utils/jsonutils.h
    src/utils/jsonutils.cpp
    src/utils/imageutils.h
    src/utils/imageutils.cpp
    src/workouts/exerciseespec.h
    src/workouts/exerciseespec.cpp
    src/pose/statemachine.h
    src/pose/statemachine.cpp
    src/pose/condition.h
    src/pose/angleconstraint.h
    src/pose/angleconstraint.cpp
    src/workouts/trainingsesion.h
    src/workouts/trainingsesion.cpp
    src/workouts/trainingworkout.h
    src/workouts/trainingworkout.cpp
    src/pose/sesionreport.h
    src/pose/sesionreport.cpp
    src/repositories/userrepository.h
    src/repositories/userrepository.cpp
    src/db/dbtable.h
    src/db/dbtable.cpp
    src/profiles/profiles.h
    src/enums/MobilityLevelEnum.h
    src/enums/FeedbackModeEnum.h
    src/enums/GenderEnum.h
    src/enums/UserTypeEnum.h
    src/enums/PreferencesFieldEnum.h
    src/enums/enums.h
    src/profiles/clientprofile.h
    src/profiles/clientprofile.cpp
    src/repositories/trainingrepository.h
    src/repositories/trainingrepository.cpp
    src/enums/UserFieldEnum.h
    src/repositories/metricsrepository.h
    src/repositories/metricsrepository.cpp
    src/metrics/performancemetric.h
    src/metrics/performancemetric.cpp
    src/enums/TypeBDEnum.h
    src/core/appcontroller.h
    src/core/appcontroller.cpp
    src/core/dbmanager.h
    src/core/dbmanager.cpp
    src/core/trainingmanager.h
    src/core/trainingmanager.cpp
    src/core/posemanager.h
    src/core/posemanager.cpp
    src/core/usermanager.h
    src/core/usermanager.cpp
    src/core/loginmanager.h
    src/core/loginmanager.cpp
    src/enums/ExEspecFieldEnum.h
    src/enums/WorkoutFieldEnum.h
    src/workouts/workoutsummary.h
    src/workouts/workoutsummary.cpp
    src/workouts/exercisesummary.h
    src/workouts/exercisesummary.cpp
    src/ui/main/mainwindow.h
    src/ui/main/mainwindow.cpp
    src/ui/main/mainwindow.ui
    src/ui/main/loginwidget.h
    src/ui/main/loginwidget.cpp
    src/ui/main/loginwidget.ui
    src/ui/main/mainboardwidget.h
    src/ui/main/mainboardwidget.cpp
    src/ui/main/mainboardwidget.ui
    src/ui/init_loader.h
    src/ui/init_loader.cpp
    src/ui/fitnesstrainer/trainerboardwidget.h
    src/ui/fitnesstrainer/trainerboardwidget.cpp
    src/ui/fitnesstrainer/trainerboardwidget.ui
    src/ui/main/uitablemodel.h
    src/ui/main/uitablemodel.cpp
    src/utils/uiutils.h
    src/utils/uiutils.cpp
    src/ui/main/comboboxdelegate.h
    src/ui/main/comboboxdelegate.cpp
    src/ui/fitnesstrainer/assignworkoutdialog.h
    src/ui/fitnesstrainer/assignworkoutdialog.cpp
    src/ui/fitnesstrainer/assignworkoutdialog.ui
    src/ui/fitnesstrainer/trainerworkoutdesignwidget.h
    src/ui/fitnesstrainer/trainerworkoutdesignwidget.cpp
    src/ui/fitnesstrainer/trainerworkoutdesignwidget.ui
    src/ui/fitnesstrainer/trainerclientinfowidget.h
    src/ui/fitnesstrainer/trainerclientinfowidget.cpp
    src/ui/fitnesstrainer/trainerclientinfowidget.ui
    src/ui/fitnesstrainer/trainerworkoutassignwidget.h
    src/ui/fitnesstrainer/trainerworkoutassignwidget.cpp
    src/ui/fitnesstrainer/trainerworkoutassignwidget.ui
    src/ui/fitnesstrainer/trainerinfowidget.h
    src/ui/fitnesstrainer/trainerinfowidget.cpp
    src/ui/fitnesstrainer/trainerinfowidget.ui
    src/enums/profileEnums.h
    src/enums/profileEnums.cpp
    src/ui/client/userclienttrainingsesionwidget.h
    src/ui/client/userclienttrainingsesionwidget.cpp
    src/ui/client/userclienttrainingsesionwidget.ui
    src/ui/client/userclientprofilewidget.h
    src/ui/client/userclientprofilewidget.cpp
    src/ui/client/userclientprofilewidget.ui
    src/ui/client/userclientinfowidget.h
    src/ui/client/userclientinfowidget.cpp
    src/ui/client/userclientinfowidget.ui
    src/ui/client/userclientmainboardwidget.h
    src/ui/client/userclientmainboardwidget.cpp
    src/ui/client/userclientmainboardwidget.ui
    src/ui/client/userclientsesionexecution.h
    src/ui/client/userclientsesionexecution.cpp
    src/ui/client/userclientsesionexecution.ui
    src/ui/client/feedbackgraphwidget.h
    src/ui/client/feedbackgraphwidget.cpp
    src/ui/client/feedbackgraphwidget.ui
    src/core/soundfeedbackmanager.h
    src/core/soundfeedbackmanager.cpp
    src/core/validationmanager.h
    src/core/validationmanager.cpp
    src/enums/PoseViewEnum.h
    src/core/metricsmanager.h
    src/core/metricsmanager.cpp
    src/ui/fitnesstrainer/trainermetricsmainwidget.h
    src/ui/fitnesstrainer/trainermetricsmainwidget.cpp
    src/ui/fitnesstrainer/trainermetricsmainwidget.ui
    src/ui/metrics/heatmapwidget.h
    src/ui/metrics/heatmapwidget.cpp
    src/ui/metrics/heatmapwidget.ui
    src/ui/metrics/timewidget.h
    src/ui/metrics/timewidget.cpp
    src/ui/metrics/timewidget.ui
    src/ui/metrics/angleviewwidget.h
    src/ui/metrics/angleviewwidget.cpp
    src/ui/metrics/angleviewwidget.ui
    src/ui/metrics/logviewerwidget.h
    src/ui/metrics/logviewerwidget.cpp
    src/ui/metrics/logviewerwidget.ui
    src/ui/admin/adminmainboardwidget.h
    src/ui/admin/adminmainboardwidget.cpp
    src/ui/admin/adminmainboardwidget.ui
    src/ui/admin/appconfigwidget.h
    src/ui/admin/appconfigwidget.cpp
    src/ui/admin/appconfigwidget.ui
    src/ui/admin/usersadminwidget.h
    src/ui/admin/usersadminwidget.cpp
    src/ui/admin/usersadminwidget.ui
    src/ui/client/userclientconfigwidget.h
    src/ui/client/userclientconfigwidget.cpp
    src/ui/client/userclientconfigwidget.ui
    resources.qrc
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(pfg MANUAL_FINALIZATION ${PROJECT_SOURCES})
else()
    add_executable(pfg ${PROJECT_SOURCES})
endif()

target_link_libraries(pfg PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::Sql
    Qt6::Charts
    ${OpenCV_LIBS}
    nlohmann_json::nlohmann_json
    sqlite3
)


include(GNUInstallDirs)
install(TARGETS pfg
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(pfg)
endif()


file(GLOB_RECURSE DOXYGEN_DOCS "docs/html/*")
source_group("Documentation" FILES ${DOXYGEN_DOCS})

# -------------------------
# Test Unitario
# -------------------------

set(TEST_UNIT_SOURCES
    src/utils/jsonutils.cpp
    src/utils/imageutils.cpp
    src/enums/profileEnums.h
    src/enums/profileEnums.cpp
    src/core/dbmanager.cpp
    src/db/dbtable.cpp
    src/pose/state.cpp
    src/pose/angleconstraint.cpp
    src/pose/pose.cpp
    src/workouts/exercisesummary.cpp
    src/workouts/exerciseespec.cpp
    src/workouts/trainingworkout.cpp
    src/workouts/workoutsummary.cpp
    src/profiles/user.cpp
    src/profiles/fitnesstrainer.cpp
    src/profiles/client.cpp
    src/profiles/admin.cpp
    src/profiles/userpreferences.cpp
    src/profiles/clientprofile.cpp
    test/unit/main.cpp
    test/unit/testpose.cpp
    test/unit/testexerciseespec.h test/unit/testexerciseespec.cpp
    test/unit/testdbtable.h test/unit/testdbtable.cpp
    test/unit/Sqlite3_test.cpp
    test/unit/testdb.h test/unit/testdb.cpp
    test/unit/testworkoutsummary.h test/unit/testworkoutsummary.cpp
    test/unit/testtrainingworkout.h test/unit/testtrainingworkout.cpp
    test/unit/testadmin.cpp test/unit/testadmin.h
    test/unit/testclient.cpp test/unit/testclient.h
    test/unit/testclientprofile.cpp test/unit/testclientprofile.h
    test/unit/testfitnesstrainer.cpp test/unit/testfitnesstrainer.h
    test/unit/testuser.cpp test/unit/testuser.h
    test/unit/testuserpreferences.cpp test/unit/testuserpreferences.h
)
add_executable(Test_Unit ${TEST_UNIT_SOURCES})
target_link_libraries(Test_Unit PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    nlohmann_json::nlohmann_json
    ${OpenCV_LIBS}
    Qt6::Sql
    sqlite3
)

# -------------------------
# Test Integración
# -------------------------
set(TEST_INTEGRATION_SOURCES
    src/repositories/userrepository.cpp
    src/repositories/trainingrepository.cpp
    src/core/dbmanager.cpp
    src/db/dbtable.cpp
    src/core/usermanager.cpp
    src/core/trainingmanager.cpp
    src/core/validationmanager.cpp
    src/workouts/trainingworkout.cpp
    src/workouts/exerciseespec.cpp
    src/workouts/exercisesummary.cpp
    src/workouts/trainingsesion.cpp
    src/workouts/workoutsummary.cpp
    src/pose/state.cpp
    src/pose/angleconstraint.cpp
    src/pose/sesionreport.cpp
    src/pose/condition.h
    src/profiles/user.cpp
    src/profiles/client.cpp
    src/profiles/admin.cpp
    src/profiles/fitnesstrainer.cpp
    src/profiles/clientprofile.cpp
    src/profiles/userpreferences.cpp
    src/pose/statemachine.h
    src/pose/statemachine.cpp
    src/pose/pose.cpp
    src/pose/feedback.cpp
    src/enums/enums.h
    src/enums/UserTypeEnum.h
    src/enums/userFieldEnum.h
    src/enums/profileEnums.h
    src/enums/profileEnums.cpp
    test/Integracion/testintegracionuserrepositorydb.h
    test/Integracion/testintegracionuserrepositorydb.cpp
    test/Integracion/testintegraciontrainingrepositorydb.h
    test/Integracion/testintegraciontrainingrepositorydb.cpp
    test/Integracion/main.cpp
    test/Integracion/testintegracionusermanagerdb.h
    test/Integracion/testintegracionusermanagerdb.cpp
    test/Integracion/testintegraciontrainingmanagerdb.h
    test/Integracion/testintegraciontrainingmanagerdb.cpp
    src/ui/main/uitablemodel.cpp
    src/ui/main/comboboxdelegate.cpp
    src/utils/uiutils.cpp
    test/Integracion/dummyposemanager.h
    test/Integracion/dummyposemanager.cpp
    test/Integracion/testintegraciondummyposemanager.h
    test/Integracion/testintegraciondummyposemanager.cpp
)

add_executable(Test_Integration ${TEST_INTEGRATION_SOURCES})
target_link_libraries(Test_Integration PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Multimedia
    nlohmann_json::nlohmann_json
    ${OpenCV_LIBS}
    Qt6::Sql
    sqlite3
)

# -------------------------
# Test Funcional
# -------------------------
set(TEST_FUNCTIONAL_SOURCES
    test/functional/main.cpp
    test/functional/testdb.h
    test/functional/testdb.cpp
    test/functional/testuserrepository.h
    test/functional/testuserrepository.cpp
    test/functional/fakedbmanager.h
    test/functional/fakedbmanager.cpp
    test/functional/testtrainingrepository.h
    test/functional/testtrainingrepository.cpp
    src/core/dbmanager.cpp
    src/db/dbtable.cpp
    src/repositories/userrepository.cpp
    src/profiles/user.cpp
    src/profiles/client.cpp
    src/profiles/admin.cpp
    src/profiles/fitnesstrainer.cpp
    src/profiles/clientprofile.cpp
    src/profiles/userpreferences.cpp
    src/repositories/trainingrepository.cpp
    src/workouts/trainingworkout.cpp
    src/workouts/exerciseespec.cpp
    src/pose/state.cpp
    src/pose/angleconstraint.cpp
    src/pose/sesionreport.cpp
    src/workouts/trainingsesion.cpp
    src/workouts/exercisesummary.cpp
    src/workouts/workoutsummary.cpp
    src/pose/condition.h
    src/enums/profileEnums.h
    src/enums/profileEnums.cpp
)

add_executable(Test_Functional ${TEST_FUNCTIONAL_SOURCES})
target_link_libraries(Test_Functional PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    nlohmann_json::nlohmann_json
    Qt6::Sql
    ${OpenCV_LIBS}
)
