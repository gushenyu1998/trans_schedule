# Build the app, using the HAL

include_directories(include)
file(GLOB MY_SOURCES "src/*.c")
add_executable(light_sampler ${MY_SOURCES})

# Make use of the HAL library
target_link_libraries(light_sampler LINK_PRIVATE hal)

# Use libnfc and Flite include directories
target_include_directories(light_sampler PRIVATE ${LIBNFC_INCLUDE_DIRS} ${FLITE_INCLUDE_DIRS})

# Link libnfc and Flite libraries
target_link_libraries(light_sampler PRIVATE ${LIBNFC_LIBRARIES} ${FLITE_LIBRARIES} m flite_usenglish flite_cmu_us_kal flite_cmulex flite)

# Specify library link directories (if needed)
target_link_directories(light_sampler PRIVATE ${LIBNFC_LIBRARY_DIRS} ${FLITE_LIBRARY_DIRS})

# Copy executable to final location (change `hello_world` to project name as needed)
add_custom_command(TARGET light_sampler POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy
        "$<TARGET_FILE:light_sampler>"
        "/home/parallels/cmpt433/public/"
        COMMENT "Copying ARM executable to public NFS directory")

