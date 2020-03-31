# CMakeList.txt : CMake project for helloLibevent, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)


project ("littleGift" VERSION 0.3)


option(CPPHTTPLIB_ZLIB_SUPPORT "httplib support gzip" OFF)


# --------------------------------------------------------------------------
# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-D__STDC_CONSTANT_MACROS")

set(CPPHTTPLIB_THREAD_POOL_COUNT "64")

IF(MSVC)
    SET(CMAKE_CXX_FLAGS "/EHsc")  # deal with MSVC Warning C4530
ENDIF(MSVC)


######################################
#  zlib for httplib gzip.
######################################

set(ZLIB_LIBRARY "${PROJECT_SOURCE_DIR}/lib/zlibwapi.lib")



######################################
#  information
######################################

message(CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR})
message(PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR})
message(ZLIB_LIBRARY=${ZLIB_LIBRARY})
message(SPDLOG_LIBRARY=${SPDLOG_LIBRARY})
message(CPPHTTPLIB_ZLIB_SUPPORT=${CPPHTTPLIB_ZLIB_SUPPORT})


#-------------------------------------
#  PRE BUILD DEFINE
#-------------------------------------
if(WIN32)
	add_definitions(-DZLIB_WINAPI )
endif()
#add_definitions(-DCPPHTTPLIB_ZLIB_SUPPORT )



find_package(Threads REQUIRED) 

#-------------------------------------
#  spdlog settings
#-------------------------------------
include(./cmake/utils.cmake)



configure_file(
	"config.h.in" 
	"config.h"
)


# Add source to this project's executable.
add_executable (${PROJECT_NAME} 
	"src/helloLittleGift.cpp"
  "src/appUitls.cpp"
  "src/main.cpp" 
	"src/pch.cpp")


target_include_directories( ${PROJECT_NAME}  
	PRIVATE 
		${PROJECT_BINARY_DIR}
		${PROJECT_SOURCE_DIR}/include
)


#spdlog_enable_warnings(${PROJECT_NAME})


target_link_libraries( ${PROJECT_NAME}  
	PRIVATE 
		${ZLIB_LIBRARY} 
)


if(WIN32)
    # Copy DLL files to output directory
	message("COPY ZLIB DLL")
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/lib/zlibwapi.dll"              
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()




