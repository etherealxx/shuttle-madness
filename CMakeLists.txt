cmake_minimum_required(VERSION 3.5)

project(ShuttleMadness)

# Add the source files to the executable
file(GLOB SOURCES "*.cpp")

# Add the header files to the project
file(GLOB HEADERS "*.h")

# Add resource files
file(GLOB RESOURCES "*.wav" "*.png")

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