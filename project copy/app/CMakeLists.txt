## CMakeLists.txt for App Section

include_directories(include)
include_directories(../pru/)

file(GLOB MY_SOURCES "src/*.c")

add_executable(Project ${MY_SOURCES})

find_package(Threads REQUIRED)
target_link_libraries(Project LINK_PRIVATE Threads::Threads)
find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBNFC REQUIRED libnfc)

include_directories(${CURL_INCLUDE_DIRS})
include_directories(${JSON-C_INCLUDE_DIRS})

target_link_libraries(Project LINK_PRIVATE hal m ${CURL_LIBRARIES} ${JSON-C_LIBRARIES})
target_link_libraries(Project PRIVATE ${CURL_LIBRARIES} ${JSON-C_LIBRARIES})
target_include_directories(Project PRIVATE ${LIBNFC_INCLUDE_DIRS})
target_link_libraries(Project PRIVATE ${LIBNFC_LIBRARIES} m)
target_link_directories(Project PRIVATE ${LIBNFC_LIBRARY_DIRS} )
target_include_directories(Project PRIVATE ${LIBNFC_INCLUDE_DIRS} ${FLITE_INCLUDE_DIRS})

# Link libnfc and Flite libraries
target_link_libraries(Project PRIVATE ${LIBNFC_LIBRARIES} ${FLITE_LIBRARIES} flite_usenglish flite_cmu_us_kal flite_cmulex flite m)

# Specify library link directories (if needed)
target_link_directories(Project PRIVATE ${LIBNFC_LIBRARY_DIRS} ${FLITE_LIBRARY_DIRS})
add_custom_command(TARGET Project POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy
        "$<TARGET_FILE:Project>"
        "~/cmpt433/public/myApps/Project"
        COMMENT "Copying ARM executable to public NFS directory")