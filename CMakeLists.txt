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

# Create the executable
add_executable(ShuttleMadness ${SOURCES} ${HEADERS} shuttle_icon.o)

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
        "${CMAKE_SOURCE_DIR}/mingw32/bin/freeglut.dll"
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
