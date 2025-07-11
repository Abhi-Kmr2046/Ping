cmake_minimum_required(VERSION 3.9.1)
project(Ping)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
add_subdirectory(${CMAKE_BINARY_DIR}/src/client)
add_subdirectory(${CMAKE_BINARY_DIR}/src/server)

add_custom_target(serverrun
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/server
  DEPENDS server
  COMMENT "Running Server"
)
add_custom_target(clientrun
  COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/client
  DEPENDS client
  COMMENT "Running Client"
)