# Copyright (c) 2018 National Centre for Audiology. All right reserved.

# function that will fetch the git hash for this build
function(githash build_hash)
  find_package(Git)
  if(GIT_FOUND)
    execute_process(
      COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_HASH
      OUTPUT_STRIP_TRAILING_WHITESPACE
      )
  else()
    set(GIT_HASH GIT-NOTFOUND)
  endif()
  message(STATUS "Git Hash: ${GIT_HASH}")
  set(${build_hash} ${GIT_HASH} PARENT_SCOPE)
endfunction()
