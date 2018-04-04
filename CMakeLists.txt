# Copyright (c) 2018 National Centre for Audiology. All right reserved.
#
# Build script for openMHA WavPlayer

cmake_minimum_required(VERSION 3.0)
project(rnnoise-cli)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

option(ENABLE_PRERELEASE_TAG "Enable semver pre-release tag" TRUE)
option(SNDFILE_SOURCE_DIR "Location of the openMHA source codes" )
option(RNNOISE_SOURCE_DIR "Location of the openMHA source codes" )

set(rnnoise_cli_VERSION_MAJOR 0 CACHE STRING "openMHA WavPlayer Plugin major version")
set(rnnoise_cli_VERSION_MINOR 1 CACHE STRING "openMHA WavPlayer Plugin minor version")
set(rnnoise_cli_VERSION_PATCH 2 CACHE STRING "openMHA WavPlayer Plugin patch version")

set(rnnoise_cli_VERSION
  ${rnnoise_cli_VERSION_MAJOR}.${rnnoise_cli_VERSION_MINOR}.${rnnoise_cli_VERSION_PATCH})

if(ENABLE_PRERELEASE_TAG)
    include(githash)
    githash(rnnoise_cli_GIT_HASH)
    if(rnnoise_cli_GIT_HASH)
        set(rnnoise_cli_VERSION ${rnnoise_cli_VERSION}+build.${rnnoise_cli_GIT_HASH})
    endif()
endif()

find_program(CLANG_FORMAT clang-format ENV PATH DOC "Using astyle to format codes")

include_directories(${PROJECT_BINARY_DIR})

add_subdirectory(src)
