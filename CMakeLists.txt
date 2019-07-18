cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

OPTION(DEFINE_DEBUG
  "Build the project using debugging code"
  OFF)
IF(DEFINE_DEBUG)
  MESSAGE("Adding Debug flag...")
  ADD_DEFINITIONS(-DDEFINE_DEBUG)
  #SET(CMAKE_BUILD_TYPE Debug)
  MESSAGE("Build type is " ${CMAKE_BUILD_TYPE})
ENDIF(DEFINE_DEBUG)


set(SHORT_NAME repo-browser)
project(${SHORT_NAME})
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "repo-browser")
set(VITA_TITLEID  "rbdv00002")
set(VITA_VERSION  "02.50")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=gnu14 -Wall ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -Wall -Wno-psabi ")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(${SHORT_NAME}.elf
    src/main.cpp
    src/images.cpp
    src/vita-keyboard.cpp
    src/controls.cpp
    src/curl-tools.cpp
    src/json-tools.cpp
    src/file-io.cpp
    src/draw.cpp
    src/repo.cpp
    src/user.cpp
)

target_link_libraries(${SHORT_NAME}.elf
vita2d
ScePgf_stub
SceGxm_stub
SceCtrl_stub
SceTouch_stub
SceAppUtil_stub
SceDisplay_stub
SceLibKernel_stub
SceSysmodule_stub
SceSysmem_stub
ScePower_stub
SceCommonDialog_stub
SceRegistryMgr_stub
SceHid_stub
freetype
png
jpeg
SceAudio_stub
SceIofilemgr_stub
SceNet_stub
SceNetCtl_stub
SceHttp_stub
SceSsl_stub
debugnet
pthread
jansson
${CURL_LIBRARIES}
${CURL_LIBRARIES}
${OPENSSL_LIBRARIES}
c
m
z

)


vita_create_self(${SHORT_NAME}.self ${SHORT_NAME}.elf)

vita_create_vpk(${SHORT_NAME}.vpk ${VITA_TITLEID} ${SHORT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}

  FILE font.ttf font.ttf
  FILE images/user-background-list.png images/user-background-list.png
)
