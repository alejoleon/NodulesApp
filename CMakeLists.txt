cmake_minimum_required(VERSION 2.8)
PROJECT(NodulesApp)

FIND_PACKAGE(ITK)
IF(NOT ITK_DIR)
  MESSAGE(FATAL_ERROR "Please set ITK_DIR.")
ENDIF(NOT ITK_DIR)
INCLUDE(${ITK_USE_FILE})

FIND_PACKAGE(VTK)
IF(NOT VTK_DIR)
  MESSAGE(FATAL_ERROR "Please set VTK_DIR.")
ENDIF(NOT VTK_DIR)
INCLUDE(${VTK_USE_FILE})

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

# Set your files and resources here
FILE(GLOB NodulesAppSources ${PROJECT_SOURCE_DIR}/View/Sources/*.cpp)
FILE(GLOB NodulesAppUI  ${PROJECT_SOURCE_DIR}/View/Uis/*.ui)
FILE(GLOB NodulesAppHeaders ${PROJECT_SOURCE_DIR}/View/Headers/*.h)
FILE(GLOB NodulesAppResources ${PROJECT_SOURCE_DIR}/Resources/*.qrc)
FILE(GLOB NodulesAppOtherHeaders ${PROJECT_SOURCE_DIR}/Model/Headers/*.h ${PROJECT_SOURCE_DIR}/View/Headers/*.h ${PROJECT_SOURCE_DIR}/Controller/Headers/*.h)
FILE(GLOB NodulesAppOtherSources ${PROJECT_SOURCE_DIR}/Model/Sources/*.cpp ${PROJECT_SOURCE_DIR}/Controller/Sources/*.cpp)

# The rest should just work (sure...)
QT4_WRAP_UI(NodulesAppUISources ${NodulesAppUI})
QT4_WRAP_CPP(NodulesAppMOCSources ${NodulesAppHeaders})
QT4_ADD_RESOURCES (SAMPLE_RC_SRCS ${NodulesAppResources} )

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/Controller/Headers
    ${PROJECT_SOURCE_DIR}/Controller/Sources
    ${PROJECT_SOURCE_DIR}/View/Sources
    ${PROJECT_SOURCE_DIR}/View/Uis
    ${PROJECT_SOURCE_DIR}/View/Headers
    ${PROJECT_SOURCE_DIR}/Model/Headers
    ${PROJECT_SOURCE_DIR}/Model/Sources


)

ADD_EXECUTABLE(
    NodulesApp
    ${NodulesAppSources}
    ${NodulesAppUISources}
    ${NodulesAppMOCSources}
    ${NodulesAppOtherHeaders}
    ${NodulesAppOtherSources}
    ${SAMPLE_RC_SRCS}
)

TARGET_LINK_LIBRARIES(
    NodulesApp
    ${ITK_LIBRARIES}
    ${VTK_LIBRARIES}
    ${QT_LIBRARIES}
    vtkInteractionImage
    vtkGUISupportQt
    vtkChartsCore
    vtkViewsContext2D
)
