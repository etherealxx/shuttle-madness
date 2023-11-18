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

# Add dll files
file(GLOB DLL_FILES "*.dll")

# Find OpenGL and GLUT packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Create the executable
add_executable(ShuttleMadness ${SOURCES} ${HEADERS})

# Hide the console
if (WIN32)
    set_target_properties(ShuttleMadness PROPERTIES WIN32_EXECUTABLE true)
endif()

# Link libraries
target_link_libraries(ShuttleMadness PRIVATE OpenGL::GL GLUT::GLUT)

# Copy resource files to the build directory
foreach(RESOURCE ${RESOURCES})
    configure_file(${RESOURCE} ${CMAKE_BINARY_DIR} COPYONLY)
endforeach()

# Copy text files to the build directory
foreach(TEXT ${TEXTS})
    configure_file(${TEXT} ${CMAKE_BINARY_DIR} COPYONLY)
endforeach()

# Copy dll files to the build directory
foreach(DLL_FILE ${DLL_FILES})
    configure_file(${DLL_FILE} ${CMAKE_BINARY_DIR} COPYONLY)
endforeach()