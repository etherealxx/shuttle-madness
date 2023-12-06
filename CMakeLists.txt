cmake_minimum_required(VERSION 3.5)

project(ShuttleMadness)

# Add the source files to the executable
file(GLOB SOURCES "*.cpp")

# Add the header files to the project
file(GLOB HEADERS "*.h")

# Add resource files
file(GLOB RESOURCES "*.wav" "*.png")

# Add texts files
set(TEXTS
    shuttle_howtoplay.txt
    shuttle_audio_credit.txt
)

# Find OpenGL and GLUT packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

get_filename_component(C_COMPILER_DIR "${CMAKE_C_COMPILER}" DIRECTORY)
get_filename_component(ABOVE_MINGW_DIR "${C_COMPILER_DIR}/../.." ABSOLUTE) # Go up three directories
get_filename_component(ABOVE_MINGW_DIR_NAME "${ABOVE_MINGW_DIR}" NAME) # Extract only the last component of the path

# Checking if the user is using the mingw32 or msys64 method
if (ABOVE_MINGW_DIR_NAME STREQUAL "msys64")
    set(FREEGLUT_DLL_PATH "${ABOVE_MINGW_DIR}/mingw64/bin/libfreeglut.dll")
    set(SHUTTLE_ICON_FILE "shuttle_icon_64.o")
elseif (ABOVE_MINGW_DIR STREQUAL CMAKE_SOURCE_DIR) # mingw32 that is located inside the repo directory
    set(FREEGLUT_DLL_PATH "${CMAKE_SOURCE_DIR}/mingw32/bin/freeglut.dll")
    set(SHUTTLE_ICON_FILE "shuttle_icon.o")
else()
    # adjust the dependecies yourself here, i guess
endif()

# Create the executable
add_executable(ShuttleMadness ${SOURCES} ${HEADERS} ${SHUTTLE_ICON_FILE})

# Hide the console
if (WIN32)
    set_target_properties(ShuttleMadness PROPERTIES WIN32_EXECUTABLE true)
endif()

# Link libraries statically
target_link_libraries(ShuttleMadness PRIVATE
    -static-libgcc
    -static-libstdc++
    -static # for libwinpthread-1.dll
    OpenGL::GL
    GLUT::GLUT
)

# Copy DLLs to the build directory
add_custom_command(TARGET ShuttleMadness POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${FREEGLUT_DLL_PATH}"
        $<TARGET_FILE_DIR:ShuttleMadness>
)

# Copy resource files to the build directory
foreach(RESOURCE ${RESOURCES})
    configure_file(${RESOURCE} ${CMAKE_BINARY_DIR} COPYONLY)
endforeach()

# Copy text files to the build directory
foreach(TEXT ${TEXTS})
    configure_file(${TEXT} ${CMAKE_BINARY_DIR} COPYONLY)
endforeach()
