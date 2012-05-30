project(Glog)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})

INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/extern/glog/
  ${CMAKE_SOURCE_DIR}/extern/glog/windows
)

SET(Glog_SOURCES
  vlog_is_on.cc
  utilities.cc
  raw_logging.cc
  logging.cc
  windows/port.cc
)

SET(Glog_HEADERS
  base/commandlineflags.h
  base/googleinit.h
  base/mutex.h
  windows/glog/log_severity.h
  windows/glog/logging.h
  windows/glog/raw_logging.h
  windows/glog/stl_logging.h
  windows/glog/vlog_is_on.h
  windows/config.h
  windows/port.h
  utilities.h
)

add_library(Glog STATIC ${Glog_SOURCES} ${Glog_HEADERS})