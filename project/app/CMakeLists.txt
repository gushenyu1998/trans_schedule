# Build the app, using the HAL

include_directories(include)
include_directories(../pru/)
file(GLOB MY_SOURCES "src/*.c")
add_executable(Project ${MY_SOURCES})

# PTHREAD support
find_package(Threads REQUIRED)
target_link_libraries(Project LINK_PRIVATE Threads::Threads)
find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)

include_directories(${CURL_INCLUDE_DIRS})
include_directories(${JSON-C_INCLUDE_DIRS})
# Make use of the HAL library
target_link_libraries(Project LINK_PRIVATE hal m ${CURL_LIBRARIES} ${JSON-C_LIBRARIES})

add_custom_command(TARGET Project POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:Project>"
     "~/cmpt433/public/myApps/Project" 
  COMMENT "Copying ARM executable to public NFS directory")
