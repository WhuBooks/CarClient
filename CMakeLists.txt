cmake_minimum_required(VERSION 3.13)
project(CarClient)

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

# output paths settings
set(CMAKE_DEBUG_POSTFIX "_d" CACHE STRING "postfix applied to debug build of libraries")
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "postfix applied to release build of libraries")

# work out the postfix; required where we use OUTPUT_NAME
if(CMAKE_BUILD_TYPE MATCHES Release)
    set(EXE_POSTFIX ${CMAKE_RELEASE_POSTFIX})
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
    set(EXE_POSTFIX ${CMAKE_DEBUG_POSTFIX})
endif(CMAKE_BUILD_TYPE MATCHES Release)

if(WIN32)
    MESSAGE(STATUS "Compiling for Windows")
elseif(ANDROID)
    MESSAGE(STATUS "Compiling for Android")
elseif(APPLE)
    MESSAGE(STATUS "Compiling for OSX")
elseif(UNIX)
    SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")
    MESSAGE(STATUS "Compiling for Unix")
else()
    MESSAGE(STATUS "Platform can't be known")
endif(WIN32)

link_libraries(${LCM_LIB})

if(NOT WIN32)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
    set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
else()
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
    set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
endif(NOT WIN32)

MESSAGE(STATUS "Executable Output Path : ${EXECUTABLE_OUTPUT_PATH}")
MESSAGE(STATUS "Library Output Path : ${LIBRARY_OUTPUT_PATH}")

find_package(Mosquitto REQUIRED)
#MESSAGE(STATUS "Mosquitto Dir : ${MOSQUITTO_INCLUDE_DIR}")
#MESSAGE(STATUS "Mosquitto Lib : ${MOSQUITTO_LIBRARY}")
include_directories(${MOSQUITTO_INCLUDE_DIR})
link_libraries(${MOSQUITTO_LIBRARY})

include_directories(src)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(ThirdParty)

add_subdirectory(src)
add_subdirectory(app)

option(UseTest "Compile Test" ON)
MESSAGE(STATUS "Use Test or not : ${UseTest}")

if(UseTest)
    add_subdirectory(Test)
endif()