# CMakeList.txt : CMake project for helloLibevent, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)


project ("littleGift" VERSION 0.3)




# --------------------------------------------------------------------------
# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-D__STDC_CONSTANT_MACROS")



IF(MSVC)
    SET(CMAKE_CXX_FLAGS "/EHsc")  # deal with MSVC Warning C4530
ENDIF(MSVC)


######################################
#  zlib for httplib gzip.
######################################

set(ZLIB_LIBRARY "${PROJECT_SOURCE_DIR}/lib/zlibwapi.lib")

######################################
#  openSSL for httplib https.
######################################
set(LIBSSL_LIBRARY "${PROJECT_SOURCE_DIR}/lib/libssl.lib")
set(LIBCRYPTO_LIBRARY "${PROJECT_SOURCE_DIR}/lib/libcrypto.lib")



#-------------------------------------
#  PRE BUILD DEFINE
#-------------------------------------
if(WIN32)
	add_definitions(-DZLIB_WINAPI )
endif()



find_package(Threads REQUIRED) 

#-------------------------------------
#  spdlog settings
#-------------------------------------
include(./cmake/utils.cmake)



#-------------------------------------
#  config.h.in settings
#-------------------------------------

set(CPPHTTPLIB_THREAD_POOL_COUNT 64)
option(CPPHTTPLIB_ZLIB_SUPPORT "httplib gzip support" OFF)
option(CPPHTTPLIB_OPENSSL_SUPPORT "httplib ssl support" ONE)

set(SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE)
set(LOG_FILE_NAME "littleGift.log")
set(CA_CERT_FILE "./resources/ca-bundle.crt")

configure_file(
	"./cmake/config.h.in" 
	"${PROJECT_SOURCE_DIR}/include/config.h"
	@ONLY
)


######################################
#  Information
######################################
message(" ===================== Information ======================")

message(CMAKE_SOURCE_DIR         	    =  ${CMAKE_SOURCE_DIR})
message(PROJECT_SOURCE_DIR            =  ${PROJECT_SOURCE_DIR})
message(ZLIB_LIBRARY                  =  ${ZLIB_LIBRARY})
message(CPPHTTPLIB_ZLIB_SUPPORT       =  ${CPPHTTPLIB_ZLIB_SUPPORT})
message(CPPHTTPLIB_THREAD_POOL_COUNT  =  ${CPPHTTPLIB_THREAD_POOL_COUNT})


# Add source to this project's executable.
add_executable (${PROJECT_NAME} 
	"src/routes.cpp"
  "src/appUitls.cpp"
  "src/main.cpp" 
	"src/pch.cpp" 
	"src/tmptest.cpp" 
	"src/actions.cpp" 
	"src/pages.cpp" 
	"src/testActions.cpp" 
	"src/logger.cpp" 
	"src/base.cpp" "src/httpUtils.cpp")


target_include_directories( ${PROJECT_NAME}  
	PRIVATE 
		${PROJECT_SOURCE_DIR}/include
)


#spdlog_enable_warnings(${PROJECT_NAME})


target_link_libraries( ${PROJECT_NAME}  
	PRIVATE 
		${ZLIB_LIBRARY} 
		${LIBSSL_LIBRARY} 
		${ZLIB_LIBRARY} 
		${LIBCRYPTO_LIBRARY} 
)


if(WIN32)
    # Copy DLL files to output directory
	message("COPY ZLIB DLL")
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${PROJECT_SOURCE_DIR}/lib/zlibwapi.dll"              
			$<TARGET_FILE_DIR:${PROJECT_NAME}>
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${PROJECT_SOURCE_DIR}/lib/libcrypto-1_1-x64.dll"              
			$<TARGET_FILE_DIR:${PROJECT_NAME}>		
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${PROJECT_SOURCE_DIR}/lib/libssl-1_1-x64.dll"              
			$<TARGET_FILE_DIR:${PROJECT_NAME}>
	)
endif()

# Copy resource dir to output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD  
	COMMAND ${CMAKE_COMMAND} -E remove_directory   
		$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
	COMMAND ${CMAKE_COMMAND} -E copy_directory 
		"${PROJECT_SOURCE_DIR}/resources"              
		$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)



