# Build the app, using the HAL

include_directories(include)
file(GLOB MY_SOURCES "src/*.c")
add_executable(display ${MY_SOURCES})

# Make use of the HAL library
target_link_libraries(display LINK_PRIVATE hal)

# Copy executable to final location (change `display` to project name as needed)
add_custom_command(TARGET display POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:display>"
     "~/cmpt433/public/myApps/display" 
  COMMENT "Copying ARM executable to public NFS directory")
