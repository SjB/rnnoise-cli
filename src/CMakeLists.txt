# Build script for openMHA WavPlayer

find_package(SndFile)
find_package(rnnoise)

if(RNNOISE_FOUND AND SNDFILE_FOUND)
  set(RNNOISE_CLI_SRCS rnnoise-cli.c)

  include_directories(${SNDFILE_INCLUDE_DIRS}
    ${RNNOISE_INCLUDE_DIRS})

  add_executable(rnnoise-cli ${RNNOISE_CLI_SRCS})
  target_link_libraries(rnnoise-cli
    ${SNDFILE_LIBRARIES}
    ${RNNOISE_LIBRARIES})

endif(RNNOISE_FOUND AND SNDFILE_FOUND)

