#------	----------------------------------------------------------------#
#                        Build version 0.01                            #
#----------------------------------------------------------------------#
cmake_minimum_required(VERSION 2.8)

enable_testing()

set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall" ) 
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib )

set(PROJ_NAME test_impl)

set(CMAKE_FIND_LIBRARY_PREFIXES lib)
set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .dll)

include(FindBoost)
include(FindGLFW)

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
	# Set up linux search paths
	set(LIBRARY_SEARCH_PATH
	  /usr/local
	  /opt/local
	  /usr
	  /opt
	)


	set(HEADER_SEARCH_PATH
	  /usr/local
	  /opt/local
	  /usr
	  /opt
	)


	# Find OpenGL
	find_package(OpenGL)
	find_package(X11)

	# Find glfw library
	find_library(GLFW_LIBRARIES glfw3 ${LIBRARY_SEARCH_PATH})


	# Set additional libs
	set( ADDITIONAL_LIBS 
	     -lGLEW
	     -lglfw3
	     -lGL
	     -lX11
	     -lXcursor
	     -lm
	     -pthread
	     -lrt
	     -lXinerama
	     -lXrandr
	     -lXxf86vm
	     -lXi
	     -ldl
	)

	# Libraries needed on all platforms for this project
	set(LIBRARIES
		${OPENGL_LIBRARIES}
		${GLFW_LIBRARIES}
		${X11_LIBRARIES}
		# Neccessary libs to include 
		# from ${ADDITIONAL_LIBS};
		-lXxf86vm
		-pthread
		-ldl
		-lXrandr
		-lGLEW
		-lXcursor
		-lXi
		-lXinerama
		-lfreetype
		#${ADDITIONAL_LIBS}  
	)
endif()

#if(MSVC)
	# Set up windows search paths
	set(LIBRARY_SEARCH_PATH
		../boost_1_61_0/stage/lib
		./libs
		./libs/Windows
		./libs/Windows/freetype-2.6.5
		./libs/Windows/glew-2.0.0
		./libs/Windows/glfw-lib-vc2015
		./libs/Windows/glew-2.0.0/Win32
		./libs/Windows/glew-2.0.0/x64
	)
	# Find and set up boost 
	add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
	add_definitions(-DBOOST_ALL_DYN_LINK)
	set(Boost_DEBUG 1)
	set(Boost_USE_STATIC_LIBS OFF)
	set(Boost_USE_MULTITHREAD ON)
	set(Boost_USE_STATIC_RUNTIME OFF)
	set(BOOST_INCLUDEDIR C:/Users/jharr/Desktop/cxx/boost_1_61_0/boost)
	set(BOOST_LIBRARYDIR C:/Users/jharr/Desktop/cxx/boost_1_61_0/stage/lib)
	set(BOOSTROOT C:/Users/jharr/Desktop/cxx/boost_1_61_0)
	set(Boost_LIBRARIES libboost_filesystem-vc140-mt-gd-1_61.lib)
	find_package(Boost 1.61)

	if(Boost_FOUND)
		message(STATUS "Boost found!")
	endif()

	# Find OpenGL
	find_package(OpenGL)
	# Find X11 a part of OpenGL
	find_package(X11)
	# Find glfw library
	#find_library(GLFW_LIBRARIES glfw3)

	# Libraries needed on all platforms for this project
	set(LIBRARIES
		${OPENGL_LIBRARIES}
		-lglfw3
		${Boost_LIBRARIES}
		${X11_LIBRARIES}
		# Neccessary libs to include 
		# from ${ADDITIONAL_LIBS};
		-lXxf86vm
		-pthread
		-ldl
		-lXrandr
		-lGLEW
		-lXcursor
		-lXi
		-lXinerama
		-lfreetype
		#${ADDITIONAL_LIBS}  
	)
#endif(MSVC)

link_directories(${LIBRARY_SEARCH_PATH})

project(${PROJ_NAME})

include_directories(
	"../boost_1_61_0"
	"./include"
	"./"
)

file(GLOB source_ *.cpp)
file(GLOB gfx_source_ ${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp)
file(GLOB err_source_ ${CMAKE_CURRENT_SOURCE_DIR}/console/*.cpp)
file(GLOB io_source_ ${CMAKE_CURRENT_SOURCE_DIR}/file_manager/*.cpp)

# Add a target executable
add_executable(
	${PROJ_NAME}
	${source_}
	${gfx_source_}
	${err_source_}
	${io_source_}
)

# Libraries to be linked
target_link_libraries(
	${PROJ_NAME}
	${LIBRARIES}
)
