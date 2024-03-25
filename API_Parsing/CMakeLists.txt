cmake_minimum_required(VERSION 3.0.0)
project(trans_schdule VERSION 0.1.0 LANGUAGES C)


find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSON-C REQUIRED json-c)

include_directories(${CURL_INCLUDE_DIRS})
include_directories(${JSON-C_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES} ${JSON-C_LIBRARIES})
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_custom_command(TARGET trans_schdule POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:trans_schdule>"
     "~/cmpt433/public/myApps/trans_schdule" 
  COMMENT "Copying ARM executable to public NFS directory")
