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

set(SHORT_NAME RepoHub)
project(${SHORT_NAME})
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "RepoHub")
set(VITA_TITLEID  "REPOHUB01")
set(VITA_VERSION  "02.50")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=gnu11 -Wno-narrowing ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall -Wno-narrowing -fpermissive ")

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(BZip2 REQUIRED)

FUNCTION(ADD_RESOURCES out_var)
  SET(result)
  FOREACH(in_f ${ARGN})
  SET(out_f "${CMAKE_CURRENT_BINARY_DIR}/${in_f}.o")
  GET_FILENAME_COMPONENT(out_dir ${out_f} DIRECTORY)
  ADD_CUSTOM_COMMAND(OUTPUT ${out_f}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${out_dir}
    COMMAND ${CMAKE_LINKER} -r -b binary -o ${out_f} ${in_f}
    DEPENDS ${in_f}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Building resource ${out_f}"
    VERBATIM
    )
  LIST(APPEND result ${out_f})
  ENDFOREACH()
  SET(${out_var} "${result}" PARENT_SCOPE)
ENDFUNCTION()

file(GLOB res_files RELATIVE
  ${CMAKE_SOURCE_DIR}
  resources/head.bin
)

add_resources(vitashell_res ${res_files})

add_executable(${SHORT_NAME}
    ${vitashell_res}
    src/installAPP.cpp
    src/file.cpp
    src/headgen.cpp
    src/sfo.cpp
    src/main.cpp
    src/includes.cpp
    src/images.cpp
    src/vita-keyboard.cpp
    src/controls.cpp
    src/curl-tools.cpp
    src/json-tools.cpp
    src/file-io.cpp
    src/draw.cpp
    src/draw_templates.cpp
    src/release.cpp
    src/repo.cpp
    src/user.cpp
    src/search.cpp
)

target_link_libraries(${SHORT_NAME}
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
SceAppMgr_stub
debugnet
pthread
jansson
${CURL_LIBRARIES}
${CURL_LIBRARIES}
${OPENSSL_LIBRARIES}
zip
ScePromoterUtil_stub
ScePvf_stub
SceIme_stub
ssl
crypto
z
m
c
${BZIP2_LIBRARIES}

)


vita_create_self(${SHORT_NAME}.self ${SHORT_NAME} UNSAFE)

vita_create_vpk(${SHORT_NAME}.vpk ${VITA_TITLEID} ${SHORT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}

  FILE font.ttf font.ttf
  FILE images/defaultPic.png images/defaultPic.png
  FILE images/star.png images/star.png
  FILE images/fork.png images/fork.png

  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/pic0.png sce_sys/pic0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/star.png sce_sys/livearea/contents/star.png
  FILE sce_sys/livearea/contents/fork.png sce_sys/livearea/contents/fork.png
  FILE sce_sys/livearea/contents/logo.png sce_sys/livearea/contents/logo.png

  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml

)
