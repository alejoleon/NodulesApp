cmake_minimum_required(VERSION 2.8)
 
project(AlgoritmoBouma)
 
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
#if (ITKVtkGlue_LOADED)
  find_package(VTK REQUIRED)
  include(${VTK_USE_FILE})
#else()
 # find_package(ItkVtkGlue REQUIRED)
  #include(${ItkVtkGlue_USE_FILE})
  #set(Glue ItkVtkGlue)
#endif()

FILE(GLOB AlgoritmoBoumaSources ${PROJECT_SOURCE_DIR}/Sources/*.cpp ${PROJECT_SOURCE_DIR}/*.cpp)
FILE(GLOB AlgoritmoBoumaHeaders ${PROJECT_SOURCE_DIR}/Headers/*.h)

include_directories(
	${PROJECT_SOURCE_DIR}
    	${PROJECT_BINARY_DIR}
	${PROJECT_SOURCE_DIR}/Headers
	${PROJECT_SOURCE_DIR}/Sources
)


add_executable(Bouma ${AlgoritmoBoumaSources} ${AlgoritmoBoumaHeaders})
target_link_libraries(Bouma
 ${VTK_LIBRARIES} ${ITK_LIBRARIES})
